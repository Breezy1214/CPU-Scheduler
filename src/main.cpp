/**
 * @file main.cpp
 * @brief Main entry point for CPU Scheduler Simulator
 * @version 1.0
 */

#include "Simulator.h"
#include "Process.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

/**
 * @brief Display welcome banner
 */
void displayWelcomeBanner() {
    std::cout << "\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "|                                                                          |\n";
    std::cout << "|              CPU SCHEDULER SIMULATOR - VERSION 1.0                       |\n";
    std::cout << "|                                                                          |\n";
    std::cout << "|       Advanced CPU Scheduling Algorithm Simulation Environment          |\n";
    std::cout << "|                                                                          |\n";
    std::cout << "|  Supported Algorithms:                                                   |\n";
    std::cout << "|    * Round Robin (RR)                                                    |\n";
    std::cout << "|    * Priority Scheduling (Preemptive & Non-Preemptive)                   |\n";
    std::cout << "|    * Multilevel Queue (MLQ)                                              |\n";
    std::cout << "|    * Multilevel Feedback Queue (MLFQ)                                    |\n";
    std::cout << "|                                                                          |\n";
    std::cout << "|  Features:                                                               |\n";
    std::cout << "|    * Real-time visualization                                             |\n";
    std::cout << "|    * Performance metrics analysis                                        |\n";
    std::cout << "|    * Context switching simulation                                        |\n";
    std::cout << "|    * Starvation prevention (aging)                                       |\n";
    std::cout << "|    * Algorithm comparison                                                |\n";
    std::cout << "|                                                                          |\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "\n";
}

/**
 * @brief Display usage information
 */
void displayUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help              Display this help message\n";
    std::cout << "  -i, --interactive       Start interactive menu mode (default)\n";
    std::cout << "  -f, --file <filename>   Load processes from file\n";
    std::cout << "  -n, --num <count>       Generate N random processes\n";
    std::cout << "  -a, --algorithm <algo>  Run specific algorithm:\n";
    std::cout << "                            rr    - Round Robin\n";
    std::cout << "                            pp    - Priority Preemptive\n";
    std::cout << "                            pnp   - Priority Non-Preemptive\n";
    std::cout << "                            mlq   - Multilevel Queue\n";
    std::cout << "                            mlfq  - Multilevel Feedback Queue\n";
    std::cout << "                            all   - Run all and compare\n";
    std::cout << "  -q, --quantum <value>   Set time quantum (default: 4)\n";
    std::cout << "  -c, --context <value>   Set context switch time (default: 1)\n";
    std::cout << "  -b, --benchmark         Run performance benchmark\n";
    std::cout << "  -o, --output <file>     Export results to CSV file\n";
    std::cout << "  --no-color              Disable colored output\n";
    std::cout << "  --no-gantt              Disable Gantt chart display\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " -i\n";
    std::cout << "  " << programName << " -n 10 -a all\n";
    std::cout << "  " << programName << " -f processes.txt -a rr -q 5\n";
    std::cout << "  " << programName << " -b -o results.csv\n";
    std::cout << "\n";
}

/**
 * @brief Create sample process set for demonstration
 */
std::vector<Process> createSampleProcesses() {
    std::vector<Process> processes;
    
    // Sample processes with varied characteristics
    processes.emplace_back(1, 2, 10, 0, "P1");
    processes.emplace_back(2, 1, 5, 1, "P2");
    processes.emplace_back(3, 3, 8, 2, "P3");
    processes.emplace_back(4, 2, 4, 3, "P4");
    processes.emplace_back(5, 4, 6, 4, "P5");
    
    return processes;
}

/**
 * @brief Quick demo mode
 */
void runQuickDemo() {
    std::cout << "\n+--------------------------------------------------------------+\n";
    std::cout << "|                    QUICK DEMO MODE                           |\n";
    std::cout << "+--------------------------------------------------------------+\n";
    std::cout << "| Running comparison of all scheduling algorithms...           |\n";
    std::cout << "+--------------------------------------------------------------+\n";
    
    Simulator sim;
    sim.setProcesses(createSampleProcesses());
    sim.runComparison();
}

/**
 * @brief Parse command line arguments
 */
int main(int argc, char* argv[]) {
    displayWelcomeBanner();
    
    // Default configuration
    SimulationConfig simConfig;
    SchedulerConfig schedConfig;
    
    bool interactiveMode = true;
    bool benchmarkMode = false;
    std::string inputFile;
    std::string outputFile;
    std::string algorithm;
    int numProcesses = 0;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            displayUsage(argv[0]);
            return 0;
        }
        else if (arg == "-i" || arg == "--interactive") {
            interactiveMode = true;
        }
        else if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) {
                inputFile = argv[++i];
                interactiveMode = false;
            }
        }
        else if (arg == "-n" || arg == "--num") {
            if (i + 1 < argc) {
                numProcesses = std::atoi(argv[++i]);
                interactiveMode = false;
            }
        }
        else if (arg == "-a" || arg == "--algorithm") {
            if (i + 1 < argc) {
                algorithm = argv[++i];
                interactiveMode = false;
            }
        }
        else if (arg == "-q" || arg == "--quantum") {
            if (i + 1 < argc) {
                schedConfig.timeQuantum = std::atoi(argv[++i]);
            }
        }
        else if (arg == "-c" || arg == "--context") {
            if (i + 1 < argc) {
                schedConfig.contextSwitchTime = std::atoi(argv[++i]);
            }
        }
        else if (arg == "-b" || arg == "--benchmark") {
            benchmarkMode = true;
            interactiveMode = false;
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            }
        }
        else if (arg == "--no-color") {
            // Color will be handled by visualizer
        }
        else if (arg == "--no-gantt") {
            simConfig.showGanttChart = false;
        }
        else if (arg == "--demo") {
            runQuickDemo();
            return 0;
        }
    }
    
    // Create simulator
    Simulator simulator;
    simulator.initialize(simConfig, schedConfig);
    
    // Run in appropriate mode
    if (interactiveMode) {
        // Interactive menu mode
        simulator.interactiveMenu();
    }
    else if (benchmarkMode) {
        // Benchmark mode
        std::cout << "Starting performance benchmark...\n";
        simulator.runBenchmark(5);
        
        if (!outputFile.empty()) {
            simulator.exportResults(outputFile);
        }
    }
    else {
        // Command-line mode
        
        // Load or generate processes
        if (!inputFile.empty()) {
            std::cout << "Loading processes from " << inputFile << "...\n";
            if (!simulator.loadProcessesFromFile(inputFile)) {
                std::cerr << "Error: Failed to load processes from file.\n";
                return 1;
            }
        }
        else if (numProcesses > 0) {
            std::cout << "Generating " << numProcesses << " random processes...\n";
            simulator.generateProcesses(numProcesses);
        }
        else {
            std::cout << "Using sample process set...\n";
            simulator.setProcesses(createSampleProcesses());
        }
        
        // Run scheduler(s)
        if (algorithm.empty() || algorithm == "all") {
            std::cout << "Running comparison of all algorithms...\n";
            simulator.runComparison();
        }
        else {
            SchedulerType type;
            
            if (algorithm == "rr") {
                type = SchedulerType::ROUND_ROBIN;
            }
            else if (algorithm == "pp") {
                type = SchedulerType::PRIORITY_PREEMPTIVE;
            }
            else if (algorithm == "pnp") {
                type = SchedulerType::PRIORITY_NON_PREEMPTIVE;
            }
            else if (algorithm == "mlq") {
                type = SchedulerType::MULTILEVEL_QUEUE;
            }
            else if (algorithm == "mlfq") {
                type = SchedulerType::MULTILEVEL_FEEDBACK_QUEUE;
            }
            else {
                std::cerr << "Error: Unknown algorithm '" << algorithm << "'\n";
                std::cerr << "Use --help for list of supported algorithms.\n";
                return 1;
            }
            
            std::cout << "Running " << algorithm << " scheduler...\n";
            simulator.run(type);
        }
        
        // Export results if requested
        if (!outputFile.empty()) {
            simulator.exportResults(outputFile);
        }
        
        simulator.printSummary();
    }
    
    std::cout << "\n";
    std::cout << "+--------------------------------------------------------------+\n";
    std::cout << "|                  Simulation Complete!                        |\n";
    std::cout << "|            Thank you for using CPU Scheduler!                |\n";
    std::cout << "+--------------------------------------------------------------+\n";
    std::cout << "\n";
    
    return 0;
}
