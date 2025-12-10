/**
 * @file MultilevelFeedbackQueueScheduler.cpp
 * @brief Implementation of Multilevel Feedback Queue scheduling algorithm
 * @version 1.0
 */

#include "MultilevelFeedbackQueueScheduler.h"
#include <algorithm>

MultilevelFeedbackQueueScheduler::MultilevelFeedbackQueueScheduler(
    int numQueues, const SchedulerConfig& config)
    : Scheduler(config)
    , numQueues(numQueues)
    , agingEnabled(config.agingEnabled)
    , agingInterval(config.agingThreshold * 5)
    , lastBoostTime(0)
{
    queues.resize(numQueues);
    
    // Setup quantums - exponentially increasing
    quantums.resize(numQueues);
    quantums[0] = config.timeQuantum;
    for (int i = 1; i < numQueues; ++i) {
        quantums[i] = quantums[i-1] * 2;
    }
    
    // If custom quantums provided, use them
    if (!config.quantums.empty()) {
        for (size_t i = 0; i < config.quantums.size() && i < quantums.size(); ++i) {
            quantums[i] = config.quantums[i];
        }
    }
}

void MultilevelFeedbackQueueScheduler::demoteProcess(int processIdx) {
    Process& p = processes[processIdx];
    int currentQueue = p.getQueueLevel();
    
    if (currentQueue < numQueues - 1) {
        p.setQueueLevel(currentQueue + 1);
        processQueueMap[p.getPid()] = currentQueue + 1;
    }
}

void MultilevelFeedbackQueueScheduler::promoteProcess(int processIdx) {
    Process& p = processes[processIdx];
    int currentQueue = p.getQueueLevel();
    
    if (currentQueue > 0) {
        p.setQueueLevel(currentQueue - 1);
        processQueueMap[p.getPid()] = currentQueue - 1;
    }
}

void MultilevelFeedbackQueueScheduler::priorityBoost() {
    // Move all processes to the highest priority queue
    for (size_t i = 0; i < processes.size(); ++i) {
        Process& p = processes[i];
        if (p.getState() != ProcessState::TERMINATED) {
            p.setQueueLevel(0);
            processQueueMap[p.getPid()] = 0;
        }
    }
    
    // Rebuild queues
    for (int i = 0; i < numQueues; ++i) {
        while (!queues[i].empty()) {
            queues[i].pop();
        }
    }
    
    for (size_t i = 0; i < processes.size(); ++i) {
        Process& p = processes[i];
        if (p.getState() == ProcessState::READY) {
            queues[0].push(static_cast<int>(i));
        }
    }
    
    lastBoostTime = currentTime;
}

int MultilevelFeedbackQueueScheduler::getHighestPriorityQueue() {
    for (int i = 0; i < numQueues; ++i) {
        if (!queues[i].empty()) {
            return i;
        }
    }
    return -1;
}

int MultilevelFeedbackQueueScheduler::getQuantumForQueue(int queueLevel) const {
    if (queueLevel >= 0 && queueLevel < static_cast<int>(quantums.size())) {
        return quantums[queueLevel];
    }
    return config.timeQuantum;
}

void MultilevelFeedbackQueueScheduler::addProcess(const Process& process) {
    Scheduler::addProcess(process);
    int processIdx = static_cast<int>(processes.size()) - 1;
    processes[processIdx].setQueueLevel(0);  // Start at highest priority
    processQueueMap[process.getPid()] = 0;
    timeInQueue[process.getPid()] = 0;
}

void MultilevelFeedbackQueueScheduler::run() {
    currentTime = 0;
    timeline.clear();
    lastBoostTime = 0;
    
    // Initialize queues
    for (int i = 0; i < numQueues; ++i) {
        while (!queues[i].empty()) {
            queues[i].pop();
        }
    }
    
    processQueueMap.clear();
    timeInQueue.clear();
    
    // Set initial process states
    for (size_t i = 0; i < processes.size(); ++i) {
        Process& p = processes[i];
        p.reset();
        p.setQueueLevel(0);
        processQueueMap[p.getPid()] = 0;
        timeInQueue[p.getPid()] = 0;
        
        if (p.getArrivalTime() == 0) {
            p.setState(ProcessState::READY);
            queues[0].push(static_cast<int>(i));
        } else {
            p.setState(ProcessState::NEW);
        }
    }
    
    int completedProcesses = 0;
    int totalProcesses = static_cast<int>(processes.size());
    
    while (completedProcesses < totalProcesses) {
        // Priority boost to prevent starvation
        if (agingEnabled && (currentTime - lastBoostTime) >= agingInterval) {
            priorityBoost();
        }
        
        // Handle new arrivals - always start at highest priority queue
        for (size_t i = 0; i < processes.size(); ++i) {
            Process& p = processes[i];
            if (p.getState() == ProcessState::NEW && 
                p.getArrivalTime() == currentTime) {
                p.setState(ProcessState::READY);
                p.setQueueLevel(0);
                processQueueMap[p.getPid()] = 0;
                queues[0].push(static_cast<int>(i));
            }
        }
        
        // Get highest priority non-empty queue
        int activeQueue = getHighestPriorityQueue();
        
        if (activeQueue != -1 && !queues[activeQueue].empty()) {
            int processIdx = queues[activeQueue].front();
            queues[activeQueue].pop();
            
            Process& p = processes[processIdx];
            
            if (p.getState() == ProcessState::READY) {
                p.setState(ProcessState::RUNNING);
                
                // Record response time on first execution
                if (!p.getHasStarted()) {
                    p.setResponseTime(currentTime - p.getArrivalTime());
                    p.setHasStarted(true);
                }
                
                // Context switch overhead
                if (!timeline.empty() && 
                    timeline.back().processId != p.getPid()) {
                    currentTime += config.contextSwitchTime;
                    contextSwitches++;
                }
                
                // Execute for time quantum
                int quantum = getQuantumForQueue(activeQueue);
                int executionStart = currentTime;
                int actualTime = p.execute(quantum);
                currentTime += actualTime;
                
                // Record execution event
                timeline.push_back({p.getPid(), executionStart, currentTime});
                
                // Update waiting time for other ready processes
                for (auto& proc : processes) {
                    if (proc.getState() == ProcessState::READY && 
                        proc.getArrivalTime() <= currentTime &&
                        proc.getPid() != p.getPid()) {
                        proc.setWaitingTime(proc.getWaitingTime() + actualTime);
                    }
                }
                
                // Update time in queue
                timeInQueue[p.getPid()] += actualTime;
                
                // Check if process completed
                if (p.getRemainingTime() == 0) {
                    p.setState(ProcessState::TERMINATED);
                    p.setCompletionTime(currentTime);
                    p.setTurnaroundTime(currentTime - p.getArrivalTime());
                    completedProcesses++;
                } else {
                    // Process used its entire quantum - demote
                    if (actualTime >= quantum) {
                        demoteProcess(processIdx);
                    }
                    
                    // Return to appropriate queue
                    p.setState(ProcessState::READY);
                    int newQueue = p.getQueueLevel();
                    queues[newQueue].push(processIdx);
                }
            }
        } else {
            // CPU idle
            currentTime++;
        }
    }
    
    // Calculate metrics
    calculateMetrics();
}

Process* MultilevelFeedbackQueueScheduler::getNextProcess() {
    int activeQueue = getHighestPriorityQueue();
    if (activeQueue == -1 || queues[activeQueue].empty()) {
        return nullptr;
    }
    
    int processIdx = queues[activeQueue].front();
    return &processes[processIdx];
}

void MultilevelFeedbackQueueScheduler::reset() {
    Scheduler::reset();
    for (auto& queue : queues) {
        while (!queue.empty()) {
            queue.pop();
        }
    }
    processQueueMap.clear();
    timeInQueue.clear();
    lastBoostTime = 0;
}
