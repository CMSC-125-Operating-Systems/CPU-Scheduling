#include "../include/process.h"
#include <string.h>
#include <stdlib.h>

// Sorts processes by arrival time (essential for FCFS)
void sort_by_arrival(Process *procs, int n) {
    qsort(procs, n, sizeof(Process), compare_arrival);
}

// A helper to find a process by its PID string
Process* find_process_by_id(Process *procs, int n, char* id) {
    for(int i = 0; i < n; i++) {
        if(strcmp(procs[i].pid, id) == 0) return &procs[i];
    }
    return NULL;
}
