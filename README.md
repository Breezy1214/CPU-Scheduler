# CPU Scheduler Simulator

A C++ application for simulating CPU scheduling algorithms. Implements Round Robin, Priority Scheduling, Multilevel Queue, and Multilevel Feedback Queue with performance analysis.

## Features

- Multiple scheduling algorithms
- Performance metrics calculation
- Real-time visualization
- Context switching simulation
- Process import/export

## Quick Start

### Prerequisites

- C++17 compiler
- GNU Make

### Build

```bash
make build
./bin/scheduler
```

### Demo

```bash
./bin/scheduler --demo

# Compare all algorithms with 10 random processes
./bin/scheduler -n 10 -a all

# Run Round Robin with custom quantum
./bin/scheduler -n 5 -a rr -q 8
```

## Usage

### Interactive Mode

```bash
./bin/scheduler
```

### Command-Line

```bash
./bin/scheduler -n <count> -a <algorithm>
```

Algorithms: rr, pp, pnp, mlq, mlfq, all

## Project Structure

```text
CPU-Scheduler/
├── src/              # Source files
├── include/          # Headers
├── test/             # Tests
├── doc/              # Documentation
├── scripts/          # Build scripts
├── bin/              # Executables
└── Makefile
```

## Build Targets

- `make build` - Release build
- `make test` - Run tests
- `make clean` - Clean build files

## Team Members

This project is collaboratively developed by:
- **Chidiebube Nwabuike and De'Ja Smith** - Algorithm Implementation
- **Raven Goings** - Testing & Documentation
- **Shelsie Roberts** - Visualization & CI/CD



## Performance Benchmarks

Sample results from benchmark suite:

| Algorithm | Avg Wait Time | Avg Turnaround | CPU Util | Throughput |
|-----------|--------------|----------------|----------|------------|
| Round Robin | 12.4 | 25.8 | 95% | 0.39 |
| Priority (P) | 10.2 | 23.1 | 96% | 0.41 |
| Priority (NP) | 14.7 | 28.2 | 94% | 0.37 |
| MLQ | 11.8 | 24.5 | 95% | 0.40 |
| MLFQ | 11.0 | 23.9 | 96% | 0.41 |

*Results with 10 processes, quantum=4, context switch time=1*

## Algorithms Explained

### Round Robin
Fair time-sharing algorithm where each process gets equal CPU time slices. Excellent for interactive systems.

### Priority Scheduling
Processes with higher priority execute first. Aging prevents starvation of low-priority processes.

### Multilevel Queue
Processes are assigned to fixed priority queues based on their type (system, interactive, batch).

### Multilevel Feedback Queue
Processes dynamically move between queues based on behavior. CPU-bound processes move down, I/O-bound stay up.

## Known Issues & Limitations

- No actual I/O simulation (processes don't block)
- Single-core simulation only
- Discrete time units (not real-time microseconds)
- No memory management simulation


## Documentation

- [Design Document](doc/DESIGN.md)
- [User Manual](doc/USER_MANUAL.md)
- [Performance Analysis](doc/PERFORMANCE_ANALYSIS.md)
