/**
 * @file Process.cpp
 * @brief Implementation of the Process class
 * @version 1.0
 */

#include "Process.h"
#include <sstream>

// Default constructor
Process::Process()
    : pid(0)
    , priority(0)
    , burstTime(0)
    , remainingTime(0)
    , arrivalTime(0)
    , waitingTime(0)
    , turnaroundTime(0)
    , responseTime(-1)
    , completionTime(0)
    , queueLevel(0)
    , hasStarted(false)
    , state(ProcessState::NEW)
    , name("")
{}

// Parameterized constructor
Process::Process(int pid, int priority, int burstTime, int arrivalTime, 
                 const std::string& name)
    : pid(pid)
    , priority(priority)
    , burstTime(burstTime)
    , remainingTime(burstTime)
    , arrivalTime(arrivalTime)
    , waitingTime(0)
    , turnaroundTime(0)
    , responseTime(-1)
    , completionTime(0)
    , queueLevel(0)
    , hasStarted(false)
    , state(ProcessState::NEW)
    , name(name.empty() ? "P" + std::to_string(pid) : name)
{}

int Process::execute(int timeSlice) {
    if (remainingTime == 0) {
        return 0;
    }
    
    // Mark as started and set response time if first execution
    if (!hasStarted) {
        hasStarted = true;
    }
    
    // Calculate actual execution time
    int executedTime = std::min(timeSlice, remainingTime);
    remainingTime -= executedTime;
    
    // Update state
    state = ProcessState::RUNNING;
    
    if (remainingTime == 0) {
        state = ProcessState::TERMINATED;
    }
    
    return executedTime;
}

void Process::reset() {
    remainingTime = burstTime;
    waitingTime = 0;
    turnaroundTime = 0;
    responseTime = -1;
    completionTime = 0;
    queueLevel = 0;
    hasStarted = false;
    state = ProcessState::NEW;
}

std::string Process::getStateString() const {
    switch (state) {
        case ProcessState::NEW:        return "NEW";
        case ProcessState::READY:      return "READY";
        case ProcessState::RUNNING:    return "RUNNING";
        case ProcessState::WAITING:    return "WAITING";
        case ProcessState::TERMINATED: return "TERMINATED";
        default:                       return "UNKNOWN";
    }
}

bool Process::operator<(const Process& other) const {
    // Lower priority value means higher priority
    if (priority != other.priority) {
        return priority < other.priority;
    }
    // Tie-breaker: earlier arrival time
    if (arrivalTime != other.arrivalTime) {
        return arrivalTime < other.arrivalTime;
    }
    // Final tie-breaker: lower PID
    return pid < other.pid;
}

bool Process::operator==(const Process& other) const {
    return pid == other.pid;
}

std::ostream& operator<<(std::ostream& os, const Process& p) {
    os << "Process[PID=" << p.pid 
       << ", Name=" << p.name
       << ", Priority=" << p.priority
       << ", Burst=" << p.burstTime
       << ", Remaining=" << p.remainingTime
       << ", Arrival=" << p.arrivalTime
       << ", State=" << p.getStateString()
       << "]";
    return os;
}
