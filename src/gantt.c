//gantt.c 




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/gantt.h"

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
    chart->capacity = 64;
    chart->count = 0;
    chart->entries = (GanttEntry *)malloc(chart->capacity * sizeof(GanttEntry));
    if (chart->entries == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for Gantt entries\n");
        free(chart);
        return NULL;
    }
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

printf("Adding entry to Gantt chart...\n");

void add_gantt(GanttChart *chart, const char *pid, int start, int end) {
    if (chart == NULL || pid == NULL) {
        fprintf(stderr, "Error: Chart or PID is NULL\n");
        return;
    }
    //Merge check
    if (chart->count > 0) {
        GanttEntry *last = &chart->entries[chart->count - 1];
        if (strcmp(last->pid, pid) == 0 && last->end == start) {
            last->end = end; //Extend existing bar, no need to add new
            return;
        }
    }
    //Grow if needed
    if (chart->count >= chart->capacity) {
        chart->capacity *= 2;
        GanttEntry *new_entries = (GanttEntry *)realloc(chart->entries, chart->capacity * sizeof(GanttEntry));
        if (new_entries == NULL) {
            fprintf(stderr, "Error: Could not reallocate memory for Gantt entries\n");
            return;
        }
        chart->entries = new_entries;
    }
    //Append new entry
    strncpy(chart->entries[chart->count].pid, pid, sizeof(chart->entries[chart->count].pid) - 1); //Copy PID with safety
    chart->entries[chart->count].pid[sizeof(chart->entries[chart->count].pid) - 1] = '\0'; // Ensure null-termination
    chart->entries[chart->count].start = start;
    chart->entries[chart->count].end = end;
    chart->count++;
}

// print_gantt(chart):
//     print top border  (+---+---+)
//     print PID row     (| A | B |)
//     print bot border  (+---+---+)
//     print time labels (0   10  20)

printf("Printing Gantt chart...\n");

void print_gantt(const GanttChart *chart) {
    if (chart == NULL || chart->count == 0) { //No entries to print
        printf("Gantt chart is empty.\n");
        return;
    }
    // Print top border
    for (int i = 0; i < chart->count; i++) {
        printf("+---");
    }
    printf("+\n");
    // Print PID row
    for (int i = 0; i < chart->count; i++) {
        printf("| %s ", chart->entries[i].pid);
    }
    printf("|\n");
    // Print bottom border
    for (int i = 0; i < chart->count; i++) {
        printf("+---");
    }
    printf("+\n");
    // Print time labels
    for (int i = 0; i < chart->count; i++) {
        printf("%-4d", chart->entries[i].start);
    }
    printf("%-4d\n", chart->entries[chart->count - 1].end); // Last end time
}

// free_gantt(chart):
//     free entries array
//     free chart struct

printf("Freeing Gantt chart...\n");

void free_gantt(GanttChart *chart) {
    if (chart) {
        free(chart->entries);
        free(chart);
    }
}