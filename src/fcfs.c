#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/process.h"

int schedule_fcfs(SchedulerState *state) {
    int current_time = 0;

    if (state == NULL || state->processes == NULL || state->chart == NULL) {
        return -1;
    }

    sort_by_arrival(state->processes, state->num_processes);

    for (int i = 0; i < state->num_processes; i++) {
        Process *p = &state->processes[i];

        if (current_time < p->arrival_time) {
            add_gantt(state->chart, "IDLE", current_time, p->arrival_time);
            current_time = p->arrival_time;
        }

        p->start_time = current_time;
        p->response_time = p->start_time - p->arrival_time;

        add_gantt(state->chart, p->pid, current_time, current_time + p->burst_time);

        current_time += p->burst_time;

        p->remaining_time = 0;
        p->finish_time = current_time;
        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->completed = 1;
    }

    state->current_time = current_time;
    return 0;
}