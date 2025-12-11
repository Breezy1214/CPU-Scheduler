/**
 * @file PriorityScheduler.cpp
 * @brief Implementation of Priority-based scheduling algorithm
 * @version 1.0
 */

#include "PriorityScheduler.h"
#include <algorithm>
#include <limits>

PriorityScheduler::PriorityScheduler(bool preemptive, const SchedulerConfig& config)
    : Scheduler(config)
    , preemptive(preemptive)
    , agingEnabled(config.agingEnabled)
    , agingThreshold(config.agingThreshold)
{
}

void PriorityScheduler::applyAging() {
    if (!agingEnabled) return;
    
    for (size_t i = 0; i < processes.size(); ++i) {
        Process& p = processes[i];
        if (p.getState() == ProcessState::READY) {
            // Check how long the process has been waiting
            if (waitingSince.find(p.getPid()) == waitingSince.end()) {
                waitingSince[p.getPid()] = currentTime;
            }
            
            int waitingTime = currentTime - waitingSince[p.getPid()];
            if (waitingTime >= agingThreshold) {
                // Boost priority (decrease priority value)
                int currentPriority = p.getPriority();
                if (currentPriority > 0) {
                    p.setPriority(currentPriority - 1);
                    waitingSince[p.getPid()] = currentTime;
                }
            }
        }
    }
}

int PriorityScheduler::findHighestPriority() {
    int highestPriorityIdx = -1;
    int highestPriority = std::numeric_limits<int>::max();
    
    for (size_t i = 0; i < processes.size(); ++i) {
        const Process& p = processes[i];
        if (p.getState() == ProcessState::READY && 
            p.getArrivalTime() <= currentTime) {
            if (p.getPriority() < highestPriority) {
                highestPriority = p.getPriority();
                highestPriorityIdx = static_cast<int>(i);
            } else if (p.getPriority() == highestPriority) {
                // Tie-breaker: choose process with earlier arrival time
                if (highestPriorityIdx == -1 || 
                    p.getArrivalTime() < processes[highestPriorityIdx].getArrivalTime()) {
                    highestPriorityIdx = static_cast<int>(i);
                }
            }
        }
    }
    
    return highestPriorityIdx;
}

bool PriorityScheduler::shouldPreempt(const Process& arriving) {
    if (!preemptive || currentProcess == nullptr) {
        return false;
    }
    
    return arriving.getPriority() < currentProcess->getPriority();
}

void PriorityScheduler::run() {
    currentTime = 0;
    int currentProcessIdx = -1;
    timeline.clear();
    contextSwitches = 0;
    
    // Reset all processes first, then set initial states
    for (auto& p : processes) {
        p.reset();
        if (p.getArrivalTime() == 0) {
            p.setState(ProcessState::READY);
        }
    }
    
    int completedProcesses = 0;
    int totalProcesses = static_cast<int>(processes.size());
    
    while (completedProcesses < totalProcesses) {
        // Handle new arrivals
        for (auto& p : processes) {
            if (p.getState() == ProcessState::NEW && 
                p.getArrivalTime() <= currentTime) {
                p.setState(ProcessState::READY);
                
                // Check for preemption
                if (preemptive && shouldPreempt(p)) {
                    if (currentProcessIdx != -1) {
                        processes[currentProcessIdx].setState(ProcessState::READY);
                        contextSwitches++;
                        currentProcessIdx = -1;
                    }
                }
            }
        }
        
        // Apply aging to prevent starvation
        applyAging();
        
        // If no current process, find highest priority process
        if (currentProcessIdx == -1) {
            currentProcessIdx = findHighestPriority();
            
            if (currentProcessIdx != -1) {
                Process& selected = processes[currentProcessIdx];
                selected.setState(ProcessState::RUNNING);
                
                // Record response time on first execution
                if (!selected.getHasStarted()) {
                    selected.setResponseTime(currentTime - selected.getArrivalTime());
                    selected.setHasStarted(true);
                }
                
                // Context switch overhead
                if (!timeline.empty() && timeline.back().processId != selected.getPid()) {
                    currentTime += config.contextSwitchTime;
                }
                
                waitingSince.erase(selected.getPid());
            } else {
                // CPU idle - no ready processes
                currentTime++;
                continue;
            }
        }
        
        // Execute current process
        Process& current = processes[currentProcessIdx];
        int executionStart = currentTime;
        
        // In non-preemptive mode, execute until completion
        int executionTime = preemptive ? 1 : current.getRemainingTime();
        int actualTime = current.execute(executionTime);
        currentTime += actualTime;
        
        // Record execution event
        timeline.push_back({current.getPid(), executionStart, currentTime});
        
        // Update waiting time for other ready processes
        for (auto& p : processes) {
            if (p.getState() == ProcessState::READY && 
                p.getArrivalTime() <= currentTime) {
                p.setWaitingTime(p.getWaitingTime() + actualTime);
            }
        }
        
        // Check if process completed
        if (current.getRemainingTime() == 0) {
            current.setState(ProcessState::TERMINATED);
            current.setCompletionTime(currentTime);
            current.setTurnaroundTime(currentTime - current.getArrivalTime());
            completedProcesses++;
            currentProcessIdx = -1;
        } else if (preemptive) {
            // Check if a higher priority process has arrived
            int higherPriorityIdx = findHighestPriority();
            if (higherPriorityIdx != -1 && higherPriorityIdx != currentProcessIdx) {
                const Process& higherPriority = processes[higherPriorityIdx];
                if (higherPriority.getPriority() < current.getPriority()) {
                    current.setState(ProcessState::READY);
                    contextSwitches++;
                    currentProcessIdx = -1;
                }
            }
        }
    }
    
    // Calculate metrics
    calculateMetrics();
}

Process* PriorityScheduler::getNextProcess() {
    int idx = findHighestPriority();
    return (idx != -1) ? &processes[idx] : nullptr;
}

std::string PriorityScheduler::getName() const {
    return preemptive ? "Priority (Preemptive)" : "Priority (Non-Preemptive)";
}

SchedulerType PriorityScheduler::getType() const {
    return preemptive ? SchedulerType::PRIORITY_PREEMPTIVE : 
                        SchedulerType::PRIORITY_NON_PREEMPTIVE;
}

void PriorityScheduler::reset() {
    Scheduler::reset();
    waitingSince.clear();
}
