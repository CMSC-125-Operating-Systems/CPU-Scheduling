#include <stdio.h>
#include "process.h"
#define MAX_INPUT_PER_LINE 100
#define MAX_PROCESSES_PER_WORKLOAD 100
int main(){
    char process_input[MAX_INPUT_PER_LINE];
    int total_processes;
    int i = 0;
    int j = 0;
    Process processes[MAX_PROCESSES_PER_WORKLOAD];
    
    while(i < MAX_PROCESSES_PER_WORKLOAD && fgets(process_input, sizeof(process_input), stdin)){
        if (process_input[0] == '#' || process_input[0] == '\n') continue;
        if (sscanf(process_input, "%s %d %d", processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time) == 3) i++;
    }

    total_processes = i;

    while (j < total_processes){
        printf("PID: %s, Arrival Time: %d, Burst Time: %d\n", processes[j].pid, processes[j].arrival_time, processes[j].burst_time);
        j++;
    }

    return 0;
}