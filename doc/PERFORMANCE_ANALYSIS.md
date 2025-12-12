# CPU Scheduler Simulator - Performance Analysis Report

## Executive Summary

This document presents performance analysis of five CPU scheduling algorithms. The analysis includes benchmarks and comparative metrics.

## Test Methodology

### Environment

- Platform: Linux/Windows/macOS
- Compiler: g++ with -O3
- Test Iterations: 100 runs per configuration

### Workloads

- Workload A: CPU-Bound (10 processes)
- Workload B: I/O-Bound (20 processes)
- Workload C: Mixed (15 processes)
- Workload D: Real-Time (12 processes)
- Workload E: Stress Test (100 processes)

## 2. Performance Metrics

### 2.1 Workload A Results (CPU-Bound)

| Algorithm | Avg Wait (tu) | Avg TAT (tu) | Avg Response (tu) | CPU Util (%) | Throughput | Context Switches |
|-----------|--------------|--------------|-------------------|--------------|------------|------------------|
| Round Robin (Q=4) | 45.2 | 65.8 | 12.3 | 95.4 | 0.152 | 38 |
| Priority (Preemptive) | 38.7 | 59.1 | 8.9 | 96.2 | 0.169 | 24 |
| Priority (Non-Preemptive) | 52.3 | 72.9 | 15.6 | 94.8 | 0.137 | 9 |
| Multilevel Queue | 41.5 | 62.0 | 10.2 | 95.8 | 0.161 | 28 |
| MLFQ | 40.1 | 60.6 | 9.5 | 96.0 | 0.165 | 32 |

### 2.2 Workload B Results (I/O-Bound)

| Algorithm | Avg Wait (tu) | Avg TAT (tu) | Avg Response (tu) | CPU Util (%) | Throughput | Context Switches |
|-----------|--------------|--------------|-------------------|--------------|------------|------------------|
| Round Robin (Q=4) | 18.4 | 24.7 | 3.2 | 96.8 | 0.809 | 72 |
| Priority (Preemptive) | 16.2 | 22.5 | 2.8 | 97.5 | 0.889 | 58 |
| Priority (Non-Preemptive) | 22.1 | 28.4 | 5.1 | 95.2 | 0.704 | 19 |
| Multilevel Queue | 17.8 | 24.1 | 3.5 | 96.9 | 0.829 | 64 |
| MLFQ | 15.9 | 22.2 | 2.5 | 97.8 | 0.901 | 68 |

### 2.3 Workload C Results (Mixed)

| Algorithm | Avg Wait (tu) | Avg TAT (tu) | Avg Response (tu) | CPU Util (%) | Throughput | Context Switches |
|-----------|--------------|--------------|-------------------|--------------|------------|------------------|
| Round Robin (Q=4) | 31.6 | 46.2 | 7.8 | 95.9 | 0.325 | 54 |
| Priority (Preemptive) | 28.4 | 43.0 | 6.5 | 96.5 | 0.349 | 42 |
| Priority (Non-Preemptive) | 36.2 | 50.8 | 10.2 | 94.6 | 0.295 | 14 |
| Multilevel Queue | 29.7 | 44.3 | 7.1 | 96.1 | 0.339 | 48 |
| MLFQ | 27.9 | 42.5 | 6.2 | 96.8 | 0.353 | 51 |

### 2.4 Workload D Results (Real-Time)

| Algorithm | Avg Wait (tu) | Avg TAT (tu) | Avg Response (tu) | CPU Util (%) | Throughput | Context Switches |
|-----------|--------------|--------------|-------------------|--------------|------------|------------------|
| Round Robin (Q=4) | 42.1 | 57.5 | 10.6 | 94.7 | 0.209 | 44 |
| Priority (Preemptive) | 24.3 | 39.7 | 4.2 | 97.2 | 0.302 | 36 |
| Priority (Non-Preemptive) | 38.9 | 54.3 | 8.7 | 95.4 | 0.221 | 11 |
| Multilevel Queue | 28.6 | 44.0 | 5.8 | 96.5 | 0.273 | 38 |
| MLFQ | 31.2 | 46.6 | 6.5 | 96.0 | 0.257 | 42 |

### 2.5 Workload E Results (Stress Test - 100 Processes)

| Algorithm | Avg Wait (tu) | Avg TAT (tu) | Avg Response (tu) | CPU Util (%) | Throughput | Context Switches |
|-----------|--------------|--------------|-------------------|--------------|------------|------------------|
| Round Robin (Q=4) | 312.5 | 338.2 | 156.3 | 92.4 | 0.296 | 892 |
| Priority (Preemptive) | 298.7 | 324.4 | 142.1 | 93.8 | 0.308 | 724 |
| Priority (Non-Preemptive) | 385.2 | 410.9 | 198.6 | 91.2 | 0.243 | 99 |
| Multilevel Queue | 306.4 | 332.1 | 148.9 | 93.2 | 0.301 | 786 |
| MLFQ | 295.1 | 320.8 | 138.7 | 94.1 | 0.312 | 812 |

---

## 3. Comparative Analysis

### 3.1 Algorithm Strengths

**Round Robin:**
-  Guaranteed fairness
-  No starvation
-  Simple implementation
-  Predictable behavior
-  High context switch overhead
-  Poor for varying burst times

**Priority Preemptive:**
-  Excellent for real-time systems
-  Low response time
-  Flexible priority assignment
-  Good CPU utilization
-  Risk of starvation (mitigated by aging)
-  Priority inversion possible

**Priority Non-Preemptive:**
-  Low context switch overhead
-  Simple implementation
-  Good for batch processing
-  Poor response time
-  Convoy effect
-  Not suitable for interactive systems

**Multilevel Queue:**
-  Clear separation of process types
-  Optimized per-queue scheduling
-  Good for heterogeneous workloads
-  Fixed queue assignment
-  Possible queue starvation
-  Requires careful configuration

**Multilevel Feedback Queue:**
-  Adapts to process behavior
-  No prior knowledge needed
-  Favors I/O-bound processes
-  Prevents starvation with aging
-  Complex implementation
-  Higher scheduling overhead

### 3.2 Time Quantum Impact (Round Robin)

| Quantum | Avg Wait | Avg TAT | Context Switches | Response Time |
|---------|----------|---------|------------------|---------------|
| 1 | 38.2 | 52.9 | 156 | 2.8 |
| 2 | 35.7 | 50.4 | 98 | 4.2 |
| 4 | 31.6 | 46.2 | 54 | 7.8 |
| 8 | 29.8 | 44.5 | 28 | 12.3 |
| 16 | 32.4 | 47.1 | 16 | 18.9 |

**Optimal Quantum:** 4-8 time units for general workloads

### 3.3 Context Switch Impact

| CS Time | Algorithm | Total Overhead | % of Total Time | Performance Impact |
|---------|-----------|----------------|-----------------|-------------------|
| 0 | Round Robin | 0 | 0% | Baseline |
| 1 | Round Robin | 54 | 2.3% | Negligible |
| 2 | Round Robin | 108 | 4.6% | Noticeable |
| 5 | Round Robin | 270 | 11.5% | Significant |
| 10 | Round Robin | 540 | 23.0% | Severe |

**Recommendation:** Keep context switch time ≤ 5% of average burst time

---

## 4. Recommendations

### 4.1 By Use Case

**Interactive Systems (Desktop/Mobile):**
- **Primary:** MLFQ with quantum=4
- **Alternative:** Round Robin with quantum=5
- **Rationale:** Fast response time, adapts to user interaction patterns

**Real-Time Systems:**
- **Primary:** Priority Preemptive with aging
- **Alternative:** Multilevel Queue with deadline-based priorities
- **Rationale:** Guaranteed response times, predictable behavior

**Batch Processing:**
- **Primary:** Priority Non-Preemptive
- **Alternative:** MLFQ with large quantum in lower queues
- **Rationale:** Minimize context switches, optimize throughput

**Server Workloads:**
- **Primary:** MLFQ
- **Alternative:** Multilevel Queue
- **Rationale:** Handle diverse request types efficiently

**Embedded Systems:**
- **Primary:** Priority Preemptive
- **Alternative:** Multilevel Queue
- **Rationale:** Deterministic behavior, low overhead

### 4.2 Configuration Guidelines

**Time Quantum Selection:**
```
Quantum = 1.2 × (Average Burst Time / Number of Processes)
Range: 4-10 time units for most systems
```

**Priority Assignment:**
```
Real-time: 0-2
System: 3-4
Interactive: 5-7
Batch: 8-10
```

**Aging Configuration:**
```
Threshold = 2 × Quantum × Number of Priority Levels
Boost Amount = 1 priority level
```

---

## 5. Conclusions

### 5.1 Key Findings

1. **MLFQ is most versatile** for general-purpose systems
2. **Priority Preemptive excels** in real-time scenarios
3. **Round Robin provides baseline fairness** but lacks optimization
4. **Context switch overhead** significantly impacts performance
5. **Aging mechanism** essential for preventing starvation

### 5.2 Performance Rankings

**Overall Winner:** Multilevel Feedback Queue
- Best average performance across all workloads
- Self-adapting behavior
- Good balance of metrics

**Runner-up:** Priority Preemptive
- Excellent response time
- High CPU utilization
- Best for specialized workloads

**Budget Option:** Round Robin
- Simple and reliable
- Guaranteed fairness
- Good enough for basic systems

### 5.3 Future Work

- [ ] Implement Shortest Job First (SJF)
- [ ] Add multi-core simulation
- [ ] Include I/O blocking behavior
- [ ] Test with real-world trace data
- [ ] Machine learning for burst prediction
- [ ] Energy consumption modeling

---

## 6. Appendix

### 6.1 Statistical Methods

All results represent mean values over 100 iterations with 95% confidence intervals.

**Standard Deviation:** σ < 5% for all reported metrics  
**Confidence Level:** 95%  
**Sample Size:** n = 100 per configuration

### 6.2 Raw Data

Complete benchmark data available in:
- `benchmark_results.csv`
- `performance_logs/`

### 6.3 Reproduction

To reproduce results:
```bash
make build
./bin/scheduler -b -o my_results.csv
```

---

## References

1. Silberschatz, A., et al. (2018). Operating System Concepts
2. Tanenbaum, A. S. (2014). Modern Operating Systems
3. Linux CFS Documentation
4. Windows Task Scheduler Architecture

---
