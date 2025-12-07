/**
 * @file Metrics.h
 * @brief Performance metrics calculation and storage
 * @author CPU Scheduler Team
 * @version 1.0
 */

#ifndef METRICS_H
#define METRICS_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * @class Metrics
 * @brief Calculates and stores CPU scheduling performance metrics
 * 
 * This class provides comprehensive metrics calculation including
 * average waiting time, turnaround time, response time, throughput,
 * and CPU utilization statistics.
 */
class Metrics {
private:
    double avgWaitingTime;          ///< Average waiting time
    double avgTurnaroundTime;       ///< Average turnaround time
    double avgResponseTime;         ///< Average response time
    double cpuUtilization;          ///< CPU utilization percentage
    double throughput;              ///< Processes completed per time unit
    int totalExecutionTime;         ///< Total simulation time
    int totalIdleTime;              ///< Total CPU idle time
    int totalContextSwitches;       ///< Number of context switches
    int contextSwitchOverhead;      ///< Total time spent in context switching
    int processCount;               ///< Number of processes

    // Individual process metrics for detailed analysis
    std::vector<int> waitingTimes;
    std::vector<int> turnaroundTimes;
    std::vector<int> responseTimes;

public:
    /**
     * @brief Default constructor
     */
    Metrics();

    /**
     * @brief Reset all metrics to initial values
     */
    void reset();

    // Setters
    void setAvgWaitingTime(double time) { avgWaitingTime = time; }
    void setAvgTurnaroundTime(double time) { avgTurnaroundTime = time; }
    void setAvgResponseTime(double time) { avgResponseTime = time; }
    void setCpuUtilization(double util) { cpuUtilization = util; }
    void setThroughput(double tp) { throughput = tp; }
    void setTotalExecutionTime(int time) { totalExecutionTime = time; }
    void setTotalIdleTime(int time) { totalIdleTime = time; }
    void setTotalContextSwitches(int count) { totalContextSwitches = count; }
    void setContextSwitchOverhead(int overhead) { contextSwitchOverhead = overhead; }
    void setProcessCount(int count) { processCount = count; }

    // Getters
    double getAvgWaitingTime() const { return avgWaitingTime; }
    double getAvgTurnaroundTime() const { return avgTurnaroundTime; }
    double getAvgResponseTime() const { return avgResponseTime; }
    double getCpuUtilization() const { return cpuUtilization; }
    double getThroughput() const { return throughput; }
    int getTotalExecutionTime() const { return totalExecutionTime; }
    int getTotalIdleTime() const { return totalIdleTime; }
    int getTotalContextSwitches() const { return totalContextSwitches; }
    int getContextSwitchOverhead() const { return contextSwitchOverhead; }
    int getProcessCount() const { return processCount; }

    /**
     * @brief Add individual waiting time
     * @param time Waiting time for a process
     */
    void addWaitingTime(int time);

    /**
     * @brief Add individual turnaround time
     * @param time Turnaround time for a process
     */
    void addTurnaroundTime(int time);

    /**
     * @brief Add individual response time
     * @param time Response time for a process
     */
    void addResponseTime(int time);

    /**
     * @brief Calculate averages from collected individual times
     */
    void calculateAverages();

    /**
     * @brief Calculate CPU utilization
     * @param totalTime Total simulation time
     * @param idleTime CPU idle time
     * @param switchOverhead Context switch overhead time
     */
    void calculateUtilization(int totalTime, int idleTime, int switchOverhead);

    /**
     * @brief Calculate throughput
     * @param totalTime Total simulation time
     */
    void calculateThroughput(int totalTime);

    /**
     * @brief Get variance of waiting times
     * @return Variance value
     */
    double getWaitingTimeVariance() const;

    /**
     * @brief Get variance of turnaround times
     * @return Variance value
     */
    double getTurnaroundTimeVariance() const;

    /**
     * @brief Get minimum waiting time
     * @return Minimum waiting time
     */
    int getMinWaitingTime() const;

    /**
     * @brief Get maximum waiting time
     * @return Maximum waiting time
     */
    int getMaxWaitingTime() const;

    /**
     * @brief Print formatted metrics report
     */
    void printReport() const;

    /**
     * @brief Print detailed per-process metrics
     */
    void printDetailedReport() const;

    /**
     * @brief Export metrics to CSV format
     * @param filename Output filename
     */
    void exportToCSV(const std::string& filename) const;

    /**
     * @brief Compare two metrics objects
     * @param other Other metrics to compare
     * @return Comparison summary string
     */
    std::string compareTo(const Metrics& other) const;

    /**
     * @brief Output stream operator
     */
    friend std::ostream& operator<<(std::ostream& os, const Metrics& m);
};

#endif // METRICS_H
