#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/process.h"

int schedule_fcfs(SchedulerState *state) {
    // 1. Logic Variable: Keep track of the "wall clock" time
    int current_time = 0;

    // 2. Loop through all processes in the order they arrived
    for (int i = 0; i < state->num_processes; i++) {
        
        // This is how you "access the variables" easily:
        // We create a pointer 'p' that points to the specific process in the array.
        Process *p = &state->processes[i];

        // 3. Handle Idle Time
        // If the CPU is free at t=10 but the next process arrives at t=15,
        // the clock must jump to 15.
        if (current_time < p->arrival_time) {
            current_time = p->arrival_time;
        }

        // 4. Set Response Time (RT)
        // In FCFS, the response time is exactly when it starts minus when it arrived.
        p->start_time = current_time;
        p->response_time = p->start_time - p->arrival_time;

        // 5. Simulate Execution (Non-preemptive)
        // Since it's FCFS, we just "jump" the clock by the burst time.
        current_time += p->burst_time;
        p->finish_time = current_time;

        // 6. Calculate remaining metrics
        // Turnaround Time (TT) = Finish - Arrival
        // Waiting Time (WT) = Turnaround - Burst (or Start - Arrival)
        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->start_time - p->arrival_time;

        // Optional: Print progress for debugging
        printf("Process %s finished at time %d\n", p->pid, p->finish_time);
    }

    return 0; // Success code for "command line etiquette"
}