#!/bin/bash

set -u

echo "Running CMSC 125 Lab 2 Test Suite..."
echo

failures=0

run_test() {
    name="$1"
    command="$2"
    expected="$3"

    echo "Test: $name"
    eval "$command" > /tmp/schedsim_test_output.txt 2>&1
    status=$?

    if [ "$status" -ne 0 ]; then
        echo "  FAIL: command exited with status $status"
        cat /tmp/schedsim_test_output.txt
        echo
        failures=$((failures + 1))
        return
    fi

    if grep -q "$expected" /tmp/schedsim_test_output.txt; then
        echo "  PASS"
    else
        echo "  FAIL: expected output containing: $expected"
        echo "  Actual output:"
        cat /tmp/schedsim_test_output.txt
        failures=$((failures + 1))
    fi

    echo
}

run_test \
    "FCFS basic workload" \
    "./schedsim --algorithm=FCFS --input=tests/workload1.txt" \
    "Average Turnaround Time"

run_test \
    "SJF basic workload" \
    "./schedsim --algorithm=SJF --input=tests/workload1.txt" \
    "Average Waiting Time"

run_test \
    "STCF inline workload" \
    "./schedsim --algorithm=STCF --processes='A:0:5,B:2:3,C:4:1'" \
    "Context Switches"

run_test \
    "RR quantum workload" \
    "./schedsim --algorithm=RR --quantum=2 --processes='A:0:5,B:1:3,C:2:1'" \
    "[A 0-2]"

run_test \
    "MLFQ basic workload" \
    "./schedsim --algorithm=MLFQ --input=tests/workload1.txt" \
    "MLFQ Configuration"

run_test \
    "MLFQ config file" \
    "./schedsim --algorithm=MLFQ --mlfq-config=tests/mlfq_config.txt --input=tests/workload1.txt" \
    "Boost period: 200"

run_test \
    "Compare mode" \
    "./schedsim --compare --input=tests/workload1.txt" \
    "Algorithm Comparison"

run_test \
    "Zero burst edge case" \
    "./schedsim --algorithm=STCF --processes='A:0:0,B:1:2'" \
    "Average Turnaround Time"

if [ "$failures" -eq 0 ]; then
    echo "All tests passed."
    exit 0
else
    echo "$failures test(s) failed."
    exit 1
fi