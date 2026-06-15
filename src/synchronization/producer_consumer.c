#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

HANDLE mutex;
HANDLE not_full;
HANDLE not_empty;

DWORD WINAPI producer(LPVOID lpParam) {
    int id = *(int*)lpParam;
    
    for (int i = 0; i < ITEMS; i++) {
        int item = rand() % 100;
        
        WaitForSingleObject(not_full, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        
        buffer[in] = item;
        printf("[生产者 %d] 生产 %d -> buffer[%d] (库存: %d)\n", id, item, in, count + 1);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        ReleaseMutex(mutex);
        ReleaseSemaphore(not_empty, 1, NULL);
        
        Sleep(rand() % 300);
    }
    
    return 0;
}

DWORD WINAPI consumer(LPVOID lpParam) {
    int id = *(int*)lpParam;
    
    for (int i = 0; i < ITEMS; i++) {
        WaitForSingleObject(not_empty, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        
        int item = buffer[out];
        printf("[消费者 %d] 消费 %d <- buffer[%d] (库存: %d)\n", id, item, out, count - 1);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        ReleaseMutex(mutex);
        ReleaseSemaphore(not_full, 1, NULL);
        
        Sleep(rand() % 500);
    }
    
    return 0;
}

int main() {
    printf("=== 生产者-消费者 (Windows) ===\n\n");
    
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
    
    printf("\n完成!\n");
    
    return 0;
}
