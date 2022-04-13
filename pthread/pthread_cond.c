#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int n = 0;
pthread_mutex_t mutelock;
pthread_cond_t cond;
void func1(void *arg){
    for(int i=0;i<10;i++){
        pthread_mutex_lock(&mutelock);
        pthread_cond_wait(&cond, &mutelock);
        printf("thread1: n---------%d\n", n++);
        pthread_mutex_unlock(&mutelock);
    }
}

void func2(void *arg){
    for(int i=0;i<10;i++){
        pthread_mutex_lock(&mutelock);
        pthread_cond_wait(&cond, &mutelock);
        printf("thread2: n=========%d\n", n++);
        pthread_mutex_unlock(&mutelock);
    }
}

int main(){
    pthread_t pid1, pid2;
    pthread_mutex_init(&mutelock, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&pid1, NULL, func1, NULL);
    pthread_create(&pid2, NULL, func2, NULL);
    for(int i=0;i<20;i++){
        pthread_cond_signal(&cond);
        sleep(1);
    }
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    printf("n:\t%d\n", n);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutelock);
    return 0;
}