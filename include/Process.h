/**
 * @file Process.h
 * @brief Process class definition for CPU Scheduler Simulator
 * @author CPU Scheduler Team
 * @version 1.0
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>

/**
 * @enum ProcessState
 * @brief Represents the possible states of a process
 */
enum class ProcessState {
    NEW,        ///< Process is newly created
    READY,      ///< Process is ready to execute
    RUNNING,    ///< Process is currently executing
    WAITING,    ///< Process is waiting for I/O or event
    TERMINATED  ///< Process has finished execution
};

/**
 * @class Process
 * @brief Represents a process in the CPU scheduler simulation
 * 
 * This class encapsulates all process-related data including timing information,
 * priority levels, and state tracking for comprehensive CPU scheduling simulation.
 */
class Process {
private:
    int pid;                    ///< Process ID (unique identifier)
    int priority;               ///< Priority level (lower value = higher priority)
    int burstTime;              ///< Total CPU burst time required
    int remainingTime;          ///< Remaining CPU time needed
    int arrivalTime;            ///< Time when process arrives in the system
    int waitingTime;            ///< Total time spent waiting in ready queue
    int turnaroundTime;         ///< Total time from arrival to completion
    int responseTime;           ///< Time from arrival to first CPU execution
    int completionTime;         ///< Time when process completed execution
    int queueLevel;             ///< Current queue level (for multilevel queues)
    bool hasStarted;            ///< Flag indicating if process has started execution
    ProcessState state;         ///< Current state of the process
    std::string name;           ///< Human-readable process name

public:
    /**
     * @brief Default constructor
     */
    Process();

    /**
     * @brief Parameterized constructor
     * @param pid Process ID
     * @param priority Priority level
     * @param burstTime CPU burst time
     * @param arrivalTime Arrival time
     * @param name Process name (optional)
     */
    Process(int pid, int priority, int burstTime, int arrivalTime, 
            const std::string& name = "");

    /**
     * @brief Copy constructor
     */
    Process(const Process& other) = default;

    /**
     * @brief Move constructor
     */
    Process(Process&& other) noexcept = default;

    /**
     * @brief Copy assignment operator
     */
    Process& operator=(const Process& other) = default;

    /**
     * @brief Move assignment operator
     */
    Process& operator=(Process&& other) noexcept = default;

    /**
     * @brief Destructor
     */
    ~Process() = default;

    // Getters
    int getPid() const { return pid; }
    int getPriority() const { return priority; }
    int getBurstTime() const { return burstTime; }
    int getRemainingTime() const { return remainingTime; }
    int getArrivalTime() const { return arrivalTime; }
    int getWaitingTime() const { return waitingTime; }
    int getTurnaroundTime() const { return turnaroundTime; }
    int getResponseTime() const { return responseTime; }
    int getCompletionTime() const { return completionTime; }
    int getQueueLevel() const { return queueLevel; }
    bool getHasStarted() const { return hasStarted; }
    ProcessState getState() const { return state; }
    std::string getName() const { return name; }

    // Setters
    void setPid(int pid) { this->pid = pid; }
    void setPriority(int priority) { this->priority = priority; }
    void setBurstTime(int burstTime) { this->burstTime = burstTime; }
    void setRemainingTime(int time) { this->remainingTime = time; }
    void setArrivalTime(int time) { this->arrivalTime = time; }
    void setWaitingTime(int time) { this->waitingTime = time; }
    void setTurnaroundTime(int time) { this->turnaroundTime = time; }
    void setResponseTime(int time) { this->responseTime = time; }
    void setCompletionTime(int time) { this->completionTime = time; }
    void setQueueLevel(int level) { this->queueLevel = level; }
    void setHasStarted(bool started) { this->hasStarted = started; }
    void setState(ProcessState state) { this->state = state; }
    void setName(const std::string& name) { this->name = name; }

    /**
     * @brief Execute the process for a given time slice
     * @param timeSlice Amount of time to execute
     * @return Actual time executed (may be less if process completes)
     */
    int execute(int timeSlice);

    /**
     * @brief Check if process has completed execution
     * @return true if remaining time is 0
     */
    bool isCompleted() const { return remainingTime == 0; }

    /**
     * @brief Reset process to initial state
     */
    void reset();

    /**
     * @brief Get string representation of process state
     * @return State as string
     */
    std::string getStateString() const;

    /**
     * @brief Comparison operator for priority-based ordering
     */
    bool operator<(const Process& other) const;

    /**
     * @brief Equality operator
     */
    bool operator==(const Process& other) const;

    /**
     * @brief Output stream operator
     */
    friend std::ostream& operator<<(std::ostream& os, const Process& p);
};

#endif // PROCESS_H
