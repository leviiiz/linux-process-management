#include "scheduler.h"

float calculate_avg_waiting_time(Process processes[], int n) {
    float total_waiting = 0;
    for (int i = 0; i < n; i++) {
        total_waiting += processes[i].waiting_time;
    }
    return total_waiting / n;
}

float calculate_avg_turnaround_time(Process processes[], int n) {
    float total_turnaround = 0;
    for (int i = 0; i < n; i++) {
        total_turnaround += processes[i].turnaround_time;
    }
    return total_turnaround / n;
}

void print_processes(Process processes[], int n) {
    printf("\n%-6s %-12s %-10s %-15s %-15s %-12s\n",
           "PID", "到达时间", "执行时间", "完成时间", "周转时间", "等待时间");
    printf("----------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-6d %-12d %-10d %-15d %-15d %-12d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);
    }
    
    printf("\n平均等待时间: %.2f\n", calculate_avg_waiting_time(processes, n));
    printf("平均周转时间: %.2f\n", calculate_avg_turnaround_time(processes, n));
}

void print_gantt_chart(Process processes[], int n, int *gantt_order, int gantt_size) {
    printf("\n=== Gantt 图 ===\n");
    
    // 打印进程块
    for (int i = 0; i < gantt_size; i++) {
        printf("-------");
    }
    printf("\n|");
    
    for (int i = 0; i < gantt_size; i++) {
        printf(" P%-3d |", gantt_order[i]);
    }
    printf("\n");
    
    for (int i = 0; i < gantt_size; i++) {
        printf("-------");
    }
    printf("\n");
}
