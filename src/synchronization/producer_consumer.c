#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/*
 * Producer-Consumer Problem
 * Synchronization using 3 semaphores:
 *   mutex     - mutual exclusion for buffer access
 *   not_full  - counting empty slots (init = BUFFER_SIZE)
 *   not_empty - counting filled slots (init = 0)
 */

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

HANDLE mutex;
HANDLE not_full;
HANDLE not_empty;

DWORD WINAPI producer(LPVOID lpParam) {
    int id = *(int*)lpParam;

    for (int i = 0; i < 10; i++) {
        int item = rand() % 100;

        // P(empty) - wait for empty slot
        WaitForSingleObject(not_full, INFINITE);
        // P(mutex) - enter critical section
        WaitForSingleObject(mutex, INFINITE);

        // Critical section: add item to buffer
        buffer[in] = item;
        printf("[Producer %d] Produced %d -> buffer[%d] (Stock: %d)\n",
               id, item, in, count + 1);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        // V(mutex) - leave critical section
        ReleaseMutex(mutex);
        // V(full) - signal consumer
        ReleaseSemaphore(not_empty, 1, NULL);

        Sleep(rand() % 300);
    }
    return 0;
}

DWORD WINAPI consumer(LPVOID lpParam) {
    int id = *(int*)lpParam;

    for (int i = 0; i < 10; i++) {
        // P(full) - wait for filled slot
        WaitForSingleObject(not_empty, INFINITE);
        // P(mutex) - enter critical section
        WaitForSingleObject(mutex, INFINITE);

        // Critical section: remove item from buffer
        int item = buffer[out];
        printf("[Consumer %d] Consumed %d <- buffer[%d] (Stock: %d)\n",
               id, item, out, count - 1);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        // V(mutex) - leave critical section
        ReleaseMutex(mutex);
        // V(empty) - signal producer
        ReleaseSemaphore(not_full, 1, NULL);

        Sleep(rand() % 500);
    }
    return 0;
}

int main() {
    printf("========================================\n");
    printf("  Producer-Consumer Problem\n");
    printf("========================================\n\n");

    mutex = CreateMutex(NULL, FALSE, NULL);
    not_full = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    not_empty = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);

    int p_id = 1, c_id = 1;
    HANDLE p_thread = CreateThread(NULL, 0, producer, &p_id, 0, NULL);
    HANDLE c_thread = CreateThread(NULL, 0, consumer, &c_id, 0, NULL);

    HANDLE threads[2] = {p_thread, c_thread};
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(p_thread);
    CloseHandle(c_thread);
    CloseHandle(mutex);
    CloseHandle(not_full);
    CloseHandle(not_empty);

    printf("\n========================================\n");
    printf("  Producer-Consumer Complete!\n");
    printf("========================================\n");

    return 0;
}
