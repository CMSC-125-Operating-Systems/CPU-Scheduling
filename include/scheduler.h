
//IMPLEMENTATION 

// typedef struct {
//     int level;              // Queue priority level (0 = highest)
//     int time_quantum;       // Time slice for this queue (-1 for FCFS)
//     int allotment;          // Max time before demotion (-1 for infinite)
//     Process *queue;         // Array or linked list of processes
//     int size;               // Current queue size
// } MLFQQueue;

// typedef struct {
//     MLFQQueue *queues;      // Array of queues
//     int num_queues;         // Number of priority levels
//     int boost_period;       // Period for priority boost (S)
//     int last_boost;         // Last boost time
// } MLFQScheduler;

// typedef struct {
//     Process *processes;     // Array of all processes
//     int num_processes;      // Number of processes
//     int current_time;       // Current simulation time
//     // ... additional fields for metrics, Gantt chart, etc.
//     // Recall: CMSC 141
// } SchedulerState;

// // Return 0 on success, -1 on error (command line etiquette)
// int schedule_fcfs(SchedulerState *state);
// int schedule_sjf(SchedulerState *state);
// int schedule_stcf(SchedulerState *state);
// int schedule_rr(SchedulerState *state, int quantum);
// int schedule_mlfq(SchedulerState *state, MLFQConfig *config);

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "gantt.h"
#include "process.h"

typedef enum {
    ALG_FCFS,
    ALG_SJF,
    ALG_STCF,
    ALG_RR,
    ALG_MLFQ
} Algorithm;

int schedule_fcfs(Process processes[], int count, GanttChart *chart);
int schedule_sjf(Process processes[], int count, GanttChart *chart);
int schedule_stcf(Process processes[], int count, GanttChart *chart);
int schedule_rr(Process processes[], int count, int quantum, GanttChart *chart);
int schedule_mlfq(Process processes[], int count, const int quantums[], const int allotments[], int levels, int boost_period, GanttChart *chart);

#endif
