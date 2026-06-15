#include "scheduler.h"
#include <string.h>

// 按到达时间排序
void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void calculate_fcfs(Process processes[], int n) {
    // 按到达时间排序
    sort_by_arrival(processes, n);
    
    int current_time = 0;
    int gantt_order[100];
    int gantt_idx = 0;
    
    for (int i = 0; i < n; i++) {
        // 如果当前时间小于到达时间，CPU空闲
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        processes[i].start_time = current_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        
        gantt_order[gantt_idx++] = processes[i].pid;
        
        current_time = processes[i].completion_time;
    }
    
    printf("\n=== FCFS 调度算法 ===\n");
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
    
    calculate_fcfs(processes, n);
    
    return 0;
}
