#include <stdio.h>
#include <windows.h>

int counter = 0;
HANDLE mutex;

DWORD WINAPI increment_thread(LPVOID lpParam) {
    int id = *(int*)lpParam;
    for (int i = 0; i < 3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        counter++;
        printf("[Thread %d] Enter critical section, counter=%d\n", id, counter);
        Sleep(100);
        printf("[Thread %d] Leave critical section\n", id);
        ReleaseMutex(mutex);
        Sleep(50);
    }
    return 0;
}

int main() {
    printf("========================================\n");
    printf("  Semaphore Demo (Mutex Lock)\n");
    printf("========================================\n\n");

    mutex = CreateMutex(NULL, FALSE, NULL);

    HANDLE threads[5];
    int thread_ids[5];

    for (int i = 0; i < 5; i++) {
        thread_ids[i] = i + 1;
        threads[i] = CreateThread(NULL, 0, increment_thread, &thread_ids[i], 0, NULL);
    }

    WaitForMultipleObjects(5, threads, TRUE, INFINITE);

    for (int i = 0; i < 5; i++) CloseHandle(threads[i]);
    CloseHandle(mutex);

    printf("\nFinal counter = %d (Expected: 15)\n", counter);
    return 0;
}
