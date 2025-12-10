# CPU Scheduler Simulator - User Manual

---

## Table of Contents

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Getting Started](#getting-started)
4. [Scheduling Algorithms](#scheduling-algorithms)
5. [Using the Simulator](#using-the-simulator)
6. [Understanding Results](#understanding-results)
7. [Advanced Features](#advanced-features)
8. [Troubleshooting](#troubleshooting)
9. [FAQ](#faq)

---

## 1. Introduction

The CPU Scheduler Simulator is a comprehensive educational tool designed to help understand how different CPU scheduling algorithms work. It simulates process execution, calculates performance metrics, and provides visual representations of scheduling behavior.

### Supported Algorithms

- **Round Robin (RR)**: Time-sliced preemptive scheduling
- **Priority Scheduling**: Both preemptive and non-preemptive modes
- **Multilevel Queue (MLQ)**: Fixed queue assignment based on priority
- **Multilevel Feedback Queue (MLFQ)**: Dynamic queue movement based on behavior

### Key Features

- Real-time visualization of process execution
- Comprehensive performance metrics
- Context switching simulation with overhead
- Starvation prevention through aging
- Side-by-side algorithm comparison
- Process import/export capabilities
- Performance benchmarking tools

---

## 2. Installation

### Prerequisites

- C++ compiler with C++17 support (g++, clang++)
- Make build system
- Google Test library (for running tests)

### Building from Source

```bash
# Clone or download the repository
cd CPU-Scheduler

# Build the project
make build

# The executable will be in bin/scheduler
./bin/scheduler
```

### Installation (Optional)

To install the simulator system-wide:

```bash
make install
```

This installs the executable to `~/bin/scheduler`.

---

## 3. Getting Started

### Quick Start

Run the simulator in interactive mode:

```bash
./bin/scheduler
```

This launches an interactive menu where you can:
1. Generate or load processes
2. Select scheduling algorithms
3. View results and metrics

### Command-Line Mode

For automated runs:

```bash
# Run comparison with 10 random processes
./bin/scheduler -n 10 -a all

# Run Round Robin with custom quantum
./bin/scheduler -n 5 -a rr -q 8

# Load processes from file
./bin/scheduler -f processes.txt -a pp
```

### Quick Demo

To see a quick demonstration:

```bash
./bin/scheduler --demo
```

---

## 4. Scheduling Algorithms

### Round Robin (RR)

**How it works:**
- Each process gets a fixed time quantum
- Processes are executed in circular queue order
- Preemptive: processes are interrupted after quantum expires

**Best for:**
- Time-sharing systems
- Fair CPU distribution among processes
- Interactive applications

**Configuration:**
- Time Quantum (default: 4)
- Context Switch Time (default: 1)

### Priority Scheduling

**How it works:**
- Processes are selected based on priority value (lower = higher priority)
- Two modes:
  - **Preemptive**: Higher priority processes interrupt lower priority
  - **Non-Preemptive**: Process runs to completion once started

**Best for:**
- Systems with different process importance levels
- Real-time systems requiring guaranteed response

**Features:**
- Aging mechanism prevents starvation
- Configurable aging threshold

### Multilevel Queue (MLQ)

**How it works:**
- Processes are permanently assigned to queues based on priority
- Three queue levels:
  - System Queue (highest priority)
  - Interactive Queue (medium priority)
  - Batch Queue (lowest priority)
- Higher priority queues must be empty before lower queues execute

**Best for:**
- Systems with distinct process types
- Prioritizing system and interactive processes

### Multilevel Feedback Queue (MLFQ)

**How it works:**
- Processes start in highest priority queue
- Processes move down if they use entire quantum (CPU-bound)
- Periodic priority boosts prevent starvation
- Each queue has increasing time quantum

**Best for:**
- General-purpose systems
- Automatically adapting to process behavior
- Favoring I/O-bound and interactive processes

---

## 5. Using the Simulator

### Interactive Menu

When you launch without arguments, you get an interactive menu:

```
╔══════════════════════════════════════════════════════════════╗
║        CPU SCHEDULER SIMULATOR - INTERACTIVE MENU            ║
╠══════════════════════════════════════════════════════════════╣
║  1. Generate Random Processes                                ║
║  2. Load Processes from File                                 ║
║  3. Add Processes Manually                                   ║
║  4. Display Current Processes                                ║
║  5. Run Round Robin Scheduler                                ║
║  6. Run Priority Scheduler (Preemptive)                      ║
║  7. Run Priority Scheduler (Non-Preemptive)                  ║
║  8. Run Multilevel Queue Scheduler                           ║
║  9. Run Multilevel Feedback Queue Scheduler                  ║
║ 10. Run All Schedulers (Comparison)                          ║
║ 11. Configure Scheduler Parameters                           ║
║ 12. Export Results to CSV                                    ║
║ 13. Run Performance Benchmark                                ║
║  0. Exit                                                     ║
╚══════════════════════════════════════════════════════════════╝
```

### Creating Processes

**Option 1: Generate Random Processes**
```
Enter number of processes: 10
```
Generates 10 processes with random burst times, arrival times, and priorities.

**Option 2: Load from File**

Create a text file (e.g., `processes.txt`):
```
PID Priority BurstTime ArrivalTime
1 2 10 0
2 1 5 1
3 3 8 2
4 2 4 3
```

Then load:
```bash
./bin/scheduler -f processes.txt -a all
```

**Option 3: Manual Entry**

In interactive mode, select option 3 and enter:
```
Enter process details (PID Priority Burst Arrival): 1 2 10 0
```

### Running Simulations

**Single Algorithm:**
```bash
./bin/scheduler -n 10 -a rr -q 4
```

**Comparison Mode:**
```bash
./bin/scheduler -n 10 -a all
```

This runs all algorithms on the same process set and compares results.

---

## 6. Understanding Results

### Gantt Chart

Visual representation of process execution over time:

```
╔══════════════════════════════════════════════════════════════╗
║                      GANTT CHART                             ║
╠══════════════════════════════════════════════════════════════╣
║ [P1][P2][P3][P1][P2][P3][P1]
║ 0--4--7--11-15--18-22--26
╚══════════════════════════════════════════════════════════════╝
```

Each `[Pn]` block represents a time period where process n is executing.

### Performance Metrics

#### Average Waiting Time
Time processes spend in the ready queue waiting for CPU.
- **Lower is better**
- Indicates system responsiveness

#### Average Turnaround Time
Total time from arrival to completion (waiting + execution).
- **Lower is better**
- Measures overall process completion time

#### Average Response Time
Time from arrival to first CPU execution.
- **Lower is better**
- Critical for interactive applications

#### CPU Utilization
Percentage of time CPU is doing useful work (not idle).
- **Higher is better** (aim for >90%)
- Indicates system efficiency

#### Throughput
Number of processes completed per time unit.
- **Higher is better**
- Measures system productivity

#### Context Switches
Number of times CPU switches between processes.
- **Lower is better** (but necessary for fairness)
- High count indicates overhead

### Comparison Table

```
╔══════════════════╤═══════╤═══════╤═══════╤══════╤══════╤═══════════════╗
║ Algorithm        │  WT   │  TAT  │  RT   │ CPU% │  TP  │ Ctx Switches  ║
╠══════════════════╪═══════╪═══════╪═══════╪══════╪══════╪═══════════════╣
║ Round Robin      │  12.4 │  25.8 │   3.2 │   95 │ 0.39 │            15 ║
║ Priority (Pre)   │  10.2 │  23.1 │   2.8 │   96 │ 0.41 │            12 ║
║ MLFQ            │  11.0 │  24.5 │   2.5 │   94 │ 0.40 │            18 ║
╚══════════════════╧═══════╧═══════╧═══════╧══════╧══════╧═══════════════╝
```

---

## 7. Advanced Features

### Configuring Parameters

**Time Quantum:**
```bash
./bin/scheduler -n 10 -a rr -q 8
```
Larger quantum = less context switching, more like FCFS  
Smaller quantum = more responsive, more overhead

**Context Switch Time:**
```bash
./bin/scheduler -n 10 -a rr -c 2
```
Simulates realistic context switch overhead.

### Benchmarking

Run performance benchmarks with varying workloads:
```bash
./bin/scheduler -b -o benchmark_results.csv
```

Tests with 5, 10, 20, 50, and 100 processes.

### Exporting Results

Save results to CSV for analysis:
```bash
./bin/scheduler -n 20 -a all -o results.csv
```

CSV format:
```csv
Algorithm,AvgWaitTime,AvgTurnaroundTime,AvgResponseTime,CPUUtilization,Throughput,ContextSwitches
Round Robin,12.4,25.8,3.2,95.0,0.39,15
...
```

### Process File Format

Create custom workloads:

```
PID Priority BurstTime ArrivalTime
1 2 10 0
2 1 5 1
3 3 8 2
```

- **PID**: Unique process identifier
- **Priority**: Lower number = higher priority (0-10 recommended)
- **BurstTime**: CPU time required (time units)
- **ArrivalTime**: When process enters system

---

## 8. Troubleshooting

### Common Issues

**Problem: Simulator won't compile**
```bash
# Ensure C++17 support
g++ --version  # Should be 7.0+

# Clean and rebuild
make clean
make build
```

**Problem: Tests fail**
```bash
# Install Google Test
# Ubuntu/Debian:
sudo apt-get install libgtest-dev

# Build and run tests
make test
```

**Problem: Cannot load process file**
- Check file format (space-separated values)
- Ensure header line is present
- Verify file path is correct

**Problem: Unexpected results**
- Verify process parameters are reasonable
- Check context switch time isn't too large
- Review process arrival times

---

## 9. FAQ

**Q: Which algorithm is best?**  
A: It depends on your workload:
- **Interactive systems**: Round Robin or MLFQ
- **Real-time systems**: Priority Scheduling
- **Batch processing**: Non-preemptive algorithms
- **General purpose**: MLFQ

**Q: Why is CPU utilization not 100%?**  
A: CPU can be idle when:
- No processes have arrived yet
- All processes are waiting for I/O
- Context switching overhead

**Q: What is starvation?**  
A: When a process never gets CPU time because others have higher priority. Our simulator uses aging to prevent this.

**Q: How do I choose time quantum?**  
A: Balance between:
- Small (2-4): More responsive, higher overhead
- Medium (5-10): Good balance for most systems
- Large (>10): Acts like FCFS

**Q: Can I add my own scheduling algorithm?**  
A: Yes! Extend the `Scheduler` base class and implement the `run()` method.

**Q: How accurate is the simulation?**  
A: The simulator models core scheduling concepts but simplifies:
- No actual I/O operations
- No memory management
- Discrete time units (not microseconds)

---

## Support and Resources

- **Source Code**: https://github.com/Breezy1214/CPU-Scheduler
- **Issues**: Report bugs on GitHub Issues
- **Documentation**: See `doc/DESIGN.md` for architecture details
- **API Reference**: Run `make docs` to generate Doxygen documentation

