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

static void enqueue_arrivals(Process processes[], int count, Queue *queues[], int current_time) {
    for (int i = 0; i < count; i++) {
        if (!processes[i].completed &&
            !processes[i].queued &&
            processes[i].arrival_time == current_time) {
            processes[i].priority = 0;
            processes[i].time_in_queue = 0;
            enqueue(queues[0], &processes[i]);
            processes[i].queued = 1;

            printf("t=%d: Process %s enters Q0\n", current_time, processes[i].pid);
        }
    }
}

static void priority_boost(Queue *queues[], int levels) {
    Queue *boosted = create_queue();

    if (boosted == NULL) {
        return;
    }

    for (int level = 0; level < levels; level++) {
        while (queues[level]->size > 0) {
            Process *p = dequeue(queues[level]);
            p->queued = 0;

            if (!p->completed && p->remaining_time > 0) {
                p->priority = 0;
                p->time_in_queue = 0;
                enqueue(boosted, p);
                p->queued = 1;
            }
        }
    }

    while (boosted->size > 0) {
        Process *p = dequeue(boosted);
        enqueue(queues[0], p);
        p->queued = 1;
    }

    free_queue(boosted);
}

static int highest_nonempty_queue(Queue *queues[], int levels) {
    for (int level = 0; level < levels; level++) {
        if (queues[level]->size > 0) {
            return level;
        }
    }

    return -1;
}

static int has_higher_priority_work(Queue *queues[], int current_level) {
    for (int level = 0; level < current_level; level++) {
        if (queues[level]->size > 0) {
            return 1;
        }
    }

    return 0;
}

int schedule_mlfq(SchedulerState *state, MLFQConfig *config) {
    Queue *queues[8];
    int current_time = 0;
    int last_boost = 0;

    if (state == NULL || state->processes == NULL || state->chart == NULL || config == NULL) {
        return -1;
    }

    if (config->levels < 3) {
        config->levels = 3;
    }

    if (config->levels > 8) {
        config->levels = 8;
    }

    for (int i = 0; i < config->levels; i++) {
        queues[i] = create_queue();

        if (queues[i] == NULL) {
            return -1;
        }
    }

    printf("\n=== MLFQ Configuration ===\n");
    for (int i = 0; i < config->levels; i++) {
        if (config->quantums[i] <= 0) {
            printf("Queue %d: FCFS-like lowest queue\n", i);
        } else {
            printf("Queue %d: quantum=%d, allotment=%d\n",
                   i, config->quantums[i], config->allotments[i]);
        }
    }
    printf("Boost period: %d\n", config->boost_period);

    printf("\n=== Execution Trace ===\n");

    while (completed_count(state->processes, state->num_processes) < state->num_processes) {
        enqueue_arrivals(state->processes, state->num_processes, queues, current_time);

        if (config->boost_period > 0 &&
            current_time > 0 &&
            current_time - last_boost >= config->boost_period) {
            printf("t=%d: Priority boost, ready processes moved to Q0\n", current_time);
            priority_boost(queues, config->levels);
            last_boost = current_time;
        }

        int level = highest_nonempty_queue(queues, config->levels);

        if (level == -1) {
            add_gantt(state->chart, "IDLE", current_time, current_time + 1);
            current_time++;
            continue;
        }

        Process *p = dequeue(queues[level]);
        p->queued = 0;

        if (p->start_time == -1) {
            p->start_time = current_time;
            p->response_time = p->start_time - p->arrival_time;
        }

        int quantum = config->quantums[level];
        int allotment = config->allotments[level];
        int used_this_turn = 0;
        int boosted_this_turn = 0;

        while (p->remaining_time > 0 && (quantum <= 0 || used_this_turn < quantum)) {
            add_gantt(state->chart, p->pid, current_time, current_time + 1);

            p->remaining_time--;
            p->time_in_queue++;
            used_this_turn++;
            current_time++;

            enqueue_arrivals(state->processes, state->num_processes, queues, current_time);

            if (p->remaining_time == 0) {
                break;
            }

            if (config->boost_period > 0 &&
                current_time > 0 &&
                current_time - last_boost >= config->boost_period) {
                printf("t=%d: Priority boost, ready processes moved to Q0\n", current_time);
                priority_boost(queues, config->levels);
                p->priority = 0;
                p->time_in_queue = 0;
                last_boost = current_time;
                boosted_this_turn = 1;
                break;
            }

            if (has_higher_priority_work(queues, p->priority)) {
                break;
            }

            if (allotment > 0 && p->time_in_queue >= allotment) {
                break;
            }
        }

        if (p->remaining_time == 0) {
            p->finish_time = current_time;
            p->turnaround_time = p->finish_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->completed = 1;

            printf("t=%d: Process %s completes\n", current_time, p->pid);
        } else {
            if (!boosted_this_turn && allotment > 0 && p->time_in_queue >= allotment && p->priority < config->levels - 1) {
                p->priority++;
                p->time_in_queue = 0;

                printf("t=%d: Process %s demoted to Q%d\n",
                       current_time, p->pid, p->priority);
            }

            enqueue(queues[p->priority], p);
            p->queued = 1;
        }
    }

    for (int i = 0; i < config->levels; i++) {
        free_queue(queues[i]);
    }

    state->current_time = current_time;
    return 0;
}