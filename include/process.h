#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>

#define MAX_PID_LENGTH 16
#define MAX_PROCESSES 256

//STRUCT process 
typedef struct {
    char pid[MAX_PID_LENGTH];           // Process identifier
    int arrival_time;       // When process arrives
    int burst_time;         // Total CPU time needed
    int order;              // Input order for stable tie-breaking
    int remaining_time;     // For preemptive algorithms
    int start_time;         // When first executed (for RT)
    int finish_time;        // When completed (for TT)
    int waiting_time;       // Time spent waiting
    int priority;           // For MLFQ
    int time_in_queue;      // For MLFQ allotment tracking
    int turnaround_time;
    int response_time;
    int completed;
    int queued;
} Process;

//STRUCT node
typedef struct Node {
    Process *proc;
    struct Node *next;
} Node;

//STRUCT queue
typedef struct {
    Node *head;
    Node *tail;
    int size;
} Queue;

Queue *create_queue();
void enqueue(Queue *q, Process *p);
Process *dequeue(Queue *q);
Process *peek(Queue *q);
void free_queue(Queue *q);

void init_process(Process *process, const char *pid, int arrival_time, int burst_time);
void init_processes(Process processes[], int count);
void copy_processes(Process dest[], const Process src[], int count);
void reset_process(Process *process);
void reset_processes(Process processes[], int count);
int find_process_index_by_pid(const Process processes[], int count, const char *pid);
int load_processes_from_stream(FILE *stream, Process processes[], int max_processes);
int load_processes_from_file(const char *path, Process processes[], int max_processes);
int load_processes_from_inline(const char *spec, Process processes[], int max_processes);

#endif
