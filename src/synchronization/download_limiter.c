#include <stdio.h>
#include <windows.h>

/*
 * Synchronization Problem #5: Download Thread Concurrency Limiter
 * Mechanism: Counting Semaphore (Initial value = 5)
 * P operation: WaitForSingleObject (decrement semaphore)
 * V operation: ReleaseSemaphore (increment semaphore)
 */

HANDLE semaphore;

DWORD WINAPI download_file(LPVOID lpParam) {
    int id = *(int*)lpParam;

    printf("[Download #%d] Waiting for available slot...\n", id);

    // === P operation: Request download slot ===
    WaitForSingleObject(semaphore, INFINITE);

    // === Critical Section: Download in progress ===
    printf("[Download #%d] Slot acquired, downloading...\n", id);
    Sleep(2000);
    printf("[Download #%d] Download complete!\n", id);

    // === V operation: Release download slot ===
    printf("[Download #%d] Releasing slot\n", id);
    ReleaseSemaphore(semaphore, 1, NULL);

    return 0;
}

int main() {
    printf("========================================\n");
    printf("  Download Concurrency Limiter\n");
    printf("  Synchronization Problem #5\n");
    printf("========================================\n");
    printf("\nMax concurrent downloads: 5\n");
    printf("Total download tasks: 10\n\n");

    // Counting semaphore: initial=5, max=5
    semaphore = CreateSemaphore(NULL, 5, 5, NULL);

    HANDLE threads[10];
    int ids[10];

    for (int i = 0; i < 10; i++) {
        ids[i] = i + 1;
        threads[i] = CreateThread(NULL, 0, download_file, &ids[i], 0, NULL);
        Sleep(200);
    }

    WaitForMultipleObjects(10, threads, TRUE, INFINITE);

    for (int i = 0; i < 10; i++) CloseHandle(threads[i]);
    CloseHandle(semaphore);

    printf("\n========================================\n");
    printf("  All downloads completed!\n");
    printf("========================================\n");

    return 0;
}
