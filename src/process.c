#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "process.h"

void init_process(Process *process, const char *pid, int arrival_time, int burst_time) {
    if (process == NULL || pid == NULL) {
        return;
    }

    strncpy(process->pid, pid, MAX_PID_LENGTH - 1);
    process->pid[MAX_PID_LENGTH - 1] = '\0';

    process->arrival_time = arrival_time;
    process->burst_time = burst_time;
    process->order = 0;

    reset_process(process);
}

void reset_process(Process *process) {
    if (process == NULL) {
        return;
    }

    process->remaining_time = process->burst_time;
    process->start_time = -1;
    process->finish_time = -1;
    process->waiting_time = 0;
    process->turnaround_time = 0;
    process->response_time = 0;
    process->priority = 0;
    process->time_in_queue = 0;
    process->completed = 0;
    process->queued = 0;

    if (process->burst_time == 0) {
        process->start_time = process->arrival_time;
        process->finish_time = process->arrival_time;
        process->completed = 1;
    } else {
        process->start_time = -1;
        process->finish_time = -1;
        process->completed = 0;
    }
}

void init_processes(Process processes[], int count) {
    int i;

    for (i = 0; i < count; i++) {
        processes[i].order = i;
        reset_process(&processes[i]);
    }
}

void reset_processes(Process processes[], int count) {
    int i;

    for (i = 0; i < count; i++) {
        reset_process(&processes[i]);
    }
}

void copy_processes(Process dest[], const Process src[], int count) {
    int i;

    for (i = 0; i < count; i++) {
        dest[i] = src[i];
    }
}

int find_process_index_by_pid(const Process processes[], int count, const char *pid) {
    int i;

    for (i = 0; i < count; i++) {
        if (strcmp(processes[i].pid, pid) == 0) {
            return i;
        }
    }

    return -1;
}

int load_processes_from_stream(FILE *stream, Process processes[], int max_processes) {
    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), stream) != NULL && count < max_processes) {
        char pid[MAX_PID_LENGTH];
        int arrival_time;
        int burst_time;

        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        if (sscanf(line, "%15s %d %d", pid, &arrival_time, &burst_time) == 3) {
            init_process(&processes[count], pid, arrival_time, burst_time);
            processes[count].order = count;
            count++;
        }
    }

    return count;
}

int load_processes_from_file(const char *path, Process processes[], int max_processes) {
    FILE *file;
    int count;

    file = fopen(path, "r");
    if (file == NULL) {
        perror("input file");
        return -1;
    }

    count = load_processes_from_stream(file, processes, max_processes);
    fclose(file);

    return count;
}

int load_processes_from_inline(const char *spec, Process processes[], int max_processes) {
    char buffer[2048];
    char *token;
    int count = 0;

    if (spec == NULL) {
        return -1;
    }

    strncpy(buffer, spec, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    token = strtok(buffer, ",");

    while (token != NULL && count < max_processes) {
        char pid[MAX_PID_LENGTH];
        int arrival_time;
        int burst_time;

        while (isspace((unsigned char)*token)) {
            token++;
        }

        if (sscanf(token, "%15[^:]:%d:%d", pid, &arrival_time, &burst_time) == 3) {
            init_process(&processes[count], pid, arrival_time, burst_time);
            processes[count].order = count;
            count++;
        }

        token = strtok(NULL, ",");
    }

    return count;
}