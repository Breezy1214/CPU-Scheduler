/**
 * @file Simulator.cpp
 * @brief Implementation of the Simulator class
 * @version 1.0
 */

#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

Simulator::Simulator()
    : rng(std::random_device{}())
{
    visualizer = std::make_unique<Visualizer>();
}

void Simulator::initialize(const SimulationConfig& simCfg, 
                           const SchedulerConfig& schedCfg) {
    simConfig = simCfg;
    schedConfig = schedCfg;
}

std::vector<Process> Simulator::generateRandomProcesses(int count, int maxBurst,
                                                         int maxArrival, int maxPriority) {
    std::vector<Process> procs;
    std::uniform_int_distribution<> burstDist(1, maxBurst);
    std::uniform_int_distribution<> arrivalDist(0, maxArrival);
    std::uniform_int_distribution<> priorityDist(0, maxPriority);
    
    for (int i = 0; i < count; ++i) {
        int burst = burstDist(rng);
        int arrival = arrivalDist(rng);
        int priority = priorityDist(rng);
        
        procs.emplace_back(i, priority, burst, arrival, "P" + std::to_string(i));
    }
    
    return procs;
}

Process* Simulator::dynamicArrivalCallback(int currentTime) {
    // Simple probability-based arrival
    std::uniform_real_distribution<> prob(0.0, 1.0);
    if (prob(rng) < 0.1) {  // 10% chance of new process
        static int nextPid = 100;
        std::uniform_int_distribution<> burstDist(5, 20);
        std::uniform_int_distribution<> priorityDist(0, 10);
        
        int burst = burstDist(rng);
        int priority = priorityDist(rng);
        
        return new Process(nextPid++, priority, burst, currentTime);
    }
    return nullptr;
}

void Simulator::addScheduler(SchedulerType type) {
    std::unique_ptr<Scheduler> scheduler;
    
    switch (type) {
        case SchedulerType::ROUND_ROBIN:
            scheduler = std::make_unique<RoundRobinScheduler>(
                schedConfig.timeQuantum, schedConfig);
            break;
        case SchedulerType::PRIORITY_PREEMPTIVE:
            scheduler = std::make_unique<PriorityScheduler>(true, schedConfig);
            break;
        case SchedulerType::PRIORITY_NON_PREEMPTIVE:
            scheduler = std::make_unique<PriorityScheduler>(false, schedConfig);
            break;
        case SchedulerType::MULTILEVEL_QUEUE:
            scheduler = std::make_unique<MultilevelQueueScheduler>(
                schedConfig.numQueues, schedConfig);
            break;
        case SchedulerType::MULTILEVEL_FEEDBACK_QUEUE:
            scheduler = std::make_unique<MultilevelFeedbackQueueScheduler>(
                schedConfig.numQueues, schedConfig);
            break;
    }
    
    if (scheduler) {
        schedulers.push_back(std::move(scheduler));
    }
}

void Simulator::addScheduler(std::unique_ptr<Scheduler> scheduler) {
    if (scheduler) {
        schedulers.push_back(std::move(scheduler));
    }
}

void Simulator::setProcesses(const std::vector<Process>& processes) {
    baseProcesses = processes;
}

void Simulator::generateProcesses(int count) {
    baseProcesses = generateRandomProcesses(count, 20, 10, 10);
}

bool Simulator::loadProcessesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    baseProcesses.clear();
    std::string line;
    
    // Skip header if present
    std::getline(file, line);
    if (line.find("PID") == std::string::npos) {
        file.seekg(0);  // Reset if no header
    }
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int pid, priority, burst, arrival;
        
        if (iss >> pid >> priority >> burst >> arrival) {
            baseProcesses.emplace_back(pid, priority, burst, arrival);
        }
    }
    
    file.close();
    return !baseProcesses.empty();
}

bool Simulator::saveProcessesToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot write to file " << filename << std::endl;
        return false;
    }
    
    file << "PID Priority BurstTime ArrivalTime\n";
    for (const auto& p : baseProcesses) {
        file << p.getPid() << " " << p.getPriority() << " " 
             << p.getBurstTime() << " " << p.getArrivalTime() << "\n";
    }
    
    file.close();
    return true;
}

void Simulator::runAll() {
    if (baseProcesses.empty()) {
        std::cerr << "Error: No processes to simulate\n";
        return;
    }
    
    results.clear();
    
    for (auto& scheduler : schedulers) {
        std::cout << "\n";
        visualizer->displayHeader(scheduler->getName());
        
        // Reset scheduler and add processes
        scheduler->reset();
        for (const auto& p : baseProcesses) {
            scheduler->addProcess(p);
        }
        
        // Run simulation
        scheduler->run();
        
        // Display results
        if (simConfig.showGanttChart) {
            visualizer->displayGanttChart(scheduler->getTimeline());
        }
        
        if (simConfig.showMetrics) {
            visualizer->displayMetrics(scheduler->getMetrics());
        }
        
        results.push_back(scheduler->getMetrics());
        visualizer->displayFooter();
    }
}

void Simulator::run(SchedulerType type) {
    addScheduler(type);
    if (!schedulers.empty()) {
        auto& scheduler = schedulers.back();
        scheduler->reset();
        for (const auto& p : baseProcesses) {
            scheduler->addProcess(p);
        }
        scheduler->run();
        
        visualizer->displayHeader(scheduler->getName());
        if (simConfig.showGanttChart) {
            visualizer->displayGanttChart(scheduler->getTimeline());
        }
        if (simConfig.showMetrics) {
            visualizer->displayMetrics(scheduler->getMetrics());
        }
        visualizer->displayFooter();
    }
}

void Simulator::runComparison() {
    if (schedulers.empty()) {
        // Add all scheduler types for comparison
        addScheduler(SchedulerType::ROUND_ROBIN);
        addScheduler(SchedulerType::PRIORITY_PREEMPTIVE);
        addScheduler(SchedulerType::PRIORITY_NON_PREEMPTIVE);
        addScheduler(SchedulerType::MULTILEVEL_QUEUE);
        addScheduler(SchedulerType::MULTILEVEL_FEEDBACK_QUEUE);
    }
    
    runAll();
    
    // Display comparison
    std::vector<std::string> names = getSchedulerNames();
    visualizer->displayComparison(names, results);
}

void Simulator::exportResults(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot write to " << filename << std::endl;
        return;
    }
    
    file << "Algorithm,AvgWaitTime,AvgTurnaroundTime,AvgResponseTime,"
         << "CPUUtilization,Throughput,ContextSwitches\n";
    
    for (size_t i = 0; i < schedulers.size() && i < results.size(); ++i) {
        const auto& metrics = results[i];
        file << schedulers[i]->getName() << ","
             << metrics.getAvgWaitingTime() << ","
             << metrics.getAvgTurnaroundTime() << ","
             << metrics.getAvgResponseTime() << ","
             << metrics.getCpuUtilization() << ","
             << metrics.getThroughput() << ","
             << metrics.getTotalContextSwitches() << "\n";
    }
    
    file.close();
    std::cout << "Results exported to " << filename << std::endl;
}

void Simulator::interactiveMenu() {
    int choice;
    
    while (true) {
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║        CPU SCHEDULER SIMULATOR - INTERACTIVE MENU            ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  1. Generate Random Processes                                ║\n";
        std::cout << "║  2. Load Processes from File                                 ║\n";
        std::cout << "║  3. Add Processes Manually                                   ║\n";
        std::cout << "║  4. Display Current Processes                                ║\n";
        std::cout << "║  5. Run Round Robin Scheduler                                ║\n";
        std::cout << "║  6. Run Priority Scheduler (Preemptive)                      ║\n";
        std::cout << "║  7. Run Priority Scheduler (Non-Preemptive)                  ║\n";
        std::cout << "║  8. Run Multilevel Queue Scheduler                           ║\n";
        std::cout << "║  9. Run Multilevel Feedback Queue Scheduler                  ║\n";
        std::cout << "║ 10. Run All Schedulers (Comparison)                          ║\n";
        std::cout << "║ 11. Configure Scheduler Parameters                           ║\n";
        std::cout << "║ 12. Export Results to CSV                                    ║\n";
        std::cout << "║ 13. Run Performance Benchmark                                ║\n";
        std::cout << "║  0. Exit                                                     ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
        std::cout << "Enter choice: ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 0:
                std::cout << "Exiting simulator. Goodbye!\n";
                return;
            case 1: {
                int count;
                std::cout << "Enter number of processes: ";
                std::cin >> count;
                generateProcesses(count);
                std::cout << "Generated " << count << " random processes.\n";
                break;
            }
            case 2: {
                std::string filename;
                std::cout << "Enter filename: ";
                std::cin >> filename;
                if (loadProcessesFromFile(filename)) {
                    std::cout << "Loaded " << baseProcesses.size() << " processes.\n";
                }
                break;
            }
            case 3: {
                int pid, priority, burst, arrival;
                std::cout << "Enter process details (PID Priority Burst Arrival): ";
                std::cin >> pid >> priority >> burst >> arrival;
                baseProcesses.emplace_back(pid, priority, burst, arrival);
                std::cout << "Process added.\n";
                break;
            }
            case 4:
                visualizer->displayProcessTable(baseProcesses);
                break;
            case 5:
                schedulers.clear();
                run(SchedulerType::ROUND_ROBIN);
                break;
            case 6:
                schedulers.clear();
                run(SchedulerType::PRIORITY_PREEMPTIVE);
                break;
            case 7:
                schedulers.clear();
                run(SchedulerType::PRIORITY_NON_PREEMPTIVE);
                break;
            case 8:
                schedulers.clear();
                run(SchedulerType::MULTILEVEL_QUEUE);
                break;
            case 9:
                schedulers.clear();
                run(SchedulerType::MULTILEVEL_FEEDBACK_QUEUE);
                break;
            case 10:
                schedulers.clear();
                runComparison();
                break;
            case 11: {
                std::cout << "Time Quantum: ";
                std::cin >> schedConfig.timeQuantum;
                std::cout << "Context Switch Time: ";
                std::cin >> schedConfig.contextSwitchTime;
                std::cout << "Configuration updated.\n";
                break;
            }
            case 12: {
                std::string filename;
                std::cout << "Enter output filename: ";
                std::cin >> filename;
                exportResults(filename);
                break;
            }
            case 13:
                runBenchmark(5);
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Simulator::runBenchmark(int iterations) {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              PERFORMANCE BENCHMARK                           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    std::vector<int> processCounts = {5, 10, 20, 50, 100};
    
    for (int count : processCounts) {
        std::cout << "\nBenchmarking with " << count << " processes...\n";
        
        generateProcesses(count);
        schedulers.clear();
        results.clear();
        
        addScheduler(SchedulerType::ROUND_ROBIN);
        addScheduler(SchedulerType::PRIORITY_PREEMPTIVE);
        addScheduler(SchedulerType::MULTILEVEL_FEEDBACK_QUEUE);
        
        auto start = std::chrono::high_resolution_clock::now();
        runAll();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start).count();
        
        std::cout << "Completed in " << duration << " ms\n";
    }
}

std::vector<std::string> Simulator::getSchedulerNames() const {
    std::vector<std::string> names;
    for (const auto& scheduler : schedulers) {
        names.push_back(scheduler->getName());
    }
    return names;
}

void Simulator::reset() {
    schedulers.clear();
    results.clear();
    baseProcesses.clear();
}

void Simulator::printSummary() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                  SIMULATION SUMMARY                          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Total Processes:     " << std::setw(10) << baseProcesses.size() 
              << "                              ║\n";
    std::cout << "║ Schedulers Run:      " << std::setw(10) << schedulers.size() 
              << "                              ║\n";
    std::cout << "║ Time Quantum:        " << std::setw(10) << schedConfig.timeQuantum 
              << "                              ║\n";
    std::cout << "║ Context Switch Time: " << std::setw(10) << schedConfig.contextSwitchTime 
              << "                              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}
