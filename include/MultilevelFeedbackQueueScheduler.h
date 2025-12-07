/**
 * @file MultilevelFeedbackQueueScheduler.h
 * @brief Multilevel Feedback Queue scheduling algorithm implementation
 * @author CPU Scheduler Team
 * @version 1.0
 */

#ifndef MULTILEVEL_FEEDBACK_QUEUE_SCHEDULER_H
#define MULTILEVEL_FEEDBACK_QUEUE_SCHEDULER_H

#include "Scheduler.h"
#include <vector>
#include <queue>
#include <map>

/**
 * @class MultilevelFeedbackQueueScheduler
 * @brief Implements Multilevel Feedback Queue CPU scheduling algorithm
 * 
 * MLFQ allows processes to move between queues based on their behavior.
 * CPU-bound processes move to lower priority queues, while I/O-bound
 * processes stay in higher priority queues. Includes aging to prevent starvation.
 */
class MultilevelFeedbackQueueScheduler : public Scheduler {
private:
    int numQueues;                              ///< Number of queue levels
    std::vector<int> quantums;                  ///< Time quantum for each queue
    std::vector<std::queue<int>> queues;        ///< Process queues
    std::map<int, int> processQueueMap;         ///< Map PID to queue level
    std::map<int, int> timeInQueue;             ///< Track time in current queue
    bool agingEnabled;                          ///< Enable aging mechanism
    int agingInterval;                          ///< Interval for priority boost
    int lastBoostTime;                          ///< Last time priority boost occurred

    /**
     * @brief Demote process to lower priority queue
     * @param processIdx Process index to demote
     */
    void demoteProcess(int processIdx);

    /**
     * @brief Promote process to higher priority queue
     * @param processIdx Process index to promote
     */
    void promoteProcess(int processIdx);

    /**
     * @brief Perform priority boost (move all to top queue)
     * Called periodically to prevent starvation
     */
    void priorityBoost();

    /**
     * @brief Get the highest priority non-empty queue
     * @return Queue index, -1 if all empty
     */
    int getHighestPriorityQueue();

    /**
     * @brief Get time quantum for a specific queue
     * @param queueLevel Queue level
     * @return Time quantum
     */
    int getQuantumForQueue(int queueLevel) const;

public:
    /**
     * @brief Constructor
     * @param numQueues Number of queue levels (default: 3)
     * @param config Scheduler configuration
     */
    explicit MultilevelFeedbackQueueScheduler(int numQueues = 3, 
                                               const SchedulerConfig& config = SchedulerConfig());

    /**
     * @brief Destructor
     */
    ~MultilevelFeedbackQueueScheduler() override = default;

    /**
     * @brief Add process to highest priority queue
     * @param process Process to add
     */
    void addProcess(const Process& process) override;

    /**
     * @brief Run the MLFQ scheduling simulation
     */
    void run() override;

    /**
     * @brief Get next process from highest priority non-empty queue
     * @return Pointer to next process
     */
    Process* getNextProcess() override;

    /**
     * @brief Get algorithm name
     * @return "Multilevel Feedback Queue"
     */
    std::string getName() const override { return "Multilevel Feedback Queue"; }

    /**
     * @brief Get scheduler type
     * @return SchedulerType::MULTILEVEL_FEEDBACK_QUEUE
     */
    SchedulerType getType() const override { return SchedulerType::MULTILEVEL_FEEDBACK_QUEUE; }

    /**
     * @brief Reset scheduler state
     */
    void reset() override;

    /**
     * @brief Set time quantum for a queue level
     * @param level Queue level
     * @param quantum Time quantum
     */
    void setQuantum(int level, int quantum);

    /**
     * @brief Get quantums for all queues
     * @return Vector of quantums
     */
    std::vector<int> getQuantums() const { return quantums; }

    /**
     * @brief Enable/disable aging
     * @param enabled Enable flag
     */
    void setAgingEnabled(bool enabled) { agingEnabled = enabled; }

    /**
     * @brief Set aging interval
     * @param interval Time between priority boosts
     */
    void setAgingInterval(int interval) { agingInterval = interval; }

    /**
     * @brief Print current queue status
     */
    void printQueueStatus() const;

    /**
     * @brief Get the queue level of a process
     * @param pid Process ID
     * @return Queue level
     */
    int getProcessQueueLevel(int pid) const;
};

#endif // MULTILEVEL_FEEDBACK_QUEUE_SCHEDULER_H
