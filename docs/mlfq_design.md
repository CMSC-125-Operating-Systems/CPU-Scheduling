# MLFQ Design Justification
Our design uses 3 priority levels because our test workloads showed a clear distinction between new interactive jobs (<50 time units), mixed workloads (50–200 time units), and long batch tasks (>200 time units). Using 3 queues provides the optimal balance between granular scheduling and low overhead, avoiding the unnecessary context switching that a 4th queue introduced during testing.

We set quantum sizes to 10 for Q0, 30 for Q1, and an FCFS-like structure for Q2. This short initial quantum gives newly arrived and interactive jobs rapid response times, while the increasing quantum sizes reduce context-switching overhead as jobs reveal themselves to be CPU-bound.

Processes are prevented from monopolizing high priorities by allotment limits set to exactly five times each queue's quantum (50 for Q0 and 150 for Q1). This 5:1 ratio gives mixed workloads enough execution slices to finish within their priority tier without being penalized by a single burst, while still efficiently demoting long computations.

A global boost period of 200 time units was selected after empirical testing showed it successfully prevents the starvation of lower-priority jobs during high-arrival periods, while allowing processes that change behavior (shifting from CPU-bound to interactive) to recover their high-priority status.

## Testing

The implementation was tested using:

```bash
make test
```

Which produced the output:

```bash
Running CMSC 125 Lab 2 Test Suite...

Test: FCFS basic workload
  PASS

Test: SJF basic workload
  PASS

Test: STCF inline workload
  PASS

Test: RR quantum workload
  PASS

Test: MLFQ basic workload
  PASS

Test: Compare mode
  PASS

Test: Zero burst edge case
  PASS

All tests passed.
```