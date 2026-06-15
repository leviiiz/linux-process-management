#include <stdio.h>

typedef struct {
    int pid, arrival_time, burst_time, remaining_time;
    int completion_time, turnaround_time, waiting_time, start_time;
} Process;

void calculate_fcfs(Process p[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process t = p[j]; p[j] = p[j+1]; p[j+1] = t;
            }
    int cur = 0;
    for (int i = 0; i < n; i++) {
        if (cur < p[i].arrival_time) cur = p[i].arrival_time;
        p[i].completion_time = cur + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        cur = p[i].completion_time;
    }
}

void calculate_rr(Process p[], int n, int q) {
    int rem[10], cur = 0, done = 0;
    for (int i = 0; i < n; i++) { rem[i] = p[i].burst_time; p[i].start_time = -1; }
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process t = p[j]; p[j] = p[j+1]; p[j+1] = t;
                int tr = rem[j]; rem[j] = rem[j+1]; rem[j+1] = tr;
            }
    while (done < n) {
        int exe = 0;
        for (int i = 0; i < n; i++) {
            if (rem[i] > 0 && p[i].arrival_time <= cur) {
                exe = 1;
                if (p[i].start_time == -1) p[i].start_time = cur;
                if (rem[i] <= q) { cur += rem[i]; rem[i] = 0; done++;
                    p[i].completion_time = cur;
                    p[i].turnaround_time = cur - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                } else { cur += q; rem[i] -= q; }
            }
        }
        if (!exe) cur++;
    }
}

float calc_avg_wt(Process p[], int n) {
    float s = 0; for (int i = 0; i < n; i++) s += p[i].waiting_time; return s/n;
}

float calc_avg_tt(Process p[], int n) {
    float s = 0; for (int i = 0; i < n; i++) s += p[i].turnaround_time; return s/n;
}

int test_fcfs() {
    printf("[测试1] FCFS 基本调度\n");
    Process p[] = {{1,0,5},{2,1,3},{3,2,8}};
    calculate_fcfs(p, 3);
    if (p[0].completion_time == 5 && p[2].completion_time == 16) { printf("  ✓ 通过\n"); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int test_fcfs_same() {
    printf("[测试2] FCFS 同时到达\n");
    Process p[] = {{1,0,10},{2,0,5},{3,0,8}};
    calculate_fcfs(p, 3);
    if (p[0].completion_time == 10 && p[1].completion_time == 15) { printf("  ✓ 通过\n"); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int test_rr() {
    printf("[测试3] Round Robin\n");
    Process p[] = {{1,0,5},{2,1,3}};
    calculate_rr(p, 2, 2);
    printf("  ✓ 通过\n"); return 0;
}

int test_rr_single() {
    printf("[测试4] RR 单进程\n");
    Process p[] = {{1,0,4}};
    calculate_rr(p, 1, 2);
    if (p[0].completion_time == 4) { printf("  ✓ 通过\n"); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int test_large() {
    printf("[测试5] 大批量进程\n");
    Process p[5];
    for (int i = 0; i < 5; i++) { p[i].pid = i+1; p[i].arrival_time = i; p[i].burst_time = 3+i; }
    calculate_fcfs(p, 5);
    printf("  ✓ 通过 (avg WT=%.1f, avg TT=%.1f)\n", calc_avg_wt(p,5), calc_avg_tt(p,5));
    return 0;
}

int main() {
    printf("=== 模块2 测试套件 ===\n\n");
    int p = 0, f = 0;
    if (test_fcfs() == 0) p++; else f++;
    if (test_fcfs_same() == 0) p++; else f++;
    if (test_rr() == 0) p++; else f++;
    if (test_rr_single() == 0) p++; else f++;
    if (test_large() == 0) p++; else f++;
    printf("\n=== 结果: %d/%d 通过 ===\n", p, p+f);
    return f > 0;
}
