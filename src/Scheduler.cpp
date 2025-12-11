/**
 * @file Scheduler.cpp
 * @brief Implementation of the abstract Scheduler base class
 * @version 1.0
 */

#include "Scheduler.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

Scheduler::Scheduler(const SchedulerConfig& config)
    : config(config)
    , currentTime(0)
    , contextSwitches(0)
    , currentProcess(nullptr)
    , isRunning(false)
{}

void Scheduler::addProcess(const Process& process) {
    processes.push_back(process);
}

void Scheduler::clearProcesses() {
    processes.clear();
}

void Scheduler::addProcesses(const std::vector<Process>& procs) {
    for (const auto& p : procs) {
        addProcess(p);
    }
}

void Scheduler::checkArrivals(int time) {
    for (auto& process : processes) {
        if (process.getArrivalTime() == time && 
            process.getState() == ProcessState::NEW) {
            process.setState(ProcessState::READY);
            readyQueue.push_back(process);
        }
    }
}

void Scheduler::performContextSwitch(Process* from, Process* to) {
    if (from != nullptr && to != nullptr && from->getPid() != to->getPid()) {
        contextSwitches++;
        
        // Record context switch in timeline
        recordEvent(-1, currentTime, currentTime + config.contextSwitchTime, 
                    true, "Context Switch");
        
        // Simulate context switch time
        currentTime += config.contextSwitchTime;
    }
}

void Scheduler::recordEvent(int pid, int start, int end, bool isSwitch, 
                            const std::string& desc) {
    ExecutionEvent event;
    event.processId = pid;
    event.startTime = start;
    event.endTime = end;
    event.isContextSwitch = isSwitch;
    event.description = desc;
    timeline.push_back(event);
}

void Scheduler::reset() {
    readyQueue.clear();
    timeline.clear();
    metrics.reset();
    currentTime = 0;
    contextSwitches = 0;
    currentProcess = nullptr;
    isRunning = false;
    
    // Reset all processes
    for (auto& process : processes) {
        process.reset();
    }
}

void Scheduler::calculateMetrics() {
    metrics.reset();
    
    int totalBurstTime = 0;
    int idleTime = 0;
    int lastEndTime = 0;
    
    for (const auto& process : processes) {
        metrics.addWaitingTime(process.getWaitingTime());
        metrics.addTurnaroundTime(process.getTurnaroundTime());
        metrics.addResponseTime(process.getResponseTime());
        totalBurstTime += process.getBurstTime();
    }
    
    // Calculate idle time from timeline
    for (const auto& event : timeline) {
        if (!event.isContextSwitch && event.processId >= 0) {
            if (event.startTime > lastEndTime) {
                idleTime += (event.startTime - lastEndTime);
            }
            lastEndTime = event.endTime;
        }
    }
    
    metrics.calculateAverages();
    metrics.setTotalContextSwitches(contextSwitches);
    metrics.calculateUtilization(currentTime, idleTime, 
                                 contextSwitches * config.contextSwitchTime);
    metrics.calculateThroughput(currentTime);
}

void Scheduler::printGanttChart() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      GANTT CHART                             ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    
    if (timeline.empty()) {
        std::cout << "║                    (No execution data)                       ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
        return;
    }
    
    // Print timeline
    std::cout << "║ ";
    for (const auto& event : timeline) {
        if (event.isContextSwitch) {
            std::cout << "[CS] ";
        } else {
            std::cout << "[P" << event.processId << "] ";
        }
    }
    std::cout << std::setw(52) << "║\n";
    
    // Print time markers
    std::cout << "║ ";
    for (const auto& event : timeline) {
        std::cout << event.startTime << "-" << event.endTime << " ";
    }
    std::cout << std::setw(45) << "║\n";
    
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

void Scheduler::printReadyQueue() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    READY QUEUE STATUS                        ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    
    if (readyQueue.empty()) {
        std::cout << "║                    (Queue is empty)                          ║\n";
    } else {
        std::cout << "║ Position │ PID │ Priority │ Remaining │ Arrival             ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        
        int pos = 0;
        for (const auto& process : readyQueue) {
            std::cout << "║ " << std::setw(8) << pos++ 
                      << " │ " << std::setw(3) << process.getPid()
                      << " │ " << std::setw(8) << process.getPriority()
                      << " │ " << std::setw(9) << process.getRemainingTime()
                      << " │ " << std::setw(7) << process.getArrivalTime()
                      << "             ║\n";
        }
    }
    
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

bool Scheduler::isComplete() const {
    for (const auto& process : processes) {
        if (process.getState() != ProcessState::TERMINATED) {
            return false;
        }
    }
    return true;
}
