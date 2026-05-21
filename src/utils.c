#include "../include/process.h"
#include <string.h>
#include <stdlib.h>

static int compare_arrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

// Sorts processes by arrival time (essential for FCFS)
void sort_by_arrival(Process *processes, int n) {
    qsort(processes, n, sizeof(Process), compare_arrival);
}

// A helper to find a process by its PID string
Process* find_process_by_id(Process *procs, int n, char* id) {
    for(int i = 0; i < n; i++) {
        if(strcmp(procs[i].pid, id) == 0) return &procs[i];
    }
    return NULL;
}
