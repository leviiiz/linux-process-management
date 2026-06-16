#include <stdio.h>
#include <windows.h>

DWORD WINAPI child_work(LPVOID lpParam) {
    printf("[Child] Thread ID: %lu, running...\n", GetCurrentThreadId());
    Sleep(2000);
    printf("[Child] Exiting with status code 42\n");
    return 42;
}

int main() {
    printf("========================================\n");
    printf("  Process Control: wait() Demo\n");
    printf("========================================\n\n");

    HANDLE hThread = CreateThread(NULL, 0, child_work, NULL, 0, NULL);

    if (hThread == NULL) {
        printf("ERROR: Failed to create thread\n");
        return 1;
    }

    printf("[Parent] Waiting for child thread...\n");

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode;
    GetExitCodeThread(hThread, &exitCode);
    printf("[Parent] Child finished, exit code: %lu\n", exitCode);

    CloseHandle(hThread);
    return 0;
}
