//metrics.c

// compute(processes, count, total time, metrics):
//     total_tt = total_wt = total_rt = 0

//     for each process:
//         TT = completion_time - arrival_time
//         WT = TT - burst_time          ← spec formula, not just queue time
//         RT = start_time - arrival_time ← first execution, not arrival

//         total_tt += TT
//         total_wt += WT
//         total_rt += RT
// print(processes, count, metrics)
//     print table header
//     for each process → print row

//     print Average TT = total_tt / n
//     print Average WT = total_wt / n
//     print Average RT = total_rt / n

#include "metrics.h"
#include "process.h"
#include <stdio.h>

void compute_metrics(const Process processes[], int count, int total_time, Metrics *metrics) {
    int i;
    double total_tt = 0;
    double total_wt = 0;
    double total_rt = 0;

    for (i = 0; i < count; i++) {
        int tt = processes[i].finish_time - processes[i].arrival_time;
        int wt = tt - processes[i].burst_time;
        int rt = processes[i].start_time - processes[i].arrival_time;

        total_tt += tt;
        total_wt += wt;
        total_rt += rt;
    }

    metrics->avg_turnaround = total_tt / count;
    metrics->avg_waiting = total_wt / count;
    metrics->avg_response = total_rt / count;
    metrics->throughput = (double)count / total_time;
}

void print_metrics(const Process processes[], int count, const Metrics *metrics) {
    int i;

    printf("PID\tArrival\tBurst\tStart\tFinish\tTurnaround\tWaiting\tResponse\n");
    for (i = 0; i < count; i++) {
        int tt = processes[i].finish_time - processes[i].arrival_time;
        int wt = tt - processes[i].burst_time;
        int rt = processes[i].start_time - processes[i].arrival_time;

        printf("%s\t%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].start_time,
               processes[i].finish_time,
               tt,
               wt,
               rt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", metrics->avg_turnaround);
    printf("Average Waiting Time: %.2f\n", metrics->avg_waiting);
    printf("Average Response Time: %.2f\n", metrics->avg_response);
    printf("Throughput: %.2f processes/unit time\n", metrics->throughput);
}

