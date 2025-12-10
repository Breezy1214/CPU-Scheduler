/**
 * @file test_process.cpp
 * @brief Unit tests for Process class
 */

#include <gtest/gtest.h>
#include "Process.h"

class ProcessTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for tests
    }
};

TEST_F(ProcessTest, DefaultConstructor) {
    Process p;
    EXPECT_EQ(p.getPid(), 0);
    EXPECT_EQ(p.getPriority(), 0);
    EXPECT_EQ(p.getBurstTime(), 0);
    EXPECT_EQ(p.getArrivalTime(), 0);
    EXPECT_EQ(p.getState(), ProcessState::NEW);
}

TEST_F(ProcessTest, ParameterizedConstructor) {
    Process p(1, 5, 10, 2, "TestProcess");
    EXPECT_EQ(p.getPid(), 1);
    EXPECT_EQ(p.getPriority(), 5);
    EXPECT_EQ(p.getBurstTime(), 10);
    EXPECT_EQ(p.getArrivalTime(), 2);
    EXPECT_EQ(p.getRemainingTime(), 10);
    EXPECT_EQ(p.getName(), "TestProcess");
}

TEST_F(ProcessTest, ExecuteReducesRemainingTime) {
    Process p(1, 1, 10, 0);
    int executed = p.execute(3);
    EXPECT_EQ(executed, 3);
    EXPECT_EQ(p.getRemainingTime(), 7);
}

TEST_F(ProcessTest, ExecuteDoesNotExceedRemainingTime) {
    Process p(1, 1, 5, 0);
    int executed = p.execute(10);
    EXPECT_EQ(executed, 5);
    EXPECT_EQ(p.getRemainingTime(), 0);
}

TEST_F(ProcessTest, StateTransitions) {
    Process p(1, 1, 10, 0);
    EXPECT_EQ(p.getState(), ProcessState::NEW);
    
    p.setState(ProcessState::READY);
    EXPECT_EQ(p.getState(), ProcessState::READY);
    
    p.setState(ProcessState::RUNNING);
    EXPECT_EQ(p.getState(), ProcessState::RUNNING);
    
    p.setState(ProcessState::TERMINATED);
    EXPECT_EQ(p.getState(), ProcessState::TERMINATED);
}

TEST_F(ProcessTest, ResponseTimeTracking) {
    Process p(1, 1, 10, 0);
    EXPECT_EQ(p.getResponseTime(), -1);  // Not yet started
    
    p.setResponseTime(5);
    EXPECT_EQ(p.getResponseTime(), 5);
}

TEST_F(ProcessTest, WaitingTimeIncrement) {
    Process p(1, 1, 10, 0);
    EXPECT_EQ(p.getWaitingTime(), 0);
    
    p.incrementWaitingTime(3);
    EXPECT_EQ(p.getWaitingTime(), 3);
    
    p.incrementWaitingTime(2);
    EXPECT_EQ(p.getWaitingTime(), 5);
}

TEST_F(ProcessTest, CompletionCheck) {
    Process p(1, 1, 5, 0);
    EXPECT_FALSE(p.isCompleted());
    
    p.execute(5);
    EXPECT_EQ(p.getRemainingTime(), 0);
}

TEST_F(ProcessTest, QueueLevelManagement) {
    Process p(1, 1, 10, 0);
    EXPECT_EQ(p.getQueueLevel(), 0);
    
    p.setQueueLevel(2);
    EXPECT_EQ(p.getQueueLevel(), 2);
}

TEST_F(ProcessTest, PriorityModification) {
    Process p(1, 5, 10, 0);
    EXPECT_EQ(p.getPriority(), 5);
    
    p.setPriority(3);
    EXPECT_EQ(p.getPriority(), 3);
}

TEST_F(ProcessTest, ResetFunctionality) {
    Process p(1, 1, 10, 0);
    p.execute(5);
    p.setState(ProcessState::RUNNING);
    p.setResponseTime(3);
    
    p.resetExecution();
    EXPECT_EQ(p.getRemainingTime(), 10);
    EXPECT_FALSE(p.hasStartedExecution());
}
