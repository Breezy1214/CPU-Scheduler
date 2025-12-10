/**
 * @file Scheduler.h
 * @brief Abstract base class for CPU scheduling algorithms
 * @version 1.0
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "Metrics.h"
#include <vector>
#include <queue>
#include <memory>
#include <string>
#include <functional>

/**
 * @enum SchedulerType
 * @brief Enumeration of available scheduling algorithms
 */
enum class SchedulerType {
    ROUND_ROBIN,
    PRIORITY_PREEMPTIVE,
    PRIORITY_NON_PREEMPTIVE,
    MULTILEVEL_QUEUE,
    MULTILEVEL_FEEDBACK_QUEUE
};

/**
 * @struct SchedulerConfig
 * @brief Configuration parameters for schedulers
 */
struct SchedulerConfig {
    int timeQuantum = 4;            ///< Time quantum for Round Robin
    int contextSwitchTime = 1;      ///< Context switch overhead
    int numQueues = 3;              ///< Number of queues for multilevel
    std::vector<int> quantums;      ///< Quantum for each queue level
    bool agingEnabled = true;       ///< Enable aging to prevent starvation
    int agingThreshold = 10;        ///< Time before priority boost
};

/**
 * @struct ExecutionEvent
 * @brief Records a single execution event for visualization
 */
struct ExecutionEvent {
    int processId;
    int startTime;
    int endTime;
    bool isContextSwitch;
    std::string description;
};

/**
 * @class Scheduler
 * @brief Abstract base class for all scheduling algorithms
 * 
 * Provides common interface and functionality for CPU scheduling algorithms.
 * Derived classes implement specific scheduling policies.
 */
class Scheduler {
protected:
    std::vector<Process> processes;          ///< All processes in the system
    std::vector<Process> readyQueue;         ///< Processes ready to execute
    std::vector<ExecutionEvent> timeline;    ///< Execution timeline for visualization
    SchedulerConfig config;                  ///< Scheduler configuration
    Metrics metrics;                         ///< Performance metrics
    int currentTime;                         ///< Current simulation time
    int contextSwitches;                     ///< Number of context switches
    Process* currentProcess;                 ///< Currently executing process
    bool isRunning;                          ///< Simulation running flag

    /**
     * @brief Add arrived processes to ready queue
     * @param time Current simulation time
     */
    virtual void checkArrivals(int time);

    /**
     * @brief Perform context switch
     * @param from Previous process (nullptr if none)
     * @param to Next process
     */
    virtual void performContextSwitch(Process* from, Process* to);

    /**
     * @brief Record an execution event
     */
    void recordEvent(int pid, int start, int end, bool isSwitch, 
                     const std::string& desc = "");

public:
    /**
     * @brief Constructor
     * @param config Scheduler configuration
     */
    explicit Scheduler(const SchedulerConfig& config = SchedulerConfig());

    /**
     * @brief Virtual destructor
     */
    virtual ~Scheduler() = default;

    /**
     * @brief Add a process to the scheduler
     * @param process Process to add
     */
    virtual void addProcess(const Process& process);

    /**
     * @brief Add multiple processes
     * @param procs Vector of processes
     */
    virtual void addProcesses(const std::vector<Process>& procs);

    /**
     * @brief Run the scheduling simulation
     */
    virtual void run() = 0;

    /**
     * @brief Get the next process to execute
     * @return Pointer to next process, nullptr if none ready
     */
    virtual Process* getNextProcess() = 0;

    /**
     * @brief Get the name of the scheduling algorithm
     * @return Algorithm name
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get the scheduler type
     * @return SchedulerType enum value
     */
    virtual SchedulerType getType() const = 0;

    /**
     * @brief Reset the scheduler state
     */
    virtual void reset();

    /**
     * @brief Calculate performance metrics
     */
    virtual void calculateMetrics();

    /**
     * @brief Get computed metrics
     * @return Metrics object
     */
    Metrics getMetrics() const { return metrics; }

    /**
     * @brief Get execution timeline
     * @return Vector of execution events
     */
    std::vector<ExecutionEvent> getTimeline() const { return timeline; }

    /**
     * @brief Get current ready queue state
     * @return Vector of processes in ready queue
     */
    std::vector<Process> getReadyQueue() const { return readyQueue; }

    /**
     * @brief Get all processes
     * @return Vector of all processes
     */
    std::vector<Process> getProcesses() const { return processes; }

    /**
     * @brief Get current simulation time
     * @return Current time
     */
    int getCurrentTime() const { return currentTime; }

    /**
     * @brief Get number of context switches
     * @return Context switch count
     */
    int getContextSwitches() const { return contextSwitches; }

    /**
     * @brief Set scheduler configuration
     * @param cfg New configuration
     */
    void setConfig(const SchedulerConfig& cfg) { config = cfg; }

    /**
     * @brief Get scheduler configuration
     * @return Current configuration
     */
    SchedulerConfig getConfig() const { return config; }

    /**
     * @brief Print Gantt chart of execution
     */
    virtual void printGanttChart() const;

    /**
     * @brief Print ready queue status
     */
    virtual void printReadyQueue() const;

    /**
     * @brief Check if simulation is complete
     * @return true if all processes completed
     */
    bool isComplete() const;
};

#endif // SCHEDULER_H
