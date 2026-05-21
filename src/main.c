#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/gantt.h"
#include "../include/metrics.h"

#define DEFAULT_QUANTUM 30

typedef enum {
    ALG_FCFS,
    ALG_SJF,
    ALG_STCF,
    ALG_RR,
    ALG_MLFQ
} Algorithm;

static int parse_algorithm(const char *name, Algorithm *algorithm) {
    if (strcmp(name, "FCFS") == 0) {
        *algorithm = ALG_FCFS;
    } else if (strcmp(name, "SJF") == 0) {
        *algorithm = ALG_SJF;
    } else if (strcmp(name, "STCF") == 0) {
        *algorithm = ALG_STCF;
    } else if (strcmp(name, "RR") == 0) {
        *algorithm = ALG_RR;
    } else if (strcmp(name, "MLFQ") == 0) {
        *algorithm = ALG_MLFQ;
    } else {
        return -1;
    }

    return 0;
}

static const char *algorithm_name(Algorithm algorithm) {
    switch (algorithm) {
        case ALG_FCFS: return "FCFS";
        case ALG_SJF: return "SJF";
        case ALG_STCF: return "STCF";
        case ALG_RR: return "RR";
        case ALG_MLFQ: return "MLFQ";
        default: return "UNKNOWN";
    }
}

static void usage(const char *program) {
    printf("Usage:\n");
    printf("  %s --algorithm=FCFS --input=workload.txt\n", program);
    printf("  %s --algorithm=RR --quantum=30 --input=workload.txt\n", program);
    printf("  %s --algorithm=MLFQ --input=workload.txt\n", program);
    printf("  %s --compare --input=workload.txt\n", program);
    printf("  %s --algorithm=STCF --processes=\"A:0:5,B:2:3\"\n", program);
}

static void default_mlfq_config(MLFQConfig *config) {
    config->levels = 3;

    config->quantums[0] = 10;
    config->allotments[0] = 50;

    config->quantums[1] = 30;
    config->allotments[1] = 150;

    config->quantums[2] = -1;
    config->allotments[2] = -1;

    config->boost_period = 200;
}

static int run_algorithm(Algorithm algorithm, Process original[], int count, int quantum, int print_full) {
    Process working[MAX_PROCESSES];
    GanttChart *chart;
    SchedulerState state;
    Metrics metrics;
    MLFQConfig mlfq_config;
    int status = 0;

    copy_processes(working, original, count);
    reset_processes(working, count);

    chart = create_gantt();
    if (chart == NULL) {
        return -1;
    }

    state.processes = working;
    state.num_processes = count;
    state.current_time = 0;
    state.chart = chart;

    if (print_full) {
        printf("\nRunning %s Scheduler...\n", algorithm_name(algorithm));
    }

    switch (algorithm) {
        case ALG_FCFS:
            status = schedule_fcfs(&state);
            break;
        case ALG_SJF:
            status = schedule_sjf(&state);
            break;
        case ALG_STCF:
            status = schedule_stcf(&state);
            break;
        case ALG_RR:
            if (print_full) {
                printf("Using time quantum q=%d\n", quantum);
            }
            status = schedule_rr(&state, quantum);
            break;
        case ALG_MLFQ:
            default_mlfq_config(&mlfq_config);
            status = schedule_mlfq(&state, &mlfq_config);
            break;
        default:
            status = -1;
            break;
    }

    if (status != 0) {
        free_gantt(chart);
        return status;
    }

    compute_metrics(working, count, state.current_time, &metrics);

    if (print_full) {
        print_gantt(chart);
        print_metrics(working, count, &metrics);
        printf("Context Switches: %d\n", gantt_context_switches(chart));
    } else {
        printf("%-10s | %8.2f | %8.2f | %8.2f | %16d\n",
               algorithm_name(algorithm),
               metrics.avg_turnaround,
               metrics.avg_waiting,
               metrics.avg_response,
               gantt_context_switches(chart));
    }

    free_gantt(chart);
    return 0;
}

static int run_compare(Process processes[], int count, int quantum) {
    printf("\n=== Algorithm Comparison ===\n");
    printf("%-10s | %8s | %8s | %8s | %16s\n",
           "Algorithm", "Avg TT", "Avg WT", "Avg RT", "Context Switches");
    printf("-----------------------------------------------------------------\n");

    run_algorithm(ALG_FCFS, processes, count, quantum, 0);
    run_algorithm(ALG_SJF, processes, count, quantum, 0);
    run_algorithm(ALG_STCF, processes, count, quantum, 0);
    run_algorithm(ALG_RR, processes, count, quantum, 0);
    run_algorithm(ALG_MLFQ, processes, count, quantum, 0);

    return 0;
}

int main(int argc, char *argv[]) {
    Process processes[MAX_PROCESSES];
    int count = 0;
    int quantum = DEFAULT_QUANTUM;
    int compare = 0;
    int algorithm_set = 0;
    Algorithm algorithm = ALG_FCFS;
    const char *input_path = NULL;
    const char *inline_processes = NULL;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            if (parse_algorithm(argv[i] + 12, &algorithm) != 0) {
                fprintf(stderr, "Unknown algorithm: %s\n", argv[i] + 12);
                return 1;
            }
            algorithm_set = 1;
        } else if (strncmp(argv[i], "--input=", 8) == 0) {
            input_path = argv[i] + 8;
        } else if (strncmp(argv[i], "--processes=", 12) == 0) {
            inline_processes = argv[i] + 12;
        } else if (strncmp(argv[i], "--quantum=", 10) == 0) {
            quantum = atoi(argv[i] + 10);
        } else if (strcmp(argv[i], "--compare") == 0) {
            compare = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (input_path != NULL) {
        count = load_processes_from_file(input_path, processes, MAX_PROCESSES);
    } else if (inline_processes != NULL) {
        count = load_processes_from_inline(inline_processes, processes, MAX_PROCESSES);
    } else {
        count = load_processes_from_stream(stdin, processes, MAX_PROCESSES);
    }

    if (count <= 0) {
        fprintf(stderr, "No processes loaded.\n");
        return 1;
    }

    init_processes(processes, count);

    if (compare) {
        return run_compare(processes, count, quantum);
    }

    if (!algorithm_set) {
        fprintf(stderr, "No algorithm specified. Defaulting to FCFS.\n");
    }

    return run_algorithm(algorithm, processes, count, quantum, 1);
}