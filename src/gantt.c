// gantt.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gantt.h"


// create_gantt:
//     allocate GanttChart
//     allocate entries array (capacity=64)
//     count = 0
//     return chart

printf("Creating Gantt chart...\n");

GanttChart *create_gantt() {
    GanttChart *chart = (GanttChart *)malloc(sizeof(GanttChart));

    if (chart == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for Gantt chart\n");
        return NULL;
    }

    chart->count = 0;
    chart->capacity = MAX_ENTRIES;
    chart->end_time = 0;

    return chart;
}

// add_gantt(chart, pid, start, end):
//     ── MERGE CHECK ──
//     if count > 0:
//         last = entries[count-1]
//         if last.pid == pid AND last.end == start:
//             last.end = end       ← extend existing bar, don't add new
//             return
//     ── GROW if needed ──
//     if count >= capacity:
//         capacity *= 2
//         realloc entries array
//     ── APPEND ──
//     entries[count] = {pid, start, end}
//     count++

printf("Adding entry to Gantt chart: PID=%s, start=%d, end=%d\n", pid, start, end);

void add_gantt(GanttChart *chart, const char *pid, int start, int end) {
    GanttEntry *last;

    if (chart == NULL || pid == NULL) {
        fprintf(stderr, "Error: Chart or PID is NULL\n");
        return;
    }

    if (chart->count > 0) {
        last = &chart->entries[chart->count - 1];
        if (strcmp(last->pid, pid) == 0 && last->end == start) {
            last->end = end;
            chart->end_time = end;
            return;
        }
    }

    if (chart->count >= chart->capacity) {
        fprintf(stderr, "Error: Gantt chart is full\n");
        return;
    }

    strncpy(chart->entries[chart->count].pid, pid,
            sizeof(chart->entries[chart->count].pid) - 1);
    chart->entries[chart->count].pid[sizeof(chart->entries[chart->count].pid) - 1] = '\0';
    chart->entries[chart->count].start = start;
    chart->entries[chart->count].end = end;
    chart->count++;
    chart->end_time = end;
}

int gantt_context_switches(const GanttChart *chart) {
    int i;
    int switches = 0;

    if (chart == NULL) {
        return 0;
    }

    for (i = 1; i < chart->count; i++) {
        if (strcmp(chart->entries[i - 1].pid, "IDLE") == 0 ||
            strcmp(chart->entries[i].pid, "IDLE") == 0) {
            continue;
        }
        switches++;
    }

    return switches;
}

// print_gantt(chart):
//     print top border  (+---+---+)
//     print PID row     (| A | B |)
//     print bot border  (+---+---+)
//     print time labels (0   10  20)

printf("Printing Gantt chart...\n");
void print_gantt(GanttChart *chart) {
    int i;

    if (chart == NULL || chart->count == 0) {
        printf("Gantt chart is empty.\n");
        return;
    }

    for (i = 0; i < chart->count; i++) {
        printf("+---");
    }
    printf("+\n");

    for (i = 0; i < chart->count; i++) {
        printf("| %s ", chart->entries[i].pid);
    }
    printf("|\n");

    for (i = 0; i < chart->count; i++) {
        printf("+---");
    }
    printf("+\n");

    for (i = 0; i < chart->count; i++) {
        printf("%-4d", chart->entries[i].start);
    }
    printf("%-4d\n", chart->entries[chart->count - 1].end);
}

printf("Freeing Gantt chart...\n");

void free_gantt(GanttChart *chart) {
    free(chart);
}
