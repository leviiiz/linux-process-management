#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdio.h>
#include <stdlib.h>

typedef enum { NEW, READY, RUNNING, WAITING, TERMINATED } ProcessState;

typedef struct {
    int pid;
    ProcessState state;
    int priority;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int start_time;
} Process;

const char* state_name(ProcessState s);
void log_state_change(Process *p, ProcessState old_st, ProcessState new_st);
float calc_avg_waiting(Process p[], int n);
float calc_avg_turnaround(Process p[], int n);
void calculate_fcfs(Process p[], int n);
void calculate_rr(Process p[], int n, int quantum);
void print_processes(Process p[], int n);
void print_gantt(Process p[], int n, int *order, int size);
#endif
