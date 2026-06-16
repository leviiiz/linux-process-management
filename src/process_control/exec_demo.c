#include <stdio.h>
#include <windows.h>

int main() {
    printf("========================================\n");
    printf("  Process Control: exec() Demo\n");
    printf("========================================\n\n");

    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;

    printf("[Parent] Creating child process to run 'dir'...\n");

    BOOL success = CreateProcess(
        "C:\\Windows\\System32\\cmd.exe",
        "cmd.exe /c dir",
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
    );

    if (success) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        printf("[Parent] Child process completed\n");
    } else {
        printf("ERROR: CreateProcess failed (code: %lu)\n", GetLastError());
    }

    return 0;
}
