#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>

#define MAX_PID_LENGTH 16
#define MAX_PROCESSES 100

//STRUCT process 
typedef struct {
    char pid[MAX_PID_LENGTH];           // Process identifier
    int arrival_time;       // When process arrives
    int burst_time;         // Total CPU time needed
    int remaining_time;     // For preemptive algorithms
    int start_time;         // When first executed (for RT)
    int finish_time;        // When completed (for TT)
    int waiting_time;       // Time spent waiting
    int priority;           // For MLFQ
    int time_in_queue;      // For MLFQ allotment tracking
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
//IMPLEMENTATION 

#endif