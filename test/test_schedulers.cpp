/**
 * @file test_schedulers.cpp
 * @brief Unit tests for scheduling algorithms
 */

#include <gtest/gtest.h>
#include "RoundRobinScheduler.h"
#include "PriorityScheduler.h"
#include "MultilevelQueueScheduler.h"
#include "MultilevelFeedbackQueueScheduler.h"

class SchedulerTest : public ::testing::Test {
protected:
    std::vector<Process> createTestProcesses() {
        std::vector<Process> procs;
        procs.emplace_back(1, 2, 10, 0, "P1");
        procs.emplace_back(2, 1, 5, 1, "P2");
        procs.emplace_back(3, 3, 8, 2, "P3");
        return procs;
    }
};

// Round Robin Tests
TEST_F(SchedulerTest, RoundRobinBasicExecution) {
    RoundRobinScheduler scheduler(4);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getProcessCount(), 0);
    EXPECT_GT(metrics.getTotalExecutionTime(), 0);
    EXPECT_GE(metrics.getAvgWaitingTime(), 0.0);
}

TEST_F(SchedulerTest, RoundRobinTimeQuantum) {
    RoundRobinScheduler scheduler(2);
    EXPECT_EQ(scheduler.getName(), "Round Robin (Q=2)");
}

TEST_F(SchedulerTest, RoundRobinContextSwitches) {
    RoundRobinScheduler scheduler(2);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getTotalContextSwitches(), 0);
}

// Priority Scheduler Tests
TEST_F(SchedulerTest, PriorityPreemptiveExecution) {
    PriorityScheduler scheduler(true);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    EXPECT_TRUE(scheduler.isPreemptive());
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getProcessCount(), 0);
}

TEST_F(SchedulerTest, PriorityNonPreemptiveExecution) {
    PriorityScheduler scheduler(false);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    EXPECT_FALSE(scheduler.isPreemptive());
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getProcessCount(), 0);
}

TEST_F(SchedulerTest, PrioritySchedulerOrdering) {
    PriorityScheduler scheduler(false);
    
    // Add processes with different priorities
    scheduler.addProcess(Process(1, 5, 10, 0, "Low"));
    scheduler.addProcess(Process(2, 1, 5, 0, "High"));
    scheduler.addProcess(Process(3, 3, 8, 0, "Med"));
    
    scheduler.run();
    
    // High priority (1) should execute first
    auto timeline = scheduler.getTimeline();
    EXPECT_FALSE(timeline.empty());
    // First execution should be process with priority 1 (PID 2)
    EXPECT_EQ(timeline[0].processId, 2);
}

// Multilevel Queue Tests
TEST_F(SchedulerTest, MultilevelQueueExecution) {
    MultilevelQueueScheduler scheduler(3);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getProcessCount(), 0);
}

TEST_F(SchedulerTest, MultilevelQueueAssignment) {
    MultilevelQueueScheduler scheduler(3);
    
    // Different priority processes should go to different queues
    scheduler.addProcess(Process(1, 1, 10, 0));  // System queue
    scheduler.addProcess(Process(2, 4, 10, 0));  // Interactive queue
    scheduler.addProcess(Process(3, 8, 10, 0));  // Batch queue
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 3);
}

// Multilevel Feedback Queue Tests
TEST_F(SchedulerTest, MultilevelFeedbackQueueExecution) {
    MultilevelFeedbackQueueScheduler scheduler(3);
    auto procs = createTestProcesses();
    
    for (const auto& p : procs) {
        scheduler.addProcess(p);
    }
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getProcessCount(), 0);
}

TEST_F(SchedulerTest, MultilevelFeedbackQueueDemotion) {
    SchedulerConfig config;
    config.timeQuantum = 2;
    MultilevelFeedbackQueueScheduler scheduler(3, config);
    
    // Process with long burst should be demoted
    scheduler.addProcess(Process(1, 0, 20, 0));
    
    scheduler.run();
    
    // Process should have moved through queues
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getTotalContextSwitches(), 0);
}

// Metrics Tests
TEST_F(SchedulerTest, MetricsCalculation) {
    RoundRobinScheduler scheduler(4);
    scheduler.addProcess(Process(1, 0, 5, 0));
    scheduler.addProcess(Process(2, 0, 3, 0));
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 2);
    EXPECT_GT(metrics.getAvgWaitingTime(), 0.0);
    EXPECT_GT(metrics.getAvgTurnaroundTime(), 0.0);
    EXPECT_GE(metrics.getAvgResponseTime(), 0.0);
    EXPECT_GT(metrics.getCpuUtilization(), 0.0);
    EXPECT_LE(metrics.getCpuUtilization(), 100.0);
}

// Timeline Tests
TEST_F(SchedulerTest, TimelineGeneration) {
    RoundRobinScheduler scheduler(4);
    scheduler.addProcess(Process(1, 0, 8, 0));
    scheduler.addProcess(Process(2, 0, 4, 0));
    
    scheduler.run();
    
    auto timeline = scheduler.getTimeline();
    EXPECT_FALSE(timeline.empty());
    
    // Timeline should be sequential
    for (size_t i = 1; i < timeline.size(); ++i) {
        EXPECT_GE(timeline[i].startTime, timeline[i-1].endTime);
    }
}

// Context Switch Tests
TEST_F(SchedulerTest, ContextSwitchOverhead) {
    SchedulerConfig config;
    config.contextSwitchTime = 2;
    
    RoundRobinScheduler scheduler(3, config);
    scheduler.addProcess(Process(1, 0, 10, 0));
    scheduler.addProcess(Process(2, 0, 10, 0));
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_GT(metrics.getContextSwitchOverhead(), 0);
}

// Starvation Prevention Tests
TEST_F(SchedulerTest, AgingPreventsStarvation) {
    SchedulerConfig config;
    config.agingEnabled = true;
    config.agingThreshold = 5;
    
    PriorityScheduler scheduler(true, config);
    
    // Add high priority process that could cause starvation
    scheduler.addProcess(Process(1, 1, 100, 0));
    scheduler.addProcess(Process(2, 10, 5, 0));  // Low priority
    
    scheduler.run();
    
    // Low priority process should eventually execute
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 2);
}

// Edge Cases
TEST_F(SchedulerTest, EmptyProcessList) {
    RoundRobinScheduler scheduler(4);
    // Don't add any processes
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 0);
}

TEST_F(SchedulerTest, SingleProcess) {
    RoundRobinScheduler scheduler(4);
    scheduler.addProcess(Process(1, 0, 10, 0));
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 1);
    EXPECT_EQ(metrics.getAvgWaitingTime(), 0.0);
}

TEST_F(SchedulerTest, ProcessesWithSameArrivalTime) {
    RoundRobinScheduler scheduler(4);
    scheduler.addProcess(Process(1, 0, 5, 0));
    scheduler.addProcess(Process(2, 0, 5, 0));
    scheduler.addProcess(Process(3, 0, 5, 0));
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 3);
}

TEST_F(SchedulerTest, ProcessesWithDifferentArrivalTimes) {
    RoundRobinScheduler scheduler(4);
    scheduler.addProcess(Process(1, 0, 10, 0));
    scheduler.addProcess(Process(2, 0, 5, 5));
    scheduler.addProcess(Process(3, 0, 8, 10));
    
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    EXPECT_EQ(metrics.getProcessCount(), 3);
    EXPECT_GT(metrics.getTotalExecutionTime(), 10);
}
