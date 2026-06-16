#include "scheduler.h"
#include <string.h>

void sort_by_arrival(Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process t = p[j]; p[j] = p[j+1]; p[j+1] = t;
            }
}

void calculate_fcfs(Process p[], int n) {
    printf("\n=== FCFS Scheduling - Process State Change Log ===\n\n");

    for (int i = 0; i < n; i++) {
        p[i].state = NEW;
        printf("[Init] PID=%d State=%s Priority=%d Arrival=%d Burst=%d\n",
               p[i].pid, state_name(p[i].state), p[i].priority, 
               p[i].arrival_time, p[i].burst_time);
    }

    sort_by_arrival(p, n);

    int cur = 0, gantt[100], gi = 0;

    for (int i = 0; i < n; i++) {
        log_state_change(&p[i], p[i].state, READY);
        p[i].state = READY;

        if (cur < p[i].arrival_time) {
            printf("[CPU Log] Idle wait: %d -> %d\n", cur, p[i].arrival_time);
            cur = p[i].arrival_time;
        }

        log_state_change(&p[i], p[i].state, RUNNING);
        p[i].state = RUNNING;
        p[i].start_time = cur;
        printf("[Schedule] Selected PID=%d (Priority=%d) to run\n", 
               p[i].pid, p[i].priority);

        p[i].completion_time = cur + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        log_state_change(&p[i], p[i].state, TERMINATED);
        p[i].state = TERMINATED;
        printf("[Finish] PID=%d completed, Finish=%d, Wait=%d\n",
               p[i].pid, p[i].completion_time, p[i].waiting_time);

        gantt[gi++] = p[i].pid;
        cur = p[i].completion_time;
    }

    printf("\n=== FCFS Scheduling Results ===\n");
    print_processes(p, n);
    print_gantt(p, n, gantt, gi);
}

int main() {
    Process p[] = {
        {1, NEW, 0, 0, 5, 0, 0, 0, 0, 0},
        {2, NEW, 1, 1, 3, 0, 0, 0, 0, 0},
        {3, NEW, 2, 2, 8, 0, 0, 0, 0, 0},
        {4, NEW, 0, 3, 6, 0, 0, 0, 0, 0}
    };
    int n = sizeof(p) / sizeof(p[0]);
    calculate_fcfs(p, n);
    return 0;
}
