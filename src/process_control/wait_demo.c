#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI child_work(LPVOID lpParam) {
    printf("[子进程] 线程ID: %lu, 执行中...\n", GetCurrentThreadId());
    Sleep(2000);
    printf("[子进程] 退出，返回状态码 42\n");
    return 42;
}

int main() {
    printf("=== 进程控制：wait() 演示 ===\n\n");
    
    HANDLE hThread = CreateThread(NULL, 0, child_work, NULL, 0, NULL);
    
    if (hThread == NULL) {
        printf("创建线程失败\n");
        return 1;
    }
    
    printf("[父进程] 等待子线程结束...\n");
    WaitForSingleObject(hThread, INFINITE);
    
    DWORD exitCode;
    GetExitCodeThread(hThread, &exitCode);
    printf("[父进程] 子线程已结束, 退出码: %lu\n", exitCode);
    
    CloseHandle(hThread);
    
    return 0;
}
