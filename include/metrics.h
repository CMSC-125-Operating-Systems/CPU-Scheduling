#ifndef METRICS_H
#define METRICS_H
#include "process.h"

typedef struct {
    double avg_turnaround;
    double avg_waiting;
    double avg_response;
    double throughput;
} Metrics;

void compute_metrics(const Process processes[], int count, int total_time, Metrics *metrics);
void print_metrics(const Process processes[], int count, const Metrics *metrics);

#endif