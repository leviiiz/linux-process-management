#include <stdio.h>
#include <windows.h>

DWORD WINAPI child_process(LPVOID lpParam) {
    printf("[Child] Thread ID: %lu\n", GetCurrentThreadId());
    Sleep(1000);
    printf("[Child] Exiting\n");
    return 0;
}

int main() {
    printf("========================================\n");
    printf("  Process Control: fork() Demo\n");
    printf("========================================\n\n");

    HANDLE hThread = CreateThread(NULL, 0, child_process, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("ERROR: Failed to create thread\n");
        return 1;
    }

    printf("[Parent] PID: %lu, Created child handle: %p\n", 
           GetCurrentProcessId(), hThread);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    printf("[Parent] Child thread finished\n");
    return 0;
}
