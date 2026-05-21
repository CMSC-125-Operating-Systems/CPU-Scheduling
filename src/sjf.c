#include <stdio.h>
#include <limits.h>
#include "../include/scheduler.h"
#include "../include/process.h"

static int all_complete(Process processes[], int count) {
    for (int i = 0; i < count; i++) {
        if (!processes[i].completed) {
            return 0;
        }
    }

    return 1;
}

static int pick_shortest_job(Process processes[], int count, int current_time) {
    int best = -1;

    for (int i = 0; i < count; i++) {
        Process *p = &processes[i];

        if (p->completed || p->arrival_time > current_time) {
            continue;
        }

        if (best == -1 ||
            p->burst_time < processes[best].burst_time ||
            (p->burst_time == processes[best].burst_time &&
             p->arrival_time < processes[best].arrival_time) ||
            (p->burst_time == processes[best].burst_time &&
             p->arrival_time == processes[best].arrival_time &&
             p->order < processes[best].order)) {
            best = i;
        }
    }

    return best;
}

static int find_next_arrival(Process processes[], int count, int current_time) {
    int next = INT_MAX;

    for (int i = 0; i < count; i++) {
        if (!processes[i].completed &&
            processes[i].arrival_time > current_time &&
            processes[i].arrival_time < next) {
            next = processes[i].arrival_time;
        }
    }

    return next;
}

int schedule_sjf(SchedulerState *state) {
    int current_time = 0;

    if (state == NULL || state->processes == NULL || state->chart == NULL) {
        return -1;
    }

    while (!all_complete(state->processes, state->num_processes)) {
        int index = pick_shortest_job(state->processes, state->num_processes, current_time);

        if (index == -1) {
            int next_time = find_next_arrival(state->processes, state->num_processes, current_time);

            if (next_time == INT_MAX) {
                break;
            }

            add_gantt(state->chart, "IDLE", current_time, next_time);
            current_time = next_time;
            continue;
        }

        Process *p = &state->processes[index];

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