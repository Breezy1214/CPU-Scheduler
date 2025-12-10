/**
 * @file Simulator.h
 * @brief Main simulation controller
 * @version 1.0
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Scheduler.h"
#include "RoundRobinScheduler.h"
#include "PriorityScheduler.h"
#include "MultilevelQueueScheduler.h"
#include "MultilevelFeedbackQueueScheduler.h"
#include "Visualizer.h"
#include "Process.h"
#include "Metrics.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <random>

/**
 * @struct SimulationConfig
 * @brief Configuration for simulation runs
 */
struct SimulationConfig {
    bool realTimeVisualization = true;      ///< Enable real-time display
    bool showGanttChart = true;             ///< Show Gantt chart after run
    bool showMetrics = true;                ///< Display metrics
    bool compareAlgorithms = false;         ///< Run comparison mode
    int visualizationDelay = 100;           ///< Delay between frames (ms)
    bool dynamicArrivals = false;           ///< Enable dynamic process arrivals
    int maxSimulationTime = 1000;           ///< Maximum simulation time
};

/**
 * @class Simulator
 * @brief Main controller for CPU scheduling simulation
 * 
 * Manages scheduler instances, process generation, simulation execution,
 * and results collection. Supports interactive and batch modes.
 */
class Simulator {
private:
    std::vector<std::unique_ptr<Scheduler>> schedulers;
    std::vector<Process> baseProcesses;
    std::unique_ptr<Visualizer> visualizer;
    SimulationConfig simConfig;
    SchedulerConfig schedConfig;
    std::vector<Metrics> results;
    std::mt19937 rng;

    /**
     * @brief Generate random processes
     * @param count Number of processes
     * @param maxBurst Maximum burst time
     * @param maxArrival Maximum arrival time
     * @param maxPriority Maximum priority value
     * @return Vector of generated processes
     */
    std::vector<Process> generateRandomProcesses(int count, int maxBurst,
                                                  int maxArrival, int maxPriority);

    /**
     * @brief Callback for dynamic process arrival
     * @param currentTime Current simulation time
     * @return New process if one arrives, nullptr otherwise
     */
    Process* dynamicArrivalCallback(int currentTime);

public:
    /**
     * @brief Constructor
     */
    Simulator();

    /**
     * @brief Destructor
     */
    ~Simulator() = default;

    /**
     * @brief Initialize simulator with configuration
     * @param simCfg Simulation configuration
     * @param schedCfg Scheduler configuration
     */
    void initialize(const SimulationConfig& simCfg = SimulationConfig(),
                    const SchedulerConfig& schedCfg = SchedulerConfig());

    /**
     * @brief Add a scheduler to the simulation
     * @param type Type of scheduler to add
     */
    void addScheduler(SchedulerType type);

    /**
     * @brief Add custom scheduler
     * @param scheduler Unique pointer to scheduler
     */
    void addScheduler(std::unique_ptr<Scheduler> scheduler);

    /**
     * @brief Set processes for simulation
     * @param processes Vector of processes
     */
    void setProcesses(const std::vector<Process>& processes);

    /**
     * @brief Generate random test processes
     * @param count Number of processes
     */
    void generateProcesses(int count);

    /**
     * @brief Load processes from file
     * @param filename Input file path
     * @return true if successful
     */
    bool loadProcessesFromFile(const std::string& filename);

    /**
     * @brief Save processes to file
     * @param filename Output file path
     * @return true if successful
     */
    bool saveProcessesToFile(const std::string& filename) const;

    /**
     * @brief Run simulation for all schedulers
     */
    void runAll();

    /**
     * @brief Run simulation for specific scheduler
     * @param type Scheduler type to run
     */
    void run(SchedulerType type);

    /**
     * @brief Run comparison between all schedulers
     */
    void runComparison();

    /**
     * @brief Get results from all schedulers
     * @return Vector of metrics
     */
    std::vector<Metrics> getResults() const { return results; }

    /**
     * @brief Export results to CSV
     * @param filename Output filename
     */
    void exportResults(const std::string& filename) const;

    /**
     * @brief Display interactive menu
     */
    void interactiveMenu();

    /**
     * @brief Run benchmark with various workloads
     * @param iterations Number of iterations per test
     */
    void runBenchmark(int iterations = 5);

    /**
     * @brief Set simulation configuration
     * @param config New configuration
     */
    void setSimulationConfig(const SimulationConfig& config) { simConfig = config; }

    /**
     * @brief Set scheduler configuration
     * @param config New configuration
     */
    void setSchedulerConfig(const SchedulerConfig& config) { schedConfig = config; }

    /**
     * @brief Get list of scheduler names
     * @return Vector of names
     */
    std::vector<std::string> getSchedulerNames() const;

    /**
     * @brief Clear all schedulers and results
     */
    void reset();

    /**
     * @brief Print simulation summary
     */
    void printSummary() const;
};

#endif // SIMULATOR_H
