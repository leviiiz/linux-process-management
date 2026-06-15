#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    printf("=== 进程控制：exec() 演示 ===\n\n");
    
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    
    printf("[父进程] 创建子进程执行 dir 命令...\n");
    
    BOOL success = CreateProcess(
        "C:\\Windows\\System32\\cmd.exe",
        "cmd.exe /c dir",
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
    );
    
    if (success) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        printf("[父进程] 子进程完成\n");
    } else {
        printf("创建进程失败，错误码: %lu\n", GetLastError());
    }
    
    return 0;
}
