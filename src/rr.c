#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/process.h"

static int completed_count(Process processes[], int count) {
    int completed = 0;

    for (int i = 0; i < count; i++) {
        if (processes[i].completed) {
            completed++;
        }
    }

    return completed;
}

static void enqueue_arrivals(Process processes[], int count, Queue *ready, int current_time) {
    for (int i = 0; i < count; i++) {
        if (!processes[i].completed &&
            !processes[i].queued &&
            processes[i].arrival_time == current_time) {
            enqueue(ready, &processes[i]);
            processes[i].queued = 1;
        }
    }
}

int schedule_rr(SchedulerState *state, int quantum) {
    Queue *ready;
    int current_time = 0;

    if (state == NULL || state->processes == NULL || state->chart == NULL) {
        return -1;
    }

    if (quantum <= 0) {
        quantum = 30;
    }

    ready = create_queue();
    if (ready == NULL) {
        return -1;
    }

    while (completed_count(state->processes, state->num_processes) < state->num_processes) {
        enqueue_arrivals(state->processes, state->num_processes, ready, current_time);

        if (ready->size == 0) {
            add_gantt(state->chart, "IDLE", current_time, current_time + 1);
            current_time++;
            continue;
        }

        Process *p = dequeue(ready);
        p->queued = 0;

        if (p->start_time == -1) {
            p->start_time = current_time;
            p->response_time = p->start_time - p->arrival_time;
        }

        int used = 0;

        while (used < quantum && p->remaining_time > 0) {
            add_gantt(state->chart, p->pid, current_time, current_time + 1);

            p->remaining_time--;
            used++;
            current_time++;

            enqueue_arrivals(state->processes, state->num_processes, ready, current_time);
        }

        if (p->remaining_time == 0) {
            p->finish_time = current_time;
            p->turnaround_time = p->finish_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->completed = 1;
        } else {
            enqueue(ready, p);
            p->queued = 1;
        }
    }

    free_queue(ready);
    state->current_time = current_time;
    return 0;
}