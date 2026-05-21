#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "gantt.h"
#include "process.h"

typedef struct {
    Process *processes;
    int num_processes;
    int current_time;
    GanttChart *chart;
} SchedulerState;

typedef struct {
    int levels;
    int quantums[8];
    int allotments[8];
    int boost_period;
    int print_trace;
} MLFQConfig;

int schedule_fcfs(SchedulerState *state);
int schedule_sjf(SchedulerState *state);
int schedule_stcf(SchedulerState *state);
int schedule_rr(SchedulerState *state, int quantum);
int schedule_mlfq(SchedulerState *state, MLFQConfig *config);

#endif