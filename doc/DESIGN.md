# CPU Scheduler Simulator - Design Document

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [System Architecture](#system-architecture)
3. [Core Components](#core-components)
4. [Scheduling Algorithms](#scheduling-algorithms)
5. [Performance Analysis](#performance-analysis)
6. [Implementation Details](#implementation-details)
7. [Testing Strategy](#testing-strategy)
8. [Future Enhancements](#future-enhancements)

---

## 1. Executive Summary

### Project Overview

The CPU Scheduler Simulator is a comprehensive C++ application designed to simulate and compare various CPU scheduling algorithms. It provides educational insights into operating system process management through realistic simulation, detailed metrics, and visual representations.

### Objectives

1. Implement industry-standard CPU scheduling algorithms
2. Provide accurate performance metrics for algorithm comparison
3. Simulate realistic operating system behavior including context switching
4. Prevent starvation through aging mechanisms
5. Offer both interactive and programmatic interfaces

### Technology Stack

- **Language**: C++17
- **Build System**: GNU Make
- **Testing Framework**: Custom test framework
- **Documentation**: Doxygen, Markdown
- **Version Control**: Git

---

## 2. System Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Main Application                      │
│                     (main.cpp)                           │
└───────────────────┬─────────────────────────────────────┘
                    │
        ┌───────────┴───────────┐
        │                       │
   ┌────▼─────┐         ┌──────▼──────┐
   │Simulator │         │ Visualizer  │
   │          │◄────────┤             │
   └────┬─────┘         └─────────────┘
        │
   ┌────▼──────────────────────────────┐
   │      Scheduler (Abstract Base)     │
   └────┬──────────────────────────────┘
        │
   ┌────┴──────────────┬──────────────┬──────────────┬─────────────┐
   │                   │              │              │             │
┌──▼──┐         ┌─────▼────┐  ┌─────▼────┐  ┌──────▼────┐ ┌────▼─────┐
│ RR  │         │Priority  │  │   MLQ    │  │   MLFQ    │ │Process   │
└─────┘         └──────────┘  └──────────┘  └───────────┘ └──────────┘
                                                            │Metrics   │
                                                            └──────────┘
```

### Component Interaction Flow

1. **User Input** → Main Application
2. **Main** → Simulator (configuration)
3. **Simulator** → Scheduler instances (process distribution)
4. **Schedulers** → Process execution simulation
5. **Schedulers** → Metrics calculation
6. **Visualizer** → Results display
7. **Simulator** → Results export (optional)

### Design Patterns

**1. Template Method Pattern**
- `Scheduler` base class defines algorithm skeleton
- Derived classes implement specific `run()` behavior

**2. Strategy Pattern**
- Different scheduling algorithms as interchangeable strategies
- Simulator can switch between algorithms dynamically

**3. Observer Pattern** (implicit)
- Visualizer observes scheduler state for real-time display

**4. Factory Pattern** (in Simulator)
- `addScheduler(SchedulerType)` creates appropriate scheduler instances

---

## 3. Core Components

### 3.1 Process Class

**Responsibility**: Represent a single process with all associated state and metrics.

**Key Attributes**:
```cpp
- pid: Process ID
- priority: Priority level (0-10, lower = higher priority)
- burstTime: Total CPU time required
- remainingTime: CPU time still needed
- arrivalTime: Time when process enters system
- waitingTime: Total time spent waiting
- turnaroundTime: Total time from arrival to completion
- responseTime: Time to first execution
- state: Current process state (NEW, READY, RUNNING, WAITING, TERMINATED)
- queueLevel: Current queue in multilevel systems
```

**Key Operations**:
- `execute(int time)`: Simulate CPU execution
- `setState(ProcessState)`: Update process state
- `incrementWaitingTime(int)`: Track waiting duration
- `resetExecution()`: Reset for re-simulation

### 3.2 Scheduler Base Class

**Responsibility**: Define common interface and functionality for all scheduling algorithms.

**Key Methods**:
```cpp
virtual void run() = 0;                    // Execute scheduling algorithm
virtual Process* getNextProcess();         // Select next process
virtual std::string getName() const = 0;   // Algorithm name
void addProcess(const Process&);           // Add process to scheduler
void calculateMetrics();                   // Compute performance metrics
const Metrics& getMetrics() const;         // Get results
```

**Common Functionality**:
- Process list management
- Timeline event recording
- Context switch simulation
- Metrics calculation

### 3.3 Metrics Class

**Responsibility**: Calculate and store performance statistics.

**Tracked Metrics**:
- Average Waiting Time
- Average Turnaround Time
- Average Response Time
- CPU Utilization
- Throughput
- Context Switch Count
- Context Switch Overhead

**Calculation Methods**:
```cpp
void calculateAverages();                          // Compute averages
void calculateUtilization(int total, int idle);    // CPU efficiency
void calculateThroughput(int totalTime);           // Processes/time
double getWaitingTimeVariance() const;             // Statistical variance
```

### 3.4 Simulator Class

**Responsibility**: Orchestrate simulation execution, manage multiple schedulers, handle I/O.


### 3.5 Visualizer Class

**Responsibility**: Provide text-based visual representations of simulation results.

**Visualization Types**:
- Gantt charts (timeline of execution)
- Process state tables
- Ready queue status
- CPU utilization bars
- Comparison tables
- Real-time simulation frames

**Features**:
- ANSI color support
- Configurable output width
- Animation capabilities
- Responsive formatting

---

## 4. Scheduling Algorithms

### 4.1 Round Robin (RR)

**Algorithm Description**:
```
1. Initialize circular queue with all ready processes
2. While processes remain:
   a. Dequeue next process
   b. Execute for min(quantum, remaining_time)
   c. If process not complete, enqueue at back
   d. Handle new arrivals
   e. Perform context switch
3. Calculate metrics
```

**Time Complexity**: O(n) per time unit, where n = number of processes  
**Space Complexity**: O(n) for process queue

**Key Implementation Details**:
- Circular queue maintains FIFO order
- Quantum configurable (default: 4 time units)
- New arrivals added to queue end
- Context switches occur between different processes

**Advantages**:
- Fair CPU distribution
- No starvation
- Good for time-sharing systems

**Disadvantages**:
- High context switch overhead with small quantum
- Poor performance for processes with short burst times

### 4.2 Priority Scheduling

**Algorithm Description (Preemptive)**:
```
1. Sort processes by priority (and arrival time)
2. While processes remain:
   a. Find highest priority ready process
   b. Execute for 1 time unit
   c. Check for higher priority arrivals
   d. Preempt if necessary
   e. Apply aging to waiting processes
3. Calculate metrics
```

**Algorithm Description (Non-Preemptive)**:
```
1. While processes remain:
   a. Find highest priority ready process
   b. Execute to completion
   c. Handle new arrivals
   d. Apply aging
3. Calculate metrics
```

**Time Complexity**: O(n log n) for sorting, O(n²) worst case for execution  
**Space Complexity**: O(n)

**Aging Mechanism**:
```cpp
void applyAging() {
    for each waiting process:
        if waiting_time >= threshold:
            decrease priority value (boost priority)
            reset waiting time
}
```

**Key Implementation Details**:
- Priority map tracks waiting durations
- Aging threshold configurable (default: 10 time units)
- Tie-breaking uses arrival time (FCFS)
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

### Metric Definitions

**1. Average Waiting Time (AWT)**
```
AWT = Σ(waiting_time_i) / n

where waiting_time_i = turnaround_time_i - burst_time_i
```

**2. Average Turnaround Time (ATAT)**
```
ATAT = Σ(turnaround_time_i) / n

where turnaround_time_i = completion_time_i - arrival_time_i
```

**3. Average Response Time (ART)**
```
ART = Σ(response_time_i) / n

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

### 6.1 Process State Machine

```
[NEW] ──arrival──> [READY] ──dispatch──> [RUNNING]
                      ▲                      │
                      │                      │
                      └──preempt/quantum────┘
                      
[RUNNING] ──complete──> [TERMINATED]
```

### 6.2 Timeline Event Structure

```cpp
struct ExecutionEvent {
    int processId;          // -1 for idle/context switch
    int startTime;          // Event start
    int endTime;            // Event end
    bool isContextSwitch;   // True if CS event
    std::string description; // Human-readable description
};
```

### 6.3 Memory Management

**Smart Pointers**:
- `std::unique_ptr` for scheduler ownership in Simulator
- `std::unique_ptr` for Visualizer instance
- Raw pointers for temporary process references (non-owning)

**Vector Usage**:
- Process lists use `std::vector` for cache efficiency
- Timeline events stored in vector for sequential access

### 6.4 Configuration Management

```cpp
struct SchedulerConfig {
    int timeQuantum = 4;
    int contextSwitchTime = 1;
    int numQueues = 3;
    std::vector<int> quantums;
    bool agingEnabled = true;
    int agingThreshold = 10;
};

struct SimulationConfig {
    bool realTimeVisualization = true;
    bool showGanttChart = true;
    bool showMetrics = true;
    bool compareAlgorithms = false;
    int visualizationDelay = 100;
    bool dynamicArrivals = false;
    int maxSimulationTime = 1000;
};
```

### 6.5 Error Handling

**Strategies**:
1. Input validation in constructors
2. Bounds checking in array access
3. File I/O error checking with informative messages
4. Graceful degradation (colored output fallback)

---

## 7. Testing Strategy

### Unit Tests

**Coverage Areas**:
1. **Process Class**: State transitions, execution, metrics
2. **Schedulers**: Algorithm correctness, edge cases
3. **Metrics**: Calculation accuracy
4. **Simulator**: Process generation, file I/O

**Test Framework**: Custom test framework with assertion macros

**Example Test**:
```cpp
void test_round_robin_context_switches() {
    RoundRobinScheduler scheduler(2);
    scheduler.addProcess(Process(1, 0, 10, 0));
    scheduler.addProcess(Process(2, 0, 10, 0));
    scheduler.run();
    
    const Metrics& metrics = scheduler.getMetrics();
    ASSERT_GT(metrics.getTotalContextSwitches(), 0);
}
```

### Integration Tests

1. **End-to-End Simulation**: Full simulation runs with known inputs
2. **Algorithm Comparison**: Verify relative performance characteristics
3. **File I/O**: Load and save process data
4. **Visualization**: Output format correctness

### Performance Tests

**Benchmark Suite**:
- 5, 10, 20, 50, 100 processes
- Measure execution time
- Verify linear/logarithmic scaling
- Memory usage profiling

### Test Coverage Goals

- Unit Test Coverage: >80%
- Critical Path Coverage: 100%
- Edge Case Coverage: >90%

---

## 8. Future Enhancements

### Planned Features

1. **Additional Algorithms**
   - Shortest Job First (SJF)
   - Shortest Remaining Time First (SRTF)
   - Completely Fair Scheduler (CFS)
   - Earliest Deadline First (EDF)

2. **I/O Simulation**
   - Process blocking for I/O
   - I/O queues
   - Interrupt handling

3. **Multi-Core Support**
   - Parallel execution simulation
   - Load balancing algorithms
   - Core affinity

4. **Advanced Visualization**
   - GUI interface (Qt/GTK)
   - Interactive timeline manipulation
   - Real-time animation

5. **Machine Learning**
   - Predictive burst time estimation
   - Adaptive quantum selection
   - Workload classification

6. **Extended Metrics**
   - Energy consumption modeling
   - Cache miss simulation
   - Memory footprint tracking

### Architecture Evolution

**Phase 1** (Current): Core scheduling algorithms  
**Phase 2**: I/O and resource management  
**Phase 3**: Multi-core and distributed systems  
**Phase 4**: Advanced modeling and ML integration

---

## Appendix A: Class Diagram

```
┌─────────────────────────────────┐
│          Process                │
├─────────────────────────────────┤
│ - pid: int                      │
│ - priority: int                 │
│ - burstTime: int                │
│ - state: ProcessState           │
├─────────────────────────────────┤
│ + execute(int): int             │
│ + setState(ProcessState): void  │
│ + getPriority(): int            │
└─────────────────────────────────┘
                △
                │
                │
┌───────────────┴─────────────────┐
│        Scheduler (abstract)     │
├─────────────────────────────────┤
│ # processes: vector<Process>    │
│ # currentTime: int              │
│ # metrics: Metrics              │
├─────────────────────────────────┤
│ + run(): void = 0               │
│ + addProcess(Process): void     │
│ + getMetrics(): Metrics         │
└─────────────────────────────────┘
                △
                │
     ┌──────────┼──────────┐
     │          │          │
┌────┴───┐  ┌──┴────┐  ┌──┴─────┐
│   RR   │  │Priority│  │  MLFQ  │
└────────┘  └────────┘  └────────┘
```

---

## References

1. Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). *Operating System Concepts* (10th ed.).
2. Tanenbaum, A. S., & Bos, H. (2014). *Modern Operating Systems* (4th ed.).
3. Love, R. (2010). *Linux Kernel Development* (3rd ed.).
4. Completely Fair Scheduler Documentation - Linux Kernel
5. MLFQ Analysis - [UCBerkeley CS162]

---