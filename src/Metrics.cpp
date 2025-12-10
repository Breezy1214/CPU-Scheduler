/**
 * @file Metrics.cpp
 * @brief Implementation of the Metrics class
 * @version 1.0
 */

#include "Metrics.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <fstream>

Metrics::Metrics()
    : avgWaitingTime(0.0)
    , avgTurnaroundTime(0.0)
    , avgResponseTime(0.0)
    , cpuUtilization(0.0)
    , throughput(0.0)
    , totalExecutionTime(0)
    , totalIdleTime(0)
    , totalContextSwitches(0)
    , contextSwitchOverhead(0)
    , processCount(0)
{}

void Metrics::reset() {
    avgWaitingTime = 0.0;
    avgTurnaroundTime = 0.0;
    avgResponseTime = 0.0;
    cpuUtilization = 0.0;
    throughput = 0.0;
    totalExecutionTime = 0;
    totalIdleTime = 0;
    totalContextSwitches = 0;
    contextSwitchOverhead = 0;
    processCount = 0;
    waitingTimes.clear();
    turnaroundTimes.clear();
    responseTimes.clear();
}

void Metrics::addWaitingTime(int time) {
    waitingTimes.push_back(time);
}

void Metrics::addTurnaroundTime(int time) {
    turnaroundTimes.push_back(time);
}

void Metrics::addResponseTime(int time) {
    responseTimes.push_back(time);
}

void Metrics::calculateAverages() {
    processCount = static_cast<int>(waitingTimes.size());
    
    if (processCount > 0) {
        double sumWaiting = std::accumulate(waitingTimes.begin(), 
                                            waitingTimes.end(), 0.0);
        double sumTurnaround = std::accumulate(turnaroundTimes.begin(), 
                                               turnaroundTimes.end(), 0.0);
        double sumResponse = std::accumulate(responseTimes.begin(), 
                                             responseTimes.end(), 0.0);
        
        avgWaitingTime = sumWaiting / processCount;
        avgTurnaroundTime = sumTurnaround / processCount;
        avgResponseTime = sumResponse / processCount;
    }
}

void Metrics::calculateUtilization(int totalTime, int idleTime, int switchOverhead) {
    totalExecutionTime = totalTime;
    totalIdleTime = idleTime;
    contextSwitchOverhead = switchOverhead;
    
    if (totalTime > 0) {
        int usefulTime = totalTime - idleTime - switchOverhead;
        cpuUtilization = (static_cast<double>(usefulTime) / totalTime) * 100.0;
    }
}

void Metrics::calculateThroughput(int totalTime) {
    if (totalTime > 0) {
        throughput = static_cast<double>(processCount) / totalTime;
    }
}

double Metrics::getWaitingTimeVariance() const {
    if (waitingTimes.size() < 2) return 0.0;
    
    double mean = avgWaitingTime;
    double variance = 0.0;
    
    for (int time : waitingTimes) {
        variance += (time - mean) * (time - mean);
    }
    
    return variance / (waitingTimes.size() - 1);
}

double Metrics::getTurnaroundTimeVariance() const {
    if (turnaroundTimes.size() < 2) return 0.0;
    
    double mean = avgTurnaroundTime;
    double variance = 0.0;
    
    for (int time : turnaroundTimes) {
        variance += (time - mean) * (time - mean);
    }
    
    return variance / (turnaroundTimes.size() - 1);
}

int Metrics::getMinWaitingTime() const {
    if (waitingTimes.empty()) return 0;
    return *std::min_element(waitingTimes.begin(), waitingTimes.end());
}

int Metrics::getMaxWaitingTime() const {
    if (waitingTimes.empty()) return 0;
    return *std::max_element(waitingTimes.begin(), waitingTimes.end());
}

void Metrics::printReport() const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               PERFORMANCE METRICS REPORT                     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Process Count:              " << std::setw(10) << processCount 
              << "                     ║\n";
    std::cout << "║ Total Execution Time:       " << std::setw(10) << totalExecutionTime 
              << " time units          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Average Waiting Time:       " << std::setw(10) << std::fixed 
              << std::setprecision(2) << avgWaitingTime << " time units          ║\n";
    std::cout << "║ Average Turnaround Time:    " << std::setw(10) << std::fixed 
              << std::setprecision(2) << avgTurnaroundTime << " time units          ║\n";
    std::cout << "║ Average Response Time:      " << std::setw(10) << std::fixed 
              << std::setprecision(2) << avgResponseTime << " time units          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ CPU Utilization:            " << std::setw(10) << std::fixed 
              << std::setprecision(2) << cpuUtilization << " %                   ║\n";
    std::cout << "║ Throughput:                 " << std::setw(10) << std::fixed 
              << std::setprecision(4) << throughput << " proc/time unit      ║\n";
    std::cout << "║ Context Switches:           " << std::setw(10) << totalContextSwitches 
              << "                     ║\n";
    std::cout << "║ Context Switch Overhead:    " << std::setw(10) << contextSwitchOverhead 
              << " time units          ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

void Metrics::printDetailedReport() const {
    printReport();
    
    if (!waitingTimes.empty()) {
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║               DETAILED STATISTICS                            ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Waiting Time:                                                ║\n";
        std::cout << "║   Min: " << std::setw(5) << getMinWaitingTime() 
                  << "  Max: " << std::setw(5) << getMaxWaitingTime()
                  << "  Variance: " << std::setw(8) << std::fixed 
                  << std::setprecision(2) << getWaitingTimeVariance() << "         ║\n";
        std::cout << "║ Turnaround Time Variance: " << std::setw(12) << std::fixed 
                  << std::setprecision(2) << getTurnaroundTimeVariance() << "                 ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Per-Process Metrics:                                         ║\n";
        std::cout << "║ Process | Wait Time | Turnaround | Response                  ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        
        for (size_t i = 0; i < waitingTimes.size(); ++i) {
            std::cout << "║    P" << std::setw(2) << i << "  |  " 
                      << std::setw(8) << waitingTimes[i] << " | "
                      << std::setw(10) << turnaroundTimes[i] << " | "
                      << std::setw(8) << responseTimes[i] << "             ║\n";
        }
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }
}

bool Metrics::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "Metric,Value\n";
    file << "Process Count," << processCount << "\n";
    file << "Total Execution Time," << totalExecutionTime << "\n";
    file << "Average Waiting Time," << avgWaitingTime << "\n";
    file << "Average Turnaround Time," << avgTurnaroundTime << "\n";
    file << "Average Response Time," << avgResponseTime << "\n";
    file << "CPU Utilization (%)," << cpuUtilization << "\n";
    file << "Throughput (proc/time)," << throughput << "\n";
    file << "Context Switches," << totalContextSwitches << "\n";
    file << "Context Switch Overhead," << contextSwitchOverhead << "\n";
    
    if (!waitingTimes.empty()) {
        file << "\nProcess,Waiting Time,Turnaround Time,Response Time\n";
        for (size_t i = 0; i < waitingTimes.size(); ++i) {
            file << "P" << i << "," 
                 << waitingTimes[i] << ","
                 << turnaroundTimes[i] << ","
                 << responseTimes[i] << "\n";
        }
    }
    
    file.close();
    return true;
}
