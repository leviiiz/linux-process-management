#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI child_process(LPVOID lpParam) {
    printf("[子进程] 线程ID: %lu\n", GetCurrentThreadId());
    Sleep(1000);
    printf("[子进程] 退出\n");
    return 0;
}

int main() {
    printf("=== 进程控制：fork() 演示 (Windows线程模拟) ===\n\n");
    
    HANDLE hThread = CreateThread(NULL, 0, child_process, NULL, 0, NULL);
    
    if (hThread == NULL) {
        printf("创建线程失败\n");
        return 1;
    }
    
    printf("[父进程] PID: %lu, 创建的子线程句柄: %p\n", GetCurrentProcessId(), hThread);
    
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    
    printf("[父进程] 子线程已结束\n");
    
    return 0;
}
