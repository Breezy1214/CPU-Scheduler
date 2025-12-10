/**
 * @file Visualizer.cpp
 * @brief Implementation of the Visualizer class
 * @version 1.0
 */

#include "Visualizer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>

#ifdef _WIN32
    #include <windows.h>
#endif

// ANSI color codes
const std::string Visualizer::RESET = "\033[0m";
const std::string Visualizer::RED = "\033[31m";
const std::string Visualizer::GREEN = "\033[32m";
const std::string Visualizer::YELLOW = "\033[33m";
const std::string Visualizer::BLUE = "\033[34m";
const std::string Visualizer::MAGENTA = "\033[35m";
const std::string Visualizer::CYAN = "\033[36m";
const std::string Visualizer::WHITE = "\033[37m";
const std::string Visualizer::BOLD = "\033[1m";

Visualizer::Visualizer(int width, bool useColors)
    : width(width)
    , colorEnabled(useColors)
    , animationEnabled(false)
    , animationDelay(100)
{}

std::string Visualizer::getProcessColor(int pid) const {
    if (!colorEnabled) return "";
    
    const std::string colors[] = {CYAN, GREEN, YELLOW, MAGENTA, BLUE, RED};
    return colors[pid % 6];
}

void Visualizer::clearScreen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Visualizer::moveCursor(int row, int col) const {
    std::cout << "\033[" << row << ";" << col << "H";
}

void Visualizer::drawLine(int length, char ch) const {
    for (int i = 0; i < length; ++i) {
        std::cout << ch;
    }
    std::cout << "\n";
}

void Visualizer::drawProgressBar(double progress, int barWidth) const {
    int filled = static_cast<int>(progress * barWidth);
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filled) {
            std::cout << (colorEnabled ? GREEN : "") << "█" << RESET;
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] " << std::fixed << std::setprecision(1) 
              << (progress * 100) << "%";
}

void Visualizer::displayGanttChart(const std::vector<ExecutionEvent>& timeline) const {
    if (timeline.empty()) {
        std::cout << "No execution timeline to display.\n";
        return;
    }
    
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      GANTT CHART                             ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    
    // Display process blocks
    std::cout << "║ ";
    for (const auto& event : timeline) {
        if (event.processId == -1) {
            std::cout << "[IDLE]";
        } else {
            std::cout << getProcessColor(event.processId) 
                      << "[P" << event.processId << "]" << RESET;
        }
    }
    std::cout << "\n";
    
    // Display timeline
    std::cout << "║ ";
    int currentPos = 0;
    for (const auto& event : timeline) {
        if (currentPos == 0) {
            std::cout << event.startTime;
        }
        
        int duration = event.endTime - event.startTime;
        for (int i = 0; i < duration; ++i) {
            std::cout << "-";
        }
        std::cout << event.endTime;
        currentPos = event.endTime;
    }
    std::cout << "\n";
    
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

void Visualizer::displayCompactGanttChart(const std::vector<ExecutionEvent>& timeline,
                                          int maxWidth) const {
    if (timeline.empty()) return;
    
    int totalTime = timeline.back().endTime;
    double scale = static_cast<double>(maxWidth) / totalTime;
    
    std::cout << "\nCompact Gantt Chart (scaled):\n";
    std::cout << "│";
    
    for (const auto& event : timeline) {
        int width = std::max(1, static_cast<int>((event.endTime - event.startTime) * scale));
        std::string color = (event.processId == -1) ? "" : getProcessColor(event.processId);
        
        for (int i = 0; i < width; ++i) {
            if (event.processId == -1) {
                std::cout << " ";
            } else {
                std::cout << color << "█" << RESET;
            }
        }
    }
    std::cout << "│\n";
    std::cout << "0" << std::setw(maxWidth) << totalTime << "\n";
}

void Visualizer::displayReadyQueue(const std::vector<Process>& readyQueue) const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    READY QUEUE STATUS                        ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    
    if (readyQueue.empty()) {
        std::cout << "║                    (Queue is empty)                          ║\n";
    } else {
        std::cout << "║ Processes: ";
        for (const auto& p : readyQueue) {
            std::cout << getProcessColor(p.getPid()) 
                      << "P" << p.getPid() << RESET << " ";
        }
        std::cout << std::setw(40) << "║\n";
    }
    
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

void Visualizer::displayCPUStatus(const Process* currentProcess, int currentTime) const {
    std::cout << "\n┌────────────────────────────────────────┐\n";
    std::cout << "│         CPU STATUS                     │\n";
    std::cout << "├────────────────────────────────────────┤\n";
    std::cout << "│ Time: " << std::setw(10) << currentTime << std::setw(22) << "│\n";
    
    if (currentProcess) {
        std::cout << "│ Running: " << getProcessColor(currentProcess->getPid())
                  << "P" << currentProcess->getPid() << RESET 
                  << std::setw(26) << "│\n";
        std::cout << "│ Remaining: " << std::setw(5) << currentProcess->getRemainingTime() 
                  << std::setw(21) << "│\n";
    } else {
        std::cout << "│ Running: IDLE" << std::setw(23) << "│\n";
    }
    
    std::cout << "└────────────────────────────────────────┘\n";
}

void Visualizer::displayCPUUtilization(double utilization) const {
    std::cout << "\nCPU Utilization: ";
    drawProgressBar(utilization / 100.0, 40);
    std::cout << "\n";
}

void Visualizer::displayProcessTable(const std::vector<Process>& processes) const {
    std::cout << "\n╔═════╤══════════╤════════╤═══════════╤════════════╤═════════╗\n";
    std::cout << "║ PID │ Priority │ Burst  │  Arrival  │   State    │ Queue   ║\n";
    std::cout << "╠═════╪══════════╪════════╪═══════════╪════════════╪═════════╣\n";
    
    for (const auto& p : processes) {
        std::cout << "║ " << std::setw(3) << p.getPid() 
                  << " │ " << std::setw(8) << p.getPriority()
                  << " │ " << std::setw(6) << p.getBurstTime()
                  << " │ " << std::setw(9) << p.getArrivalTime()
                  << " │ ";
        
        switch (p.getState()) {
            case ProcessState::NEW:
                std::cout << std::setw(10) << "NEW";
                break;
            case ProcessState::READY:
                std::cout << colorEnabled << GREEN << std::setw(10) << "READY" << RESET;
                break;
            case ProcessState::RUNNING:
                std::cout << colorEnabled << CYAN << std::setw(10) << "RUNNING" << RESET;
                break;
            case ProcessState::WAITING:
                std::cout << colorEnabled << YELLOW << std::setw(10) << "WAITING" << RESET;
                break;
            case ProcessState::TERMINATED:
                std::cout << colorEnabled << RED << std::setw(10) << "DONE" << RESET;
                break;
        }
        
        std::cout << " │ " << std::setw(7) << p.getQueueLevel() << " ║\n";
    }
    
    std::cout << "╚═════╧══════════╧════════╧═══════════╧════════════╧═════════╝\n";
}

void Visualizer::displayStateTransition(const Process& process,
                                        ProcessState fromState,
                                        ProcessState toState,
                                        int time) const {
    auto stateToString = [](ProcessState state) -> std::string {
        switch (state) {
            case ProcessState::NEW: return "NEW";
            case ProcessState::READY: return "READY";
            case ProcessState::RUNNING: return "RUNNING";
            case ProcessState::WAITING: return "WAITING";
            case ProcessState::TERMINATED: return "TERMINATED";
        }
        return "UNKNOWN";
    };
    
    std::cout << "[Time " << time << "] " 
              << getProcessColor(process.getPid()) << "P" << process.getPid() << RESET
              << ": " << stateToString(fromState) << " → " << stateToString(toState) 
              << "\n";
}

void Visualizer::displayMetrics(const Metrics& metrics) const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               PERFORMANCE METRICS                            ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Processes:              " << std::setw(10) << metrics.getProcessCount() 
              << "                        ║\n";
    std::cout << "║ Total Execution Time:   " << std::setw(10) << metrics.getTotalExecutionTime() 
              << " time units             ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Avg Waiting Time:       " << std::setw(10) << std::fixed 
              << std::setprecision(2) << metrics.getAvgWaitingTime() 
              << " time units             ║\n";
    std::cout << "║ Avg Turnaround Time:    " << std::setw(10) << std::fixed 
              << std::setprecision(2) << metrics.getAvgTurnaroundTime() 
              << " time units             ║\n";
    std::cout << "║ Avg Response Time:      " << std::setw(10) << std::fixed 
              << std::setprecision(2) << metrics.getAvgResponseTime() 
              << " time units             ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ CPU Utilization:        " << std::setw(10) << std::fixed 
              << std::setprecision(2) << metrics.getCpuUtilization() 
              << " %                      ║\n";
    std::cout << "║ Throughput:             " << std::setw(10) << std::fixed 
              << std::setprecision(4) << metrics.getThroughput() 
              << " proc/time             ║\n";
    std::cout << "║ Context Switches:       " << std::setw(10) << metrics.getTotalContextSwitches() 
              << "                        ║\n";
    std::cout << "║ CS Overhead:            " << std::setw(10) << metrics.getContextSwitchOverhead() 
              << " time units             ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

void Visualizer::displayComparison(const std::vector<std::string>& schedulerNames,
                                   const std::vector<Metrics>& allMetrics) const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    SCHEDULER COMPARISON                                  ║\n";
    std::cout << "╠══════════════════╤═══════╤═══════╤═══════╤══════╤══════╤═══════════════╣\n";
    std::cout << "║ Algorithm        │  WT   │  TAT  │  RT   │ CPU% │  TP  │ Ctx Switches  ║\n";
    std::cout << "╠══════════════════╪═══════╪═══════╪═══════╪══════╪══════╪═══════════════╣\n";
    
    for (size_t i = 0; i < schedulerNames.size() && i < allMetrics.size(); ++i) {
        const auto& m = allMetrics[i];
        std::cout << "║ " << std::left << std::setw(16) << schedulerNames[i].substr(0, 16)
                  << " │ " << std::right << std::setw(5) << std::fixed << std::setprecision(1) 
                  << m.getAvgWaitingTime()
                  << " │ " << std::setw(5) << m.getAvgTurnaroundTime()
                  << " │ " << std::setw(5) << m.getAvgResponseTime()
                  << " │ " << std::setw(4) << std::setprecision(0) << m.getCpuUtilization()
                  << " │ " << std::setw(4) << std::setprecision(2) << m.getThroughput()
                  << " │ " << std::setw(13) << m.getTotalContextSwitches()
                  << " ║\n";
    }
    
    std::cout << "╚══════════════════╧═══════╧═══════╧═══════╧══════╧══════╧═══════════════╝\n";
    
    // Find best performers
    if (!allMetrics.empty()) {
        size_t bestWT = 0, bestTAT = 0, bestRT = 0, bestCPU = 0;
        
        for (size_t i = 1; i < allMetrics.size(); ++i) {
            if (allMetrics[i].getAvgWaitingTime() < allMetrics[bestWT].getAvgWaitingTime())
                bestWT = i;
            if (allMetrics[i].getAvgTurnaroundTime() < allMetrics[bestTAT].getAvgTurnaroundTime())
                bestTAT = i;
            if (allMetrics[i].getAvgResponseTime() < allMetrics[bestRT].getAvgResponseTime())
                bestRT = i;
            if (allMetrics[i].getCpuUtilization() > allMetrics[bestCPU].getCpuUtilization())
                bestCPU = i;
        }
        
        std::cout << "\n" << BOLD << GREEN << "Best Performers:" << RESET << "\n";
        std::cout << "  Lowest Avg Waiting Time:   " << schedulerNames[bestWT] << "\n";
        std::cout << "  Lowest Avg Turnaround:     " << schedulerNames[bestTAT] << "\n";
        std::cout << "  Lowest Avg Response Time:  " << schedulerNames[bestRT] << "\n";
        std::cout << "  Highest CPU Utilization:   " << schedulerNames[bestCPU] << "\n";
    }
}

void Visualizer::displaySimulationFrame(const Scheduler& scheduler, int time) const {
    clearScreen();
    displayHeader("Real-Time Simulation");
    displayCPUStatus(scheduler.getCurrentProcess(), time);
    displayProcessTable(scheduler.getProcesses());
    wait(animationDelay);
}

void Visualizer::displayHeader(const std::string& title) const {
    std::cout << "\n";
    drawLine(width, '=');
    std::cout << std::setw(width/2 + title.length()/2) << title << "\n";
    drawLine(width, '=');
}

void Visualizer::displayFooter() const {
    drawLine(width, '=');
    std::cout << "\n";
}

void Visualizer::printSeparator() const {
    drawLine(width, '-');
}

void Visualizer::wait(int ms) const {
    if (animationEnabled && ms > 0) {
        #ifdef _WIN32
            // Use Windows Sleep on Windows
            #include <windows.h>
            Sleep(ms);
        #else
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        #endif
    }
}
