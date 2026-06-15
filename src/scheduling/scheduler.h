#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>

// 进程控制块 (PCB)
typedef struct {
    int pid;                // 进程ID
    int arrival_time;       // 到达时间
    int burst_time;         // CPU执行时间
    int remaining_time;     // 剩余执行时间
    int completion_time;    // 完成时间
    int turnaround_time;    // 周转时间
    int waiting_time;       // 等待时间
    int start_time;         // 开始执行时间
} Process;

// 计算平均等待时间
float calculate_avg_waiting_time(Process processes[], int n);

// 计算平均周转时间
float calculate_avg_turnaround_time(Process processes[], int n);

// FCFS 调度算法
void calculate_fcfs(Process processes[], int n);

// Round Robin 调度算法
void calculate_round_robin(Process processes[], int n, int quantum);

// 打印进程信息
void print_processes(Process processes[], int n);

// 打印 Gantt 图
void print_gantt_chart(Process processes[], int n, int *gantt_order, int gantt_size);

#endif
