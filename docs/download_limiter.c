#include "../common.h"

#define MAX_CONCURRENT_DOWNLOADS 5   /* 最多5个线程同时下载 */
#define TOTAL_FILES 15               /* 总共15个文件 */

sem_t download_semaphore;            /* 计数信号量，初始值为5 */
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

int active_downloads = 0;            /* 当前正在下载的文件数 */

void* download_file(void* arg) {
    int file_id = *(int*)arg;
    
    pthread_mutex_lock(&print_mutex);
    printf("[等待中] 线程 %d: 等待可用下载槽位...\n", file_id);
    pthread_mutex_unlock(&print_mutex);
    
    /* P操作：等待可用的下载槽位 */
    /* 如果信号量值 > 0，减1并继续；否则阻塞等待 */
    sem_wait(&download_semaphore);
    
    pthread_mutex_lock(&print_mutex);
    active_downloads++;
    printf("[开始下载] 线程 %d: 开始下载 file_%d.zip (当前活跃下载: %d/%d)\n", 
           file_id, file_id, active_downloads, MAX_CONCURRENT_DOWNLOADS);
    pthread_mutex_unlock(&print_mutex);
    
    /* 模拟下载时间（2秒） */
    sleep(2);
    
    pthread_mutex_lock(&print_mutex);
    active_downloads--;
    printf("[下载完成] 线程 %d: 完成下载 file_%d.zip (当前活跃下载: %d/%d)\n", 
           file_id, file_id, active_downloads, MAX_CONCURRENT_DOWNLOADS);
    pthread_mutex_unlock(&print_mutex);
    
    /* V操作：释放一个下载槽位 */
    /* 增加信号量值，唤醒等待中的线程 */
    sem_post(&download_semaphore);
    
    return NULL;
}

int main() {
    print_header("⑤ 文件下载并发线程限制");
    
    printf("问题描述：\n");
    printf("  - 系统最多允许 %d 个线程同时下载文件\n", MAX_CONCURRENT_DOWNLOADS);
    printf("  - 总共有 %d 个文件需要下载\n", TOTAL_FILES);
    printf("  - 超过限制的线程需要等待可用的下载槽位\n\n");
    
    printf("同步方案：\n");
    printf("  - 使用计数信号量（初始值=%d）控制并发\n", MAX_CONCURRENT_DOWNLOADS);
    printf("  - 下载前：sem_wait() - 申请下载槽位\n");
    printf("  - 下载后：sem_post() - 释放下载槽位\n\n");
    
    print_separator();
    
    /* 初始化计数信号量 */
    /* 初始值为5，表示有5个可用的下载槽位 */
    sem_init(&download_semaphore, 0, MAX_CONCURRENT_DOWNLOADS);
    
    pthread_t threads[TOTAL_FILES];
    int file_ids[TOTAL_FILES];
    
    printf("\n开始创建 %d 个下载线程...\n\n", TOTAL_FILES);
    
    /* 创建所有下载线程 */
    for (int i = 0; i < TOTAL_FILES; i++) {
        file_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, download_file, &file_ids[i]);
        /* 稍微延迟，使线程创建过程更清晰 */
        usleep(100000);  /* 延迟100毫秒 */
    }
    
    printf("\n所有线程已创建，开始等待下载完成...\n\n");
    
    /* 等待所有线程完成 */
    for (int i = 0; i < TOTAL_FILES; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n");
    print_separator();
    printf("✓ 所有 %d 个文件下载完成！\n", TOTAL_FILES);
    printf("✓ 总耗时约 %d 秒（%d个文件 ÷ %d个并发 × 2秒）\n", 
           (TOTAL_FILES / MAX_CONCURRENT_DOWNLOADS + 1) * 2,
           TOTAL_FILES, MAX_CONCURRENT_DOWNLOADS);
    print_separator();
    
    /* 清理信号量 */
    sem_destroy(&download_semaphore);
    
    return 0;
}