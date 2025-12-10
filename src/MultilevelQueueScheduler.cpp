/**
 * @file MultilevelQueueScheduler.cpp
 * @brief Implementation of Multilevel Queue scheduling algorithm
 * @version 1.0
 */

#include "MultilevelQueueScheduler.h"
#include <algorithm>

MultilevelQueueScheduler::MultilevelQueueScheduler(int numQueues, 
                                                     const SchedulerConfig& config)
    : Scheduler(config)
    , numQueues(numQueues)
    , currentQueue(0)
{
    queues.resize(numQueues);
    
    // Setup default queue configurations
    queueConfigs.resize(numQueues);
    
    // Queue 0: System (highest priority)
    queueConfigs[0] = {
        QueueType::SYSTEM,
        0,
        config.timeQuantum / 2,  // Smaller quantum for system tasks
        true,
        "System"
    };
    
    // Queue 1: Interactive (medium priority)
    if (numQueues > 1) {
        queueConfigs[1] = {
            QueueType::INTERACTIVE,
            1,
            config.timeQuantum,
            true,
            "Interactive"
        };
    }
    
    // Queue 2+: Batch (lower priority)
    for (int i = 2; i < numQueues; ++i) {
        queueConfigs[i] = {
            QueueType::BATCH,
            i,
            config.timeQuantum * 2,  // Larger quantum for batch
            false,
            "Batch-" + std::to_string(i - 1)
        };
    }
}

int MultilevelQueueScheduler::assignToQueue(const Process& process) {
    // Assign based on priority
    // Priority 0-2: System queue (0)
    // Priority 3-5: Interactive queue (1)
    // Priority 6+: Batch queue (2+)
    
    int priority = process.getPriority();
    
    if (priority <= 2) {
        return 0;  // System queue
    } else if (priority <= 5 && numQueues > 1) {
        return 1;  // Interactive queue
    } else {
        return std::min(numQueues - 1, 2);  // Batch queue
    }
}

int MultilevelQueueScheduler::getActiveQueue() {
    // Return highest priority non-empty queue
    for (int i = 0; i < numQueues; ++i) {
        if (!queues[i].empty()) {
            return i;
        }
    }
    return -1;
}

bool MultilevelQueueScheduler::executeQueue(int queueIdx) {
    if (queueIdx < 0 || queueIdx >= numQueues || queues[queueIdx].empty()) {
        return false;
    }
    
    int processIdx = queues[queueIdx].front();
    queues[queueIdx].erase(queues[queueIdx].begin());
    
    Process& p = processes[processIdx];
    
    if (p.getState() != ProcessState::READY || 
        p.getArrivalTime() > currentTime) {
        return false;
    }
    
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
    int quantum = queueConfigs[queueIdx].timeQuantum;
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
    
    // Check if process completed
    if (p.getRemainingTime() == 0) {
        p.setState(ProcessState::TERMINATED);
        p.setCompletionTime(currentTime);
        p.setTurnaroundTime(currentTime - p.getArrivalTime());
    } else {
        // Return to same queue
        p.setState(ProcessState::READY);
        queues[queueIdx].push_back(processIdx);
    }
    
    return true;
}

void MultilevelQueueScheduler::addProcess(const Process& process) {
    Scheduler::addProcess(process);
    int queueIdx = assignToQueue(process);
    int processIdx = static_cast<int>(processes.size()) - 1;
    processes[processIdx].setQueueLevel(queueIdx);
}

void MultilevelQueueScheduler::run() {
    currentTime = 0;
    timeline.clear();
    
    // Initialize queues and process states
    for (int i = 0; i < numQueues; ++i) {
        queues[i].clear();
    }
    
    for (size_t i = 0; i < processes.size(); ++i) {
        Process& p = processes[i];
        int queueIdx = assignToQueue(p);
        p.setQueueLevel(queueIdx);
        p.reset();
        
        if (p.getArrivalTime() == 0) {
            p.setState(ProcessState::READY);
            queues[queueIdx].push_back(static_cast<int>(i));
        } else {
            p.setState(ProcessState::NEW);
        }
    }
    
    int completedProcesses = 0;
    int totalProcesses = static_cast<int>(processes.size());
    
    while (completedProcesses < totalProcesses) {
        // Handle new arrivals
        for (size_t i = 0; i < processes.size(); ++i) {
            Process& p = processes[i];
            if (p.getState() == ProcessState::NEW && 
                p.getArrivalTime() == currentTime) {
                p.setState(ProcessState::READY);
                int queueIdx = p.getQueueLevel();
                queues[queueIdx].push_back(static_cast<int>(i));
            }
        }
        
        // Get highest priority non-empty queue
        int activeQueue = getActiveQueue();
        
        if (activeQueue != -1) {
            executeQueue(activeQueue);
            
            // Check for completed processes
            for (const auto& p : processes) {
                if (p.getState() == ProcessState::TERMINATED && 
                    p.getCompletionTime() == currentTime) {
                    completedProcesses++;
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

Process* MultilevelQueueScheduler::getNextProcess() {
    int activeQueue = getActiveQueue();
    if (activeQueue == -1 || queues[activeQueue].empty()) {
        return nullptr;
    }
    
    int processIdx = queues[activeQueue].front();
    return &processes[processIdx];
}

void MultilevelQueueScheduler::reset() {
    Scheduler::reset();
    for (auto& queue : queues) {
        queue.clear();
    }
    currentQueue = 0;
}
