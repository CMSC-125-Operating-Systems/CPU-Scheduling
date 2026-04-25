#ifndef GANTT_H
#define GANTT_H
#define MAX_ENTRIES 8192

typedef struct {
    char pid[16];
    int start;
    int end;
} GanttEntry;

typedef struct {
    GanttEntry entries[MAX_ENTRIES];
    int count;
    int capacity;
    int end_time;
} GanttChart;

//IMPLEMENTATION 
GanttChart *create_gantt();
void add_gantt(GanttChart *g, const char *pid, int start, int end);
void print_gantt(GanttChart *g);
int gantt_context_switches(const GanttChart *chart);
void free_gantt(GanttChart *g);

#endif