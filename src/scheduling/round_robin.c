#include "scheduler.h"
#include <stdbool.h>

void calculate_rr(Process p[], int n, int q) {
    printf("\n=== Round Robin Scheduling (Quantum=%d) - State Change Log ===\n\n", q);

    int rem[100], cur = 0, done = 0, gantt[200], gi = 0;

    for (int i = 0; i < n; i++) {
        rem[i] = p[i].burst_time;
        p[i].state = NEW;
        p[i].start_time = -1;
        printf("[Init] PID=%d State=%s Priority=%d Arrival=%d Burst=%d\n",
               p[i].pid, state_name(p[i].state), p[i].priority,
               p[i].arrival_time, p[i].burst_time);
    }

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process t = p[j]; p[j] = p[j+1]; p[j+1] = t;
                int tr = rem[j]; rem[j] = rem[j+1]; rem[j+1] = tr;
            }

    printf("\n=== Start Round Robin ===\n\n");

    while (done < n) {
        int exe = 0;
        for (int i = 0; i < n; i++) {
            if (rem[i] > 0 && p[i].arrival_time <= cur) {
                exe = 1;
                if (p[i].start_time == -1) p[i].start_time = cur;

                if (p[i].state != RUNNING) {
                    log_state_change(&p[i], p[i].state, RUNNING);
                    p[i].state = RUNNING;
                }

                printf("[Schedule] Time=%d: PID=%d executes ", cur, p[i].pid);

                if (rem[i] <= q) {
                    printf("%d units (LAST)\n", rem[i]);
                    cur += rem[i]; rem[i] = 0; done++;
                    p[i].completion_time = cur;
                    p[i].turnaround_time = cur - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                    log_state_change(&p[i], p[i].state, TERMINATED);
                    p[i].state = TERMINATED;
                    printf("[Finish] PID=%d done! Completion=%d\n", p[i].pid, cur);
                } else {
                    printf("%d units (Remaining=%d)\n", q, rem[i] - q);
                    cur += q; rem[i] -= q;
                    log_state_change(&p[i], p[i].state, READY);
                    p[i].state = READY;
                }
                gantt[gi++] = p[i].pid;
            }
        }
        if (!exe) {
            printf("[CPU Log] Time=%d: Idle\n", cur);
            cur++;
        }
    }

    printf("\n=== Round Robin Scheduling Results ===\n");
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
    calculate_rr(p, n, 2);
    return 0;
}
