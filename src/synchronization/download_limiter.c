#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HANDLE semaphore;

DWORD WINAPI download_file(LPVOID lpParam) {
    int id = *(int*)lpParam;
    
    WaitForSingleObject(semaphore, INFINITE);
    
    printf("[下载 #%d] 开始下载...\n", id);
    Sleep(2000);
    printf("[下载 #%d] 下载完成!\n", id);
    
    ReleaseSemaphore(semaphore, 1, NULL);
    
    return 0;
}

int main() {
    printf("=== 下载并发限制器 (Windows Semaphore) ===\n");
    printf("最大并发: 3, 总任务: 10\n\n");
    
    semaphore = CreateSemaphore(NULL, 3, 3, NULL);
    
    HANDLE threads[10];
    int ids[10];
    
    for (int i = 0; i < 10; i++) {
        ids[i] = i + 1;
        threads[i] = CreateThread(NULL, 0, download_file, &ids[i], 0, NULL);
        Sleep(100);
    }
    
    WaitForMultipleObjects(10, threads, TRUE, INFINITE);
    
    for (int i = 0; i < 10; i++) {
        CloseHandle(threads[i]);
    }
    CloseHandle(semaphore);
    
    printf("\n所有下载完成!\n");
    
    return 0;
}
