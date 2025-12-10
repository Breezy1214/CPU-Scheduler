/**
 * @file test_process.cpp
 * @brief Unit tests for Process class
 */

#include "Process.h"
#include <iostream>
#include <stdexcept>

#define ASSERT_EQ(a, b) if ((a) != (b)) throw std::runtime_error("Assertion failed: " #a " != " #b)
#define ASSERT_TRUE(a) if (!(a)) throw std::runtime_error("Assertion failed: " #a " is not true")
#define ASSERT_FALSE(a) if ((a)) throw std::runtime_error("Assertion failed: " #a " is not false")
#define ASSERT_GT(a, b) if ((a) <= (b)) throw std::runtime_error("Assertion failed: " #a " not greater than " #b)
#define ASSERT_GE(a, b) if ((a) < (b)) throw std::runtime_error("Assertion failed: " #a " not greater than or equal to " #b)

void test_default_constructor() {
    Process p;
    ASSERT_EQ(p.getPid(), 0);
    ASSERT_EQ(p.getPriority(), 0);
    ASSERT_EQ(p.getBurstTime(), 0);
    ASSERT_EQ(p.getArrivalTime(), 0);
    ASSERT_EQ(p.getState(), ProcessState::NEW);
}

void test_parameterized_constructor() {
    Process p(1, 5, 10, 2, "TestProcess");
    ASSERT_EQ(p.getPid(), 1);
    ASSERT_EQ(p.getPriority(), 5);
    ASSERT_EQ(p.getBurstTime(), 10);
    ASSERT_EQ(p.getArrivalTime(), 2);
    ASSERT_EQ(p.getRemainingTime(), 10);
    ASSERT_EQ(p.getName(), "TestProcess");
}

void test_execute_reduces_remaining_time() {
    Process p(1, 1, 10, 0);
    int executed = p.execute(3);
    ASSERT_EQ(executed, 3);
    ASSERT_EQ(p.getRemainingTime(), 7);
}

void test_execute_does_not_exceed_remaining_time() {
    Process p(1, 1, 5, 0);
    int executed = p.execute(10);
    ASSERT_EQ(executed, 5);
    ASSERT_EQ(p.getRemainingTime(), 0);
}

void test_state_transitions() {
    Process p(1, 1, 10, 0);
    ASSERT_EQ(p.getState(), ProcessState::NEW);
    
    p.setState(ProcessState::READY);
    ASSERT_EQ(p.getState(), ProcessState::READY);
    
    p.setState(ProcessState::RUNNING);
    ASSERT_EQ(p.getState(), ProcessState::RUNNING);
    
    p.setState(ProcessState::TERMINATED);
    ASSERT_EQ(p.getState(), ProcessState::TERMINATED);
}

void test_response_time_tracking() {
    Process p(1, 1, 10, 0);
    ASSERT_EQ(p.getResponseTime(), -1);  // Not yet started
    
    p.setResponseTime(5);
    ASSERT_EQ(p.getResponseTime(), 5);
}

void test_waiting_time_increment() {
    Process p(1, 1, 10, 0);
    ASSERT_EQ(p.getWaitingTime(), 0);
    
    p.incrementWaitingTime(3);
    ASSERT_EQ(p.getWaitingTime(), 3);
    
    p.incrementWaitingTime(2);
    ASSERT_EQ(p.getWaitingTime(), 5);
}

void test_completion_check() {
    Process p(1, 1, 5, 0);
    ASSERT_FALSE(p.isCompleted());
    
    p.execute(5);
    ASSERT_EQ(p.getRemainingTime(), 0);
}

void test_queue_level_management() {
    Process p(1, 1, 10, 0);
    ASSERT_EQ(p.getQueueLevel(), 0);
    
    p.setQueueLevel(2);
    ASSERT_EQ(p.getQueueLevel(), 2);
}

void test_priority_modification() {
    Process p(1, 5, 10, 0);
    ASSERT_EQ(p.getPriority(), 5);
    
    p.setPriority(3);
    ASSERT_EQ(p.getPriority(), 3);
}

void test_reset_functionality() {
    Process p(1, 1, 10, 0);
    p.execute(5);
    p.setState(ProcessState::RUNNING);
    p.setResponseTime(3);
    
    p.resetExecution();
    ASSERT_EQ(p.getRemainingTime(), 10);
    ASSERT_FALSE(p.hasStartedExecution());
}

void test_process() {
    std::cout << "  Testing Process class..." << std::endl;
    test_default_constructor();
    test_parameterized_constructor();
    test_execute_reduces_remaining_time();
    test_execute_does_not_exceed_remaining_time();
    test_state_transitions();
    test_response_time_tracking();
    test_waiting_time_increment();
    test_completion_check();
    test_queue_level_management();
    test_priority_modification();
    test_reset_functionality();
}
