#include <stdio.h>

#ifndef GANTT_H
#define GANTT_H


typedef struct {
    char pid[16];
    int start;
    int end;
} GanttEntry;

typedef struct {
    GanttEntry *entries;
    int count;
    int capacity;
} GanttChart;
//IMPLEMENTATION 

#endif