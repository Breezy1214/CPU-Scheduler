/**
 * @file RoundRobinScheduler.h
 * @brief Round Robin scheduling algorithm implementation
 * @author CPU Scheduler Team
 * @version 1.0
 */

#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"
#include <queue>

/**
 * @class RoundRobinScheduler
 * @brief Implements Round Robin CPU scheduling algorithm
 * 
 * Round Robin is a preemptive scheduling algorithm where each process
 * gets a fixed time quantum. If a process doesn't complete within its
 * quantum, it is moved to the back of the ready queue.
 */
class RoundRobinScheduler : public Scheduler {
private:
    int timeQuantum;                    ///< Time slice for each process
    std::queue<int> processQueue;       ///< Queue of process indices

    /**
     * @brief Check if any process is waiting
     * @return true if processes are waiting
     */
    bool hasWaitingProcesses() const;

public:
    /**
     * @brief Constructor
     * @param quantum Time quantum (default: 4)
     * @param config Scheduler configuration
     */
    explicit RoundRobinScheduler(int quantum = 4, 
                                  const SchedulerConfig& config = SchedulerConfig());

    /**
     * @brief Destructor
     */
    ~RoundRobinScheduler() override = default;

    /**
     * @brief Run the Round Robin scheduling simulation
     */
    void run() override;

    /**
     * @brief Get next process from the queue
     * @return Pointer to next process
     */
    Process* getNextProcess() override;

    /**
     * @brief Get algorithm name
     * @return "Round Robin"
     */
    std::string getName() const override { return "Round Robin"; }

    /**
     * @brief Get scheduler type
     * @return SchedulerType::ROUND_ROBIN
     */
    SchedulerType getType() const override { return SchedulerType::ROUND_ROBIN; }

    /**
     * @brief Reset scheduler state
     */
    void reset() override;

    /**
     * @brief Set time quantum
     * @param quantum New time quantum value
     */
    void setTimeQuantum(int quantum) { timeQuantum = quantum; }

    /**
     * @brief Get time quantum
     * @return Current time quantum
     */
    int getTimeQuantum() const { return timeQuantum; }
};

#endif // ROUND_ROBIN_SCHEDULER_H
