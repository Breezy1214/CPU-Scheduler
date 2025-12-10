/**
 * @file MultilevelQueueScheduler.h
 * @brief Multilevel Queue scheduling algorithm implementation
 * @version 1.0
 */

#ifndef MULTILEVEL_QUEUE_SCHEDULER_H
#define MULTILEVEL_QUEUE_SCHEDULER_H

#include "Scheduler.h"
#include <vector>
#include <queue>

/**
 * @enum QueueType
 * @brief Types of queues in multilevel scheduling
 */
enum class QueueType {
    SYSTEM,         ///< Highest priority - system processes
    INTERACTIVE,    ///< Medium priority - interactive processes
    BATCH           ///< Lowest priority - batch processes
};

/**
 * @struct QueueConfig
 * @brief Configuration for a single queue level
 */
struct QueueConfig {
    QueueType type;
    int priority;           ///< Queue priority (lower = higher)
    int timeQuantum;        ///< Time quantum for this queue
    bool preemptive;        ///< Whether queue uses preemption
    std::string name;       ///< Queue name for display
};

/**
 * @class MultilevelQueueScheduler
 * @brief Implements Multilevel Queue CPU scheduling algorithm
 * 
 * Processes are permanently assigned to a queue based on properties
 * like priority or type. Each queue can have its own scheduling algorithm.
 * Higher priority queues must be empty before lower priority queues execute.
 */
class MultilevelQueueScheduler : public Scheduler {
private:
    std::vector<QueueConfig> queueConfigs;              ///< Configuration for each queue
    std::vector<std::vector<int>> queues;               ///< Process indices in each queue
    int numQueues;                                       ///< Number of queue levels
    int currentQueue;                                    ///< Currently active queue

    /**
     * @brief Assign process to appropriate queue based on priority
     * @param process Process to assign
     * @return Queue index
     */
    int assignToQueue(const Process& process);

    /**
     * @brief Get next non-empty queue
     * @return Queue index, -1 if all empty
     */
    int getActiveQueue();

    /**
     * @brief Execute processes in a specific queue
     * @param queueIdx Queue index
     * @return true if any process executed
     */
    bool executeQueue(int queueIdx);

public:
    /**
     * @brief Constructor
     * @param numQueues Number of queue levels (default: 3)
     * @param config Scheduler configuration
     */
    explicit MultilevelQueueScheduler(int numQueues = 3, 
                                       const SchedulerConfig& config = SchedulerConfig());

    /**
     * @brief Destructor
     */
    ~MultilevelQueueScheduler() override = default;

    /**
     * @brief Add process and assign to queue
     * @param process Process to add
     */
    void addProcess(const Process& process) override;

    /**
     * @brief Run the Multilevel Queue scheduling simulation
     */
    void run() override;

    /**
     * @brief Get next process from highest priority non-empty queue
     * @return Pointer to next process
     */
    Process* getNextProcess() override;

    /**
     * @brief Get algorithm name
     * @return "Multilevel Queue"
     */
    std::string getName() const override { return "Multilevel Queue"; }

    /**
     * @brief Get scheduler type
     * @return SchedulerType::MULTILEVEL_QUEUE
     */
    SchedulerType getType() const override { return SchedulerType::MULTILEVEL_QUEUE; }

    /**
     * @brief Reset scheduler state
     */
    void reset() override;

    /**
     * @brief Set queue configuration
     * @param idx Queue index
     * @param config Queue configuration
     */
    void setQueueConfig(int idx, const QueueConfig& qconfig);

    /**
     * @brief Get queue configuration
     * @param idx Queue index
     * @return Queue configuration
     */
    QueueConfig getQueueConfig(int idx) const;

    /**
     * @brief Print status of all queues
     */
    void printQueueStatus() const;

    /**
     * @brief Get number of processes in a queue
     * @param queueIdx Queue index
     * @return Number of processes
     */
    int getQueueSize(int queueIdx) const;
};

#endif // MULTILEVEL_QUEUE_SCHEDULER_H
