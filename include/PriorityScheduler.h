/**
 * @file PriorityScheduler.h
 * @brief Priority-based scheduling algorithm implementation
 * @author CPU Scheduler Team
 * @version 1.0
 */

#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "Scheduler.h"
#include <map>

/**
 * @class PriorityScheduler
 * @brief Implements Priority-based CPU scheduling algorithm
 * 
 * Priority scheduling assigns CPU based on process priority.
 * Supports both preemptive and non-preemptive modes.
 * Includes aging mechanism to prevent starvation.
 */
class PriorityScheduler : public Scheduler {
private:
    bool preemptive;                    ///< Preemptive mode flag
    bool agingEnabled;                  ///< Aging enabled flag
    int agingThreshold;                 ///< Time units before priority boost
    std::map<int, int> waitingSince;    ///< Track waiting time for aging

    /**
     * @brief Apply aging to waiting processes
     * Boosts priority of processes that have waited too long
     */
    void applyAging();

    /**
     * @brief Find highest priority process in ready queue
     * @return Index of highest priority process, -1 if none
     */
    int findHighestPriority();

    /**
     * @brief Check if preemption should occur
     * @param arriving Newly arrived process
     * @return true if current process should be preempted
     */
    bool shouldPreempt(const Process& arriving);

public:
    /**
     * @brief Constructor
     * @param preemptive Enable preemptive mode
     * @param config Scheduler configuration
     */
    explicit PriorityScheduler(bool preemptive = false, 
                                const SchedulerConfig& config = SchedulerConfig());

    /**
     * @brief Destructor
     */
    ~PriorityScheduler() override = default;

    /**
     * @brief Run the Priority scheduling simulation
     */
    void run() override;

    /**
     * @brief Get next highest priority process
     * @return Pointer to next process
     */
    Process* getNextProcess() override;

    /**
     * @brief Get algorithm name
     * @return Algorithm name with mode
     */
    std::string getName() const override;

    /**
     * @brief Get scheduler type
     * @return SchedulerType for priority scheduling
     */
    SchedulerType getType() const override;

    /**
     * @brief Reset scheduler state
     */
    void reset() override;

    /**
     * @brief Set preemptive mode
     * @param preempt Enable/disable preemption
     */
    void setPreemptive(bool preempt) { preemptive = preempt; }

    /**
     * @brief Check if preemptive mode is enabled
     * @return true if preemptive
     */
    bool isPreemptive() const { return preemptive; }

    /**
     * @brief Enable/disable aging
     * @param enabled Enable flag
     */
    void setAgingEnabled(bool enabled) { agingEnabled = enabled; }

    /**
     * @brief Check if aging is enabled
     * @return true if aging is enabled
     */
    bool isAgingEnabled() const { return agingEnabled; }

    /**
     * @brief Set aging threshold
     * @param threshold Time units before priority boost
     */
    void setAgingThreshold(int threshold) { agingThreshold = threshold; }

    /**
     * @brief Get aging threshold
     * @return Current aging threshold
     */
    int getAgingThreshold() const { return agingThreshold; }
};

#endif // PRIORITY_SCHEDULER_H
