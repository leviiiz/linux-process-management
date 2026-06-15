#include <stdio.h>
#include <windows.h>

HANDLE mutex;
int counter;

DWORD WINAPI inc(LPVOID p) {
    for (int i = 0; i < 1000; i++) {
        WaitForSingleObject(mutex, INFINITE);
        counter++;
        ReleaseMutex(mutex);
    }
    return 0;
}

HANDLE sem_global;
HANDLE tm_global;
int cur_global, max_global;

DWORD WINAPI worker(LPVOID p) {
    WaitForSingleObject(sem_global, INFINITE);
    WaitForSingleObject(tm_global, INFINITE);
    cur_global++; if (cur_global > max_global) max_global = cur_global;
    ReleaseMutex(tm_global);
    Sleep(200);
    WaitForSingleObject(tm_global, INFINITE);
    cur_global--;
    ReleaseMutex(tm_global);
    ReleaseSemaphore(sem_global, 1, NULL);
    return 0;
}

DWORD WINAPI deadlock_t1(LPVOID p) {
    HANDLE m1 = (HANDLE)((HANDLE*)p)[0];
    HANDLE m2 = (HANDLE)((HANDLE*)p)[1];
    WaitForSingleObject(m1, INFINITE);
    Sleep(100);
    WaitForSingleObject(m2, INFINITE);
    ReleaseMutex(m2);
    ReleaseMutex(m1);
    return 0;
}

HANDLE stress_sem;
DWORD WINAPI stress_worker(LPVOID p) {
    WaitForSingleObject(stress_sem, INFINITE);
    Sleep(10);
    ReleaseSemaphore(stress_sem, 1, NULL);
    return 0;
}

HANDLE edge_sem;
int edge_shared;
DWORD WINAPI edge_worker(LPVOID p) {
    WaitForSingleObject(edge_sem, INFINITE);
    edge_shared++;
    ReleaseSemaphore(edge_sem, 1, NULL);
    return 0;
}

int test_mutex() {
    printf("[测试1] 互斥锁保护\n");
    counter = 0;
    mutex = CreateMutex(NULL, FALSE, NULL);
    HANDLE t[10];
    for (int i = 0; i < 10; i++) t[i] = CreateThread(NULL, 0, inc, NULL, 0, NULL);
    WaitForMultipleObjects(10, t, TRUE, INFINITE);
    for (int i = 0; i < 10; i++) CloseHandle(t[i]);
    CloseHandle(mutex);
    if (counter == 10000) { printf("  ✓ 通过 (counter=%d)\n", counter); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int test_sem() {
    printf("[测试2] 信号量并发限制\n");
    sem_global = CreateSemaphore(NULL, 3, 3, NULL);
    tm_global = CreateMutex(NULL, FALSE, NULL);
    cur_global = max_global = 0;
    HANDLE t[8];
    for (int i = 0; i < 8; i++) t[i] = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    WaitForMultipleObjects(8, t, TRUE, INFINITE);
    for (int i = 0; i < 8; i++) CloseHandle(t[i]);
    CloseHandle(sem_global); CloseHandle(tm_global);
    if (max_global <= 3) { printf("  ✓ 通过 (最大并发=%d)\n", max_global); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int test_deadlock() {
    printf("[测试3] 无死锁验证\n");
    HANDLE m1 = CreateMutex(NULL, FALSE, NULL);
    HANDLE m2 = CreateMutex(NULL, FALSE, NULL);
    HANDLE pair[2] = {m1, m2};
    HANDLE t[2];
    t[0] = CreateThread(NULL, 0, deadlock_t1, pair, 0, NULL);
    t[1] = CreateThread(NULL, 0, deadlock_t1, pair, 0, NULL);
    DWORD r = WaitForMultipleObjects(2, t, TRUE, 3000);
    CloseHandle(t[0]); CloseHandle(t[1]);
    CloseHandle(m1); CloseHandle(m2);
    if (r != WAIT_TIMEOUT) { printf("  ✓ 通过\n"); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int test_stress() {
    printf("[测试4] 压力测试\n");
    stress_sem = CreateSemaphore(NULL, 5, 5, NULL);
    HANDLE t[100];
    for (int i = 0; i < 100; i++) t[i] = CreateThread(NULL, 0, stress_worker, NULL, 0, NULL);
    WaitForMultipleObjects(100, t, TRUE, INFINITE);
    for (int i = 0; i < 100; i++) CloseHandle(t[i]);
    CloseHandle(stress_sem);
    printf("  ✓ 通过\n"); return 0;
}

int test_edge() {
    printf("[测试5] 边界条件\n");
    edge_sem = CreateSemaphore(NULL, 1, 1, NULL);
    edge_shared = 0;
    HANDLE t[2];
    t[0] = CreateThread(NULL, 0, edge_worker, NULL, 0, NULL);
    t[1] = CreateThread(NULL, 0, edge_worker, NULL, 0, NULL);
    WaitForMultipleObjects(2, t, TRUE, INFINITE);
    CloseHandle(t[0]); CloseHandle(t[1]);
    CloseHandle(edge_sem);
    if (edge_shared == 2) { printf("  ✓ 通过 (shared=%d)\n", edge_shared); return 0; }
    printf("  ✗ 失败\n"); return 1;
}

int main() {
    printf("=== 模块3 测试套件 ===\n\n");
    int p = 0, f = 0;
    if (test_mutex() == 0) p++; else f++;
    if (test_sem() == 0) p++; else f++;
    if (test_deadlock() == 0) p++; else f++;
    if (test_stress() == 0) p++; else f++;
    if (test_edge() == 0) p++; else f++;
    printf("\n=== 结果: %d/%d 通过 ===\n", p, p+f);
    return f > 0;
}
