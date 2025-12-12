# CPU Scheduler Simulator - User Manual

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

## 1. Introduction

The CPU Scheduler Simulator simulates CPU scheduling algorithms. It calculates performance metrics and provides visual output.

### Supported Algorithms

- Round Robin
- Priority Scheduling (preemptive and non-preemptive)
- Multilevel Queue
- Multilevel Feedback Queue

### Key Features

- Real-time visualization
- Performance metrics
- Context switching simulation
- Process import/export

---

## 2. Installation

### Prerequisites

- C++17 compiler
- Make

### Building

```bash
cd CPU-Scheduler
make build
./bin/scheduler
```

### Installation

```bash
make install
```

---

## 3. Getting Started

### Quick Start

```bash
./bin/scheduler
```

### Command-Line

```bash
./bin/scheduler -n 10 -a rr
```

Algorithms: rr, pp, pnp, mlq, mlfq, all

---

## 4. Scheduling Algorithms

### Round Robin

Time-sliced preemptive scheduling.

Best for: Time-sharing systems.

### Priority Scheduling

Preemptive and non-preemptive modes.

Best for: Systems with different priorities.

### Multilevel Queue

Fixed queues based on priority.

Best for: Distinct process types.

### Multilevel Feedback Queue

Dynamic queue movement.

Best for: General-purpose systems.

---

## 5. Using the Simulator

### Interactive Menu

Run without arguments for menu.

### Creating Processes

- Generate random: Enter count
- Load from file: CSV format PID,Priority,Burst,Arrival
- Manual entry: Enter details

### Running Simulations

Single: -a <algorithm>
Comparison: -a all

---

## 6. Understanding Results

### Gantt Chart

Timeline of execution.

### Performance Metrics

- Average Waiting Time
- Average Turnaround Time
- Average Response Time
- CPU Utilization
- Throughput
- Context Switches

### Comparison Table

Shows metrics for algorithms.

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

- Time Quantum: -q <value>
- Context Switch Time: -c <value>

### Benchmarking

```bash
./bin/scheduler -b -o results.csv
```

### Exporting Results

```bash
./bin/scheduler -n 20 -a all -o results.csv
```

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

- Compile error: Check C++17 support
- Tests fail: Rebuild
- File load error: Check format

## 9. FAQ

**Which algorithm is best?**
Depends on workload: MLFQ for general, Priority for real-time, RR for fair.

**Why not 100% CPU utilization?**
Due to idle time or context switches.

**What is starvation?**
Process never gets CPU. Aging prevents it.

**Choose quantum?**
4-8 for most systems.

**Add algorithm?**
Extend Scheduler base class.

