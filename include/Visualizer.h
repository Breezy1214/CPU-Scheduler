/**
 * @file Visualizer.h
 * @brief Text-based visualization for CPU scheduler
 * @author CPU Scheduler Team
 * @version 1.0
 */

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Scheduler.h"
#include "Process.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>

/**
 * @class Visualizer
 * @brief Provides text-based real-time visualization of scheduling
 * 
 * Displays Gantt charts, ready queue status, CPU utilization,
 * and process state transitions in the terminal.
 */
class Visualizer {
private:
    int width;                          ///< Display width in characters
    bool colorEnabled;                  ///< Use ANSI colors
    bool animationEnabled;              ///< Enable animation delays
    int animationDelay;                 ///< Delay between frames (ms)

    // ANSI color codes
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string BOLD;

    /**
     * @brief Get color for process
     * @param pid Process ID
     * @return ANSI color code
     */
    std::string getProcessColor(int pid) const;

    /**
     * @brief Clear console screen
     */
    void clearScreen() const;

    /**
     * @brief Move cursor to position
     * @param row Row number
     * @param col Column number
     */
    void moveCursor(int row, int col) const;

    /**
     * @brief Draw a horizontal line
     * @param length Line length
     * @param ch Character to use
     */
    void drawLine(int length, char ch = '-') const;

    /**
     * @brief Draw a progress bar
     * @param progress Value between 0 and 1
     * @param width Bar width
     */
    void drawProgressBar(double progress, int width) const;

public:
    /**
     * @brief Constructor
     * @param width Display width
     * @param useColors Enable ANSI colors
     */
    explicit Visualizer(int width = 80, bool useColors = true);

    /**
     * @brief Destructor
     */
    ~Visualizer() = default;

    /**
     * @brief Display Gantt chart
     * @param timeline Execution events
     */
    void displayGanttChart(const std::vector<ExecutionEvent>& timeline) const;

    /**
     * @brief Display compact Gantt chart
     * @param timeline Execution events
     * @param maxWidth Maximum width
     */
    void displayCompactGanttChart(const std::vector<ExecutionEvent>& timeline, 
                                   int maxWidth = 60) const;

    /**
     * @brief Display ready queue status
     * @param readyQueue Processes in ready queue
     */
    void displayReadyQueue(const std::vector<Process>& readyQueue) const;

    /**
     * @brief Display current CPU status
     * @param currentProcess Current running process
     * @param currentTime Current simulation time
     */
    void displayCPUStatus(const Process* currentProcess, int currentTime) const;

    /**
     * @brief Display CPU utilization bar
     * @param utilization CPU utilization (0-100)
     */
    void displayCPUUtilization(double utilization) const;

    /**
     * @brief Display all process states
     * @param processes All processes
     */
    void displayProcessTable(const std::vector<Process>& processes) const;

    /**
     * @brief Display process state transition
     * @param process Process that changed state
     * @param fromState Previous state
     * @param toState New state
     * @param time Time of transition
     */
    void displayStateTransition(const Process& process, 
                                 ProcessState fromState,
                                 ProcessState toState, 
                                 int time) const;

    /**
     * @brief Display metrics summary
     * @param metrics Performance metrics
     */
    void displayMetrics(const Metrics& metrics) const;

    /**
     * @brief Display comparison of multiple schedulers
     * @param schedulerNames Names of schedulers
     * @param allMetrics Metrics for each scheduler
     */
    void displayComparison(const std::vector<std::string>& schedulerNames,
                           const std::vector<Metrics>& allMetrics) const;

    /**
     * @brief Display real-time simulation frame
     * @param scheduler Current scheduler state
     * @param time Current time
     */
    void displaySimulationFrame(const Scheduler& scheduler, int time) const;

    /**
     * @brief Display header with title
     * @param title Title text
     */
    void displayHeader(const std::string& title) const;

    /**
     * @brief Display footer
     */
    void displayFooter() const;

    /**
     * @brief Enable/disable colors
     * @param enabled Enable flag
     */
    void setColorEnabled(bool enabled) { colorEnabled = enabled; }

    /**
     * @brief Enable/disable animation
     * @param enabled Enable flag
     */
    void setAnimationEnabled(bool enabled) { animationEnabled = enabled; }

    /**
     * @brief Set animation delay
     * @param ms Delay in milliseconds
     */
    void setAnimationDelay(int ms) { animationDelay = ms; }

    /**
     * @brief Print a separator line
     */
    void printSeparator() const;

    /**
     * @brief Wait for specified time (animation)
     * @param ms Milliseconds to wait
     */
    void wait(int ms) const;
};

#endif // VISUALIZER_H
