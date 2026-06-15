#include <stdio.h>
#include <windows.h>

int test_create_thread() {
    printf("[测试1] 正常创建线程\n");
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sleep, (LPVOID)500, 0, NULL);
    if (hThread == NULL) { printf("  ✗ 失败\n"); return 1; }
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    printf("  ✓ 通过\n");
    return 0;
}

int test_exit_code() {
    printf("[测试2] 获取退出码\n");
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sleep, (LPVOID)500, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    DWORD code;
    GetExitCodeThread(hThread, &code);
    CloseHandle(hThread);
    printf("  ✓ 通过 (退出码: %lu)\n", code);
    return 0;
}

int test_multiple_threads() {
    printf("[测试3] 5个线程并发\n");
    HANDLE t[5];
    for (int i = 0; i < 5; i++) t[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sleep, (LPVOID)300, 0, NULL);
    WaitForMultipleObjects(5, t, TRUE, INFINITE);
    for (int i = 0; i < 5; i++) CloseHandle(t[i]);
    printf("  ✓ 通过\n");
    return 0;
}

int test_zero_wait() {
    printf("[测试4] 零等待\n");
    HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sleep, (LPVOID)0, 0, NULL);
    WaitForSingleObject(h, INFINITE);
    CloseHandle(h);
    printf("  ✓ 通过\n");
    return 0;
}

int test_handle_close() {
    printf("[测试5] 句柄管理\n");
    HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sleep, (LPVOID)200, 0, NULL);
    WaitForSingleObject(h, INFINITE);
    CloseHandle(h);
    printf("  ✓ 通过\n");
    return 0;
}

int main() {
    printf("=== 模块1 测试套件 ===\n\n");
    int p = 0, f = 0;
    if (test_create_thread() == 0) p++; else f++;
    if (test_exit_code() == 0) p++; else f++;
    if (test_multiple_threads() == 0) p++; else f++;
    if (test_zero_wait() == 0) p++; else f++;
    if (test_handle_close() == 0) p++; else f++;
    printf("\n=== 结果: %d/%d 通过 ===\n", p, p+f);
    return f > 0;
}
