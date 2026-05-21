# CMSC 125 Lab 2 - CPU Scheduler Simulator

A CPU scheduling simulator written in C for CMSC 125 Operating Systems. The program reads process workloads, runs different scheduling algorithms, prints a Gantt chart, and reports scheduling metrics.

## Group Members

- Eleah Joy Melchor
- Jasmine Magadan

## Features

- First-Come First-Serve (FCFS)
- Shortest Job First (SJF)
- Shortest Time-to-Completion First (STCF)
- Round Robin (RR) with configurable quantum
- Multi-Level Feedback Queue (MLFQ) with configurable queue parameters
- File input and inline command-line workload input
- Compare mode for running all algorithms on the same workload
- Gantt chart output
- Average turnaround, waiting, and response time
- Context-switch count
- Automated smoke tests

## Project Structure

```bash
schedsim/
|-- Makefile
|-- README.md
|-- include/
|   |-- process.h
|   |-- scheduler.h
|   |-- metrics.h
|   +-- gantt.h
|-- src/
|   |-- main.c
|   |-- process.c
|   |-- queue.c
|   |-- fcfs.c
|   |-- sjf.c
|   |-- stcf.c
|   |-- rr.c
|   |-- mlfq.c
|   |-- metrics.c
|   |-- gantt.c
|   +-- utils.c
|-- tests/
|   |-- workload1.txt
|   |-- workload2.txt
|   |-- mlfq_config.txt
|   +-- test_suite.sh
+-- docs/
    +-- mlfq_design.md
```

## Build

```bash
make
```

This creates:

```bash
./schedsim
```

Clean build files:

```bash
make clean
```

## Workload Format

Workload files use this format:

```txt
# PID ArrivalTime BurstTime
A 0 5
B 1 3
C 2 8
```

Lines beginning with `#` are ignored.

Inline workloads use this format:

```bash
--processes="A:0:5,B:1:3,C:2:8"
```

## Usage

Run using an input file:

```bash
./schedsim --algorithm=FCFS --input=tests/workload1.txt
./schedsim --algorithm=SJF --input=tests/workload1.txt
./schedsim --algorithm=STCF --input=tests/workload1.txt
./schedsim --algorithm=RR --quantum=2 --input=tests/workload1.txt
./schedsim --algorithm=MLFQ --input=tests/workload1.txt
```

Run using inline process input:

```bash
./schedsim --algorithm=STCF --processes="A:0:5,B:2:3,C:4:1"
```

Run all algorithms for comparison:

```bash
./schedsim --compare --input=tests/workload1.txt
```

Run MLFQ with a config file:

```bash
./schedsim --algorithm=MLFQ --mlfq-config=tests/mlfq_config.txt --input=tests/workload1.txt
```

## MLFQ Configuration

The MLFQ config file uses this format:

```txt
# Queue TimeQuantum Allotment
Q0 10 50
Q1 30 150
Q2 -1 -1
BOOST_PERIOD 200
```

`Q2 -1 -1` means the lowest queue behaves like an FCFS-style queue with no fixed quantum or allotment.

## Tests

Run:

```bash
make test
```

Current test coverage includes FCFS, SJF, STCF, RR, MLFQ, MLFQ config-file loading, compare mode, file input, inline input, and a zero-burst edge case.

Expected result:

```text
All tests passed.
```

## Lab 1 Shell Integration

This simulator is a normal executable, so it can be launched from the Lab 1 shell using `fork()` and `execvp()`.

From the Lab 1 shell, first go to this project folder:

```bash
cd /home/minmin/CPU-Scheduling
```

Then run the simulator normally:

```bash
./schedsim --algorithm=FCFS --input=tests/workload1.txt
./schedsim --algorithm=RR --quantum=2 --processes=A:0:5,B:1:3,C:2:1
./schedsim --compare --input=tests/workload1.txt
```

You can also run it using the absolute path:

```bash
/home/minmin/CPU-Scheduling/schedsim --algorithm=STCF --processes=A:0:5,B:2:3
```

Input redirection works:

```bash
./schedsim --algorithm=FCFS < tests/workload1.txt
```

Output redirection works too:

```bash
./schedsim --algorithm=FCFS --input=tests/workload1.txt > output.txt
```

Note: the Lab 1 parser splits arguments by spaces, so inline workloads should not contain spaces inside the `--processes=` value.

## Known Bugs and Limitations

- Algorithm names are case-sensitive. Use `FCFS`, `SJF`, `STCF`, `RR`, or `MLFQ`.
- Workloads should use non-negative arrival and burst times.
- The Gantt chart is a compact interval-style chart rather than a scaled per-character chart.
- A compile warning may appear for `sort_by_arrival` depending on local header declarations, but the program still builds and runs.

## Development

This lab is written in C and interfaces with standard UNIX process execution through command-line arguments, standard input, standard output, and exit codes.
