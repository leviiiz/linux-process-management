#include "scheduler.h"
#include <stdbool.h>

#define MAX_PROCESSES 100

void calculate_round_robin(Process processes[], int n, int quantum) {
    int remaining[MAX_PROCESSES];
    int gantt_order[200];
    int gantt_idx = 0;
    int current_time = 0;
    int completed = 0;
    bool first_execution[MAX_PROCESSES] = {false};
    
    // 初始化剩余时间
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
        processes[i].start_time = -1;
    }
    
    // 按到达时间排序
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
                int temp_rem = remaining[j];
                remaining[j] = remaining[j + 1];
                remaining[j + 1] = temp_rem;
            }
        }
    }
    
    // Round Robin 调度
    while (completed < n) {
        bool executed = false;
        
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && processes[i].arrival_time <= current_time) {
                executed = true;
                
                // 记录首次执行时间
                if (processes[i].start_time == -1) {
                    processes[i].start_time = current_time;
                }
                
                // 执行
                if (remaining[i] <= quantum) {
                    current_time += remaining[i];
                    remaining[i] = 0;
                    completed++;
                    
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                } else {
                    current_time += quantum;
                    remaining[i] -= quantum;
                }
                
                gantt_order[gantt_idx++] = processes[i].pid;
            }
        }
        
        // 如果没有进程可执行，时间前进
        if (!executed) {
            current_time++;
        }
    }
    
    printf("\n=== Round Robin 调度算法 (时间片=%d) ===\n", quantum);
    print_processes(processes, n);
    print_gantt_chart(processes, n, gantt_order, gantt_idx);
}

int main() {
    Process processes[] = {
        {1, 0, 5, 0, 0, 0, 0},
        {2, 1, 3, 0, 0, 0, 0},
        {3, 2, 8, 0, 0, 0, 0},
        {4, 3, 6, 0, 0, 0, 0}
    };
    
    int n = sizeof(processes) / sizeof(processes[0]);
    int quantum = 2;
    
    calculate_round_robin(processes, n, quantum);
    
    return 0;
}
