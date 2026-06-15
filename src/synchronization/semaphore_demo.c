#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int counter = 0;
HANDLE mutex;

DWORD WINAPI increment_thread(LPVOID lpParam) {
    int thread_id = *(int*)lpParam;
    
    for (int i = 0; i < 3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        counter++;
        printf("[线程 %d] 进入临界区, counter = %d\n", thread_id, counter);
        Sleep(100);
        printf("[线程 %d] 离开临界区\n", thread_id);
        ReleaseMutex(mutex);
        Sleep(50);
    }
    
    return 0;
}

int main() {
    printf("=== 信号量演示 (Windows Mutex) ===\n\n");
    
    mutex = CreateMutex(NULL, FALSE, NULL);
    
    HANDLE threads[5];
    int thread_ids[5];
    
    for (int i = 0; i < 5; i++) {
        thread_ids[i] = i + 1;
        threads[i] = CreateThread(NULL, 0, increment_thread, &thread_ids[i], 0, NULL);
    }
    
    WaitForMultipleObjects(5, threads, TRUE, INFINITE);
    
    for (int i = 0; i < 5; i++) {
        CloseHandle(threads[i]);
    }
    CloseHandle(mutex);
    
    printf("\n最终 counter = %d (期望: 15)\n", counter);
    
    return 0;
}
