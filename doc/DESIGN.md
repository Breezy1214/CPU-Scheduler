# CPU Scheduler Simulator - Design Document

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [System Architecture](#system-architecture)
3. [Core Components](#core-components)
4. [Scheduling Algorithms](#scheduling-algorithms)
5. [Performance Analysis](#performance-analysis)
6. [Implementation Details](#implementation-details)

## 1. Executive Summary

The CPU Scheduler Simulator is a C++ application for simulating CPU scheduling algorithms. It provides educational insights into process management.

Objectives:
- Implement standard CPU scheduling algorithms
- Calculate performance metrics
- Simulate context switching
- Prevent starvation

Technology: C++17, GNU Make, custom testing.

---

## 2. System Architecture

### High-Level Architecture

```
Main Application → Simulator → Schedulers → Metrics
```

### Component Interaction

1. User input to Main
2. Main configures Simulator
3. Simulator distributes processes to Schedulers
4. Schedulers simulate execution
5. Metrics calculated and displayed

### Design Patterns

- Template Method: Scheduler base class
- Strategy: Interchangeable algorithms
- Factory: Scheduler creation

---

## 3. Core Components

### Process Class

Represents a process with attributes: pid, priority, burstTime, remainingTime, arrivalTime, waitingTime, turnaroundTime, responseTime, state, queueLevel.

Operations: execute, setState, incrementWaitingTime, resetExecution.

### Scheduler Base Class

Interface for algorithms: run(), getNextProcess(), getName(), addProcess(), calculateMetrics().

### Metrics Class

Calculates: average waiting time, turnaround time, response time, CPU utilization, throughput, context switches.

### Simulator Class

Manages simulations, handles I/O.

### Visualizer Class

Text-based output: Gantt charts, tables, utilization bars.

---

## 4. Scheduling Algorithms

### Round Robin

Time-sliced preemptive scheduling. Quantum default 4.

Advantages: Fair, no starvation.
Disadvantages: High overhead with small quantum.

### Priority Scheduling

Preemptive and non-preemptive modes. Aging prevents starvation.

### Multilevel Queue

Fixed queues: System (0-2), Interactive (3-5), Batch (6+).
Each queue different policy.

### Multilevel Feedback Queue

Dynamic queues. Processes move based on behavior. Priority boost prevents starvation.
- Preemptive mode checks after each time unit

### 4.3 Multilevel Queue (MLQ)

**Algorithm Description**:
```
1. Create n fixed-priority queues
2. Assign each process to queue based on priority:
   - Queue 0 (System): Priority 0-2
   - Queue 1 (Interactive): Priority 3-5
   - Queue 2+ (Batch): Priority 6+
3. While processes remain:
   a. Select highest priority non-empty queue
   b. Execute process from that queue
   c. Use queue-specific scheduling (RR for each)
   d. Only move to lower queue when current is empty
4. Calculate metrics
```

**Queue Configuration**:
```cpp
Queue 0 (System):    Quantum = 2, Preemptive
Queue 1 (Interactive): Quantum = 4, Preemptive
Queue 2 (Batch):     Quantum = 8, Non-preemptive
```

**Time Complexity**: O(q × n) where q = number of queues  
**Space Complexity**: O(n)

**Key Implementation Details**:
- Fixed queue assignment (no movement)
- Each queue can have different scheduling policy
- Higher priority queues starve lower queues
- Suitable for systems with distinct process classes

### 4.4 Multilevel Feedback Queue (MLFQ)

**Algorithm Description**:
```
1. Create n queues with increasing quantums
2. All processes start in Queue 0 (highest priority)
3. While processes remain:
   a. Select from highest non-empty queue
   b. Execute for queue's quantum
   c. If process used full quantum:
      - Demote to next lower queue
   d. If process didn't use full quantum:
      - Return to same queue
   e. Periodically boost all to Queue 0 (aging)
4. Calculate metrics
```

**Quantum Configuration**:
```
Queue 0: Quantum = 4
Queue 1: Quantum = 8
Queue 2: Quantum = 16
```

**Time Complexity**: O(n log n) average case  
**Space Complexity**: O(n + q) where q = number of queues

**Key Implementation Details**:
- Processes move between queues based on behavior
- CPU-bound processes naturally demote
- I/O-bound processes stay in higher queues
- Priority boost every N time units prevents starvation
- Adaptive to workload characteristics

**Starvation Prevention**:
```cpp
void priorityBoost() {
    if (currentTime - lastBoostTime >= boostInterval) {
        for each process:
            move to Queue 0
        lastBoostTime = currentTime
    }
}
```

---

## 5. Performance Analysis

### Metrics

- Average Waiting Time: Σ(waiting_time) / n
- Average Turnaround Time: Σ(turnaround_time) / n
- Average Response Time: Σ(response_time) / n
- CPU Utilization: (total - idle - cs_time) / total * 100%
- Throughput: processes / time
- Context Switch Overhead: cs_count * cs_time

### Algorithm Comparison

| Metric | RR | Priority P | Priority NP | MLQ | MLFQ |
|--------|----|------------|-------------|-----|------|
| Fairness | Excellent | Good | Good | Fair | Excellent |
| Response | Good | Excellent | Poor | Good | Excellent |
| Overhead | Medium | Low | Low | Medium | High |

where response_time_i = first_execution_time_i - arrival_time_i
```

**4. CPU Utilization**
```
CPU_Util = ((total_time - idle_time - context_switch_time) / total_time) × 100%
```

**5. Throughput**
```
Throughput = number_of_processes / total_execution_time
```

### Algorithm Comparison

| Metric | Round Robin | Priority (P) | Priority (NP) | MLQ | MLFQ |
|--------|-------------|--------------|---------------|-----|------|
| **Fairness** | Excellent | Fair (with aging) | Fair (with aging) | Fair per queue | Excellent |
| **Response Time** | Good | Excellent | Poor | Good | Excellent |
| **Turnaround** | Fair | Good | Good | Good | Very Good |
| **Starvation Risk** | None | Low (aging) | Low (aging) | Medium | Very Low |
| **Overhead** | Medium | Low-Medium | Low | Medium | Medium-High |
| **Best Use Case** | Time-sharing | Real-time | Batch | Fixed priorities | General purpose |

### Context Switch Analysis

**Impact of Context Switch Time**:
```
Actual_Execution_Time = Useful_CPU_Time + (CS_Count × CS_Time)

Overhead_Percentage = (CS_Count × CS_Time / Total_Time) × 100%
```

**Example**:
- 10 processes, 100 total time units
- 25 context switches × 1 time unit each
- Overhead = 25%

**Optimization Strategies**:
1. Increase time quantum (reduces switches)
2. Minimize unnecessary preemption
3. Use non-preemptive algorithms for batch jobs
4. Implement gang scheduling for related processes

---

## 6. Implementation Details

### Process State Machine

NEW → READY → RUNNING → TERMINATED
With waiting and preemption.

### Memory Management

Smart pointers for ownership, vectors for efficiency.
