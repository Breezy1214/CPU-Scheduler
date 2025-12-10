/**
 * @file test_schedulers.cpp
 * @brief Unit tests for scheduling algorithms
 */

#include "RoundRobinScheduler.h"
#include "PriorityScheduler.h"
#include "MultilevelQueueScheduler.h"
#include "MultilevelFeedbackQueueScheduler.h"
#include <iostream>
#include <stdexcept>
#include <vector>

#define ASSERT_EQ(a, b) if ((a) != (b)) throw std::runtime_error("Assertion failed: " #a " != " #b)
#define ASSERT_TRUE(a) if (!(a)) throw std::runtime_error("Assertion failed: " #a " is not true")
#define ASSERT_FALSE(a) if ((a)) throw std::runtime_error("Assertion failed: " #a " is not false")
#define ASSERT_GT(a, b) if ((a) <= (b)) throw std::runtime_error("Assertion failed: " #a " not greater than " #b)
#define ASSERT_GE(a, b) if ((a) < (b)) throw std::runtime_error("Assertion failed: " #a " not greater than or equal to " #b)
#define ASSERT_LE(a, b) if ((a) > (b)) throw std::runtime_error("Assertion failed: " #a " not less than or equal to " #b)

std::vector<Process> createTestProcesses() {
    std::vector<Process> procs;
    procs.emplace_back(1, 2, 10, 0, "P1");
    procs.emplace_back(2, 1, 5, 1, "P2");
    procs.emplace_back(3, 3, 8, 2, "P3");
    return procs;
}

// Round Robin Tests
void test_round_robin() {
    std::cout << "  Testing Round Robin scheduler..." << std::endl;
    RoundRobinScheduler scheduler(4);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    ASSERT_GT(metrics.getProcessCount(), 0);
    ASSERT_GT(metrics.getTotalExecutionTime(), 0);
    ASSERT_GE(metrics.getAvgWaitingTime(), 0.0);
}

// Priority Scheduler Tests
void test_priority_scheduler() {
    std::cout << "  Testing Priority scheduler..." << std::endl;
    PriorityScheduler scheduler(true);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    ASSERT_TRUE(scheduler.isPreemptive());
    const Metrics& metrics = scheduler.getMetrics();
    ASSERT_GT(metrics.getProcessCount(), 0);
}

// Multilevel Queue Tests
void test_multilevel_queue() {
    std::cout << "  Testing Multilevel Queue scheduler..." << std::endl;
    MultilevelQueueScheduler scheduler(3);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    ASSERT_GT(metrics.getProcessCount(), 0);
}

// Multilevel Feedback Queue Tests
void test_multilevel_feedback_queue() {
    std::cout << "  Testing Multilevel Feedback Queue scheduler..." << std::endl;
    MultilevelFeedbackQueueScheduler scheduler(3);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    ASSERT_GT(metrics.getProcessCount(), 0);
}

// Metrics Tests
void test_metrics() {
    std::cout << "  Testing metrics calculation..." << std::endl;
    RoundRobinScheduler scheduler(4);
    scheduler.addProcess(Process(1, 0, 5, 0));
    scheduler.addProcess(Process(2, 0, 3, 0));
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    ASSERT_EQ(metrics.getProcessCount(), 2);
    ASSERT_GE(metrics.getAvgWaitingTime(), 0.0);
    ASSERT_GE(metrics.getAvgTurnaroundTime(), 0.0);
    ASSERT_GE(metrics.getAvgResponseTime(), 0.0);
    ASSERT_GT(metrics.getCpuUtilization(), 0.0);
    ASSERT_LE(metrics.getCpuUtilization(), 100.0);
}

// Edge Cases
void test_edge_cases() {
    std::cout << "  Testing edge cases..." << std::endl;
    
    // Empty process list
    RoundRobinScheduler scheduler1(4);
    scheduler1.run();
    ASSERT_EQ(scheduler1.getMetrics().getProcessCount(), 0);
    
    // Single process
    RoundRobinScheduler scheduler2(4);
    scheduler2.addProcess(Process(1, 0, 10, 0));
    scheduler2.run();
    ASSERT_EQ(scheduler2.getMetrics().getProcessCount(), 1);
    ASSERT_EQ(scheduler2.getMetrics().getAvgWaitingTime(), 0.0);
}

void test_schedulers() {
    test_round_robin();
    test_priority_scheduler();
    test_multilevel_queue();
    test_multilevel_feedback_queue();
    test_metrics();
    test_edge_cases();
}
