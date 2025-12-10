# CPU Scheduler Simulator

[![Build Status](https://github.com/Breezy1214/CPU-Scheduler/workflows/CPU%20Scheduler%20CI%2FCD/badge.svg)](https://github.com/Breezy1214/CPU-Scheduler/actions)
[![License](https://img.shields.io/badge/license-Educational-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](https://github.com/Breezy1214/CPU-Scheduler)

An advanced CPU scheduling algorithm simulator developed in C++ for educational purposes. Implements multiple scheduling algorithms with comprehensive performance analysis, real-time visualization, and professional software engineering practices.

## 🎯 Features

### Supported Algorithms
- **Round Robin (RR)** - Time-sliced preemptive scheduling
- **Priority Scheduling** - Both preemptive and non-preemptive modes
- **Multilevel Queue (MLQ)** - Fixed priority queues for different process types
- **Multilevel Feedback Queue (MLFQ)** - Dynamic queue adjustment based on process behavior

### Key Capabilities
✅ Real-time process execution visualization  
✅ Comprehensive performance metrics calculation  
✅ Context switching simulation with overhead  
✅ Starvation prevention through aging mechanisms  
✅ Side-by-side algorithm comparison  
✅ Process import/export (CSV format)  
✅ Performance benchmarking suite  
✅ Interactive and command-line interfaces  

## 📊 Performance Metrics

The simulator calculates and displays:
- Average Waiting Time
- Average Turnaround Time
- Average Response Time
- CPU Utilization
- Throughput (processes/time unit)
- Context Switch Count and Overhead

## 🚀 Quick Start

### Prerequisites
```bash
- C++17 compatible compiler (g++ 7.0+, clang++ 5.0+)
- GNU Make
```

### Building

```bash
# Clone the repository
git clone https://github.com/Breezy1214/CPU-Scheduler.git
cd CPU-Scheduler

# Build the project
make build

# Run the simulator
./bin/scheduler
```

### Quick Demo

```bash
# Run a quick demonstration
./bin/scheduler --demo

# Compare all algorithms with 10 random processes
./bin/scheduler -n 10 -a all

# Run Round Robin with custom quantum
./bin/scheduler -n 5 -a rr -q 8
```

## 📖 Usage

### Interactive Mode

Launch without arguments for an interactive menu:
```bash
./bin/scheduler
```

### Command-Line Mode

```bash
# Generate processes and run specific algorithm
./bin/scheduler -n <count> -a <algorithm> [options]

# Load processes from file
./bin/scheduler -f processes.txt -a all

# Run benchmark
./bin/scheduler -b -o results.csv
```

### Algorithm Options
- `rr` - Round Robin
- `pp` - Priority Preemptive
- `pnp` - Priority Non-Preemptive
- `mlq` - Multilevel Queue
- `mlfq` - Multilevel Feedback Queue
- `all` - Run all and compare

### Example Process File Format
```
PID Priority BurstTime ArrivalTime
1 2 10 0
2 1 5 1
3 3 8 2
```

## 🏗️ Project Structure

```
CPU-Scheduler/
├── src/              # Source files (.cpp)
│   ├── main.cpp
│   ├── Scheduler.cpp
│   ├── RoundRobinScheduler.cpp
│   ├── PriorityScheduler.cpp
│   ├── MultilevelQueueScheduler.cpp
│   ├── MultilevelFeedbackQueueScheduler.cpp
│   ├── Process.cpp
│   ├── Metrics.cpp
│   ├── Simulator.cpp
│   └── Visualizer.cpp
├── include/          # Header files (.h)
│   ├── Scheduler.h
│   ├── Process.h
│   ├── Metrics.h
│   └── ...
├── test/             # Unit tests
│   ├── test_process.cpp
│   ├── test_schedulers.cpp
│   └── test_main.cpp
├── doc/              # Documentation
│   ├── USER_MANUAL.md
│   ├── DESIGN.md
│   └── API/
├── scripts/          # Build and utility scripts
├── bin/              # Compiled executables
├── build/            # Object files
├── Makefile          # Build system
└── README.md
```

## 🔧 Build System

### Makefile Targets

```bash
make build      # Build optimized release version
make debug      # Build with debug symbols
make test       # Build and run unit tests
make clean      # Remove build artifacts
make install    # Install to ~/bin
make docs       # Generate API documentation
make benchmark  # Run performance benchmark
make format     # Format code with clang-format
make lint       # Run static analysis
```

## 🧪 Testing

Comprehensive test suite:

```bash
# Run all tests
make test

# Or run the test executable directly
./bin/test_runner
```

### Test Coverage
- Unit tests for all core components
- Integration tests for algorithm correctness
- Performance benchmarks
- Edge case validation

## 📚 Documentation

- **[User Manual](doc/USER_MANUAL.md)** - Complete usage guide
- **[Design Document](doc/DESIGN.md)** - Architecture and implementation details
- **API Documentation** - Generate with `make docs`

## 🎓 Educational Value

This project demonstrates:
- **Operating System Concepts**: Process scheduling, context switching, CPU utilization
- **Data Structures**: Queues, priority queues, process control blocks
- **Algorithm Design**: Time complexity analysis, optimization strategies
- **Software Engineering**: OOP design patterns, modular architecture, testing
- **Performance Analysis**: Metrics calculation, algorithm comparison

## 👥 Team Members

This project is collaboratively developed by:
- **Team Member 1** - Algorithm Implementation
- **Team Member 2** - Testing & Documentation
- **Team Member 3** - Visualization & CI/CD


## 📊 Performance Benchmarks

Sample results from benchmark suite:

| Algorithm | Avg Wait Time | Avg Turnaround | CPU Util | Throughput |
|-----------|--------------|----------------|----------|------------|
| Round Robin | 12.4 | 25.8 | 95% | 0.39 |
| Priority (P) | 10.2 | 23.1 | 96% | 0.41 |
| Priority (NP) | 14.7 | 28.2 | 94% | 0.37 |
| MLQ | 11.8 | 24.5 | 95% | 0.40 |
| MLFQ | 11.0 | 23.9 | 96% | 0.41 |

*Results with 10 processes, quantum=4, context switch time=1*

## 🔬 Algorithms Explained

### Round Robin
Fair time-sharing algorithm where each process gets equal CPU time slices. Excellent for interactive systems.

### Priority Scheduling
Processes with higher priority execute first. Aging prevents starvation of low-priority processes.

### Multilevel Queue
Processes are assigned to fixed priority queues based on their type (system, interactive, batch).

### Multilevel Feedback Queue
Processes dynamically move between queues based on behavior. CPU-bound processes move down, I/O-bound stay up.

## 🐛 Known Issues & Limitations

- No actual I/O simulation (processes don't block)
- Single-core simulation only
- Discrete time units (not real-time microseconds)
- No memory management simulation

See [Issues](https://github.com/Breezy1214/CPU-Scheduler/issues) for the full list.

## 🔮 Future Enhancements

- [ ] Additional algorithms (SJF, SRTF, CFS)
- [ ] Multi-core CPU simulation
- [ ] I/O operations and blocking
- [ ] GUI interface
- [ ] Real-time process creation
- [ ] Machine learning for burst time prediction


## 🙏 Acknowledgments

- Course instructors for project requirements
- Operating System textbooks (Silberschatz, Tanenbaum)
- Open-source community for tools and libraries


---
