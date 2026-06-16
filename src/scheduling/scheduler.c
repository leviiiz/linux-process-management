#include "scheduler.h"

const char* state_name(ProcessState s) {
    switch(s) {
        case NEW:        return "NEW";
        case READY:      return "READY";
        case RUNNING:    return "RUNNING";
        case WAITING:    return "WAITING";
        case TERMINATED: return "TERMINATED";
        default:         return "UNKNOWN";
    }
}

void log_state_change(Process *p, ProcessState old_st, ProcessState new_st) {
    printf("[PCB Log] PID=%d: %s -> %s\n", p->pid, state_name(old_st), state_name(new_st));
}

float calc_avg_waiting(Process p[], int n) {
    float total = 0;
    for (int i = 0; i < n; i++) total += p[i].waiting_time;
    return total / n;
}

float calc_avg_turnaround(Process p[], int n) {
    float total = 0;
    for (int i = 0; i < n; i++) total += p[i].turnaround_time;
    return total / n;
}

void print_processes(Process p[], int n) {
    printf("\n%-6s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n",
           "PID", "Pri", "Arrive", "Burst", "State", "Finish", "Turn", "Wait");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-6d %-8d %-8d %-8d %-10s %-10d %-10d %-10d\n",
               p[i].pid, p[i].priority, p[i].arrival_time, p[i].burst_time,
               state_name(p[i].state), p[i].completion_time, 
               p[i].turnaround_time, p[i].waiting_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", calc_avg_waiting(p, n));
    printf("Average Turnaround Time: %.2f\n", calc_avg_turnaround(p, n));
}

void print_gantt(Process p[], int n, int *order, int size) {
    printf("\n=== Gantt Chart (Scheduling Order) ===\n");
    for (int i = 0; i < size; i++) printf("-------");
    printf("\n|");
    for (int i = 0; i < size; i++) printf(" P%-3d |", order[i]);
    printf("\n");
    for (int i = 0; i < size; i++) printf("-------");
    printf("\n");
}
