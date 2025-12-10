/**
 * @file RoundRobinScheduler.cpp
 * @brief Implementation of Round Robin scheduling algorithm
 * @version 1.0
 */

#include "RoundRobinScheduler.h"
#include <iostream>
#include <algorithm>
#include <climits>

RoundRobinScheduler::RoundRobinScheduler(int quantum, const SchedulerConfig& config)
    : Scheduler(config)
    , timeQuantum(quantum > 0 ? quantum : config.timeQuantum)
{}

bool RoundRobinScheduler::hasWaitingProcesses() const {
    for (const auto& process : processes) {
        if (process.getState() != ProcessState::TERMINATED) {
            return true;
        }
    }
    return false;
}

void RoundRobinScheduler::run() {
    if (processes.empty()) {
        return;
    }
    
    reset();
    isRunning = true;
    
    // Sort processes by arrival time
    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.getArrivalTime() < b.getArrivalTime();
              });
    
    // Initialize: check arrivals at time 0
    checkArrivals(0);
    
    // Build initial queue from ready processes
    for (size_t i = 0; i < readyQueue.size(); ++i) {
        processQueue.push(static_cast<int>(i));
    }
    
    Process* lastProcess = nullptr;
    
    while (!isComplete()) {
        // Check for new arrivals
        checkArrivals(currentTime);
        
        // Add newly arrived processes to the circular queue
        for (size_t i = 0; i < processes.size(); ++i) {
            bool inQueue = false;
            std::queue<int> tempQueue = processQueue;
            while (!tempQueue.empty()) {
                if (tempQueue.front() == static_cast<int>(i)) {
                    inQueue = true;
                    break;
                }
                tempQueue.pop();
            }
            
            if (!inQueue && 
                processes[i].getState() == ProcessState::READY &&
                processes[i].getArrivalTime() <= currentTime &&
                !processes[i].isCompleted()) {
                processQueue.push(static_cast<int>(i));
            }
        }
        
        if (processQueue.empty()) {
            // CPU idle - advance time to next arrival
            int nextArrival = INT_MAX;
            for (const auto& p : processes) {
                if (p.getArrivalTime() > currentTime && 
                    p.getState() != ProcessState::TERMINATED) {
                    nextArrival = std::min(nextArrival, p.getArrivalTime());
                }
            }
            
            if (nextArrival != INT_MAX) {
                recordEvent(-1, currentTime, nextArrival, false, "CPU Idle");
                currentTime = nextArrival;
            } else {
                break;
            }
            continue;
        }
        
        // Get next process from queue
        int processIdx = processQueue.front();
        processQueue.pop();
        
        Process& currentProc = processes[processIdx];
        
        // Skip if already completed
        if (currentProc.isCompleted()) {
            continue;
        }
        
        // Context switch if different process
        if (lastProcess != nullptr && lastProcess->getPid() != currentProc.getPid()) {
            performContextSwitch(lastProcess, &currentProc);
        }
        
        // Set response time if first execution
        if (!currentProc.getHasStarted()) {
            currentProc.setResponseTime(currentTime - currentProc.getArrivalTime());
            currentProc.setHasStarted(true);
        }
        
        currentProc.setState(ProcessState::RUNNING);
        
        // Execute for time quantum or remaining time, whichever is smaller
        int executeTime = std::min(timeQuantum, currentProc.getRemainingTime());
        int startTime = currentTime;
        
        // Execute the process
        currentProc.execute(executeTime);
        currentTime += executeTime;
        
        // Record execution event
        recordEvent(currentProc.getPid(), startTime, currentTime, false, 
                    "Execute P" + std::to_string(currentProc.getPid()));
        
        // Check for new arrivals during execution
        for (int t = startTime + 1; t <= currentTime; ++t) {
            checkArrivals(t);
        }
        
        // Add newly arrived processes to queue (they should go after current process if it continues)
        for (size_t i = 0; i < processes.size(); ++i) {
            if (static_cast<int>(i) != processIdx) {
                bool inQueue = false;
                std::queue<int> tempQueue = processQueue;
                while (!tempQueue.empty()) {
                    if (tempQueue.front() == static_cast<int>(i)) {
                        inQueue = true;
                        break;
                    }
                    tempQueue.pop();
                }
                
                if (!inQueue && 
                    processes[i].getState() == ProcessState::READY &&
                    processes[i].getArrivalTime() <= currentTime &&
                    !processes[i].isCompleted()) {
                    processQueue.push(static_cast<int>(i));
                }
            }
        }
        
        // Handle process completion or preemption
        if (currentProc.isCompleted()) {
            currentProc.setState(ProcessState::TERMINATED);
            currentProc.setCompletionTime(currentTime);
            currentProc.setTurnaroundTime(currentTime - currentProc.getArrivalTime());
            currentProc.setWaitingTime(currentProc.getTurnaroundTime() - 
                                       currentProc.getBurstTime());
        } else {
            // Preempted - add back to queue
            currentProc.setState(ProcessState::READY);
            processQueue.push(processIdx);
        }
        
        lastProcess = &currentProc;
    }
    
    // Calculate final metrics
    calculateMetrics();
    isRunning = false;
}

Process* RoundRobinScheduler::getNextProcess() {
    if (processQueue.empty()) {
        return nullptr;
    }
    
    int idx = processQueue.front();
    return &processes[idx];
}

void RoundRobinScheduler::reset() {
    Scheduler::reset();
    
    // Clear the process queue
    while (!processQueue.empty()) {
        processQueue.pop();
    }
}
