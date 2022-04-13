#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int n = 0;
pthread_mutex_t mutelock;
void func1(void *arg){
    for(int i=0;i<5000000;i++){
        pthread_mutex_lock(&mutelock);
        n++;
        pthread_mutex_unlock(&mutelock);
    }
}

void func2(void *arg){
    for(int i=0;i<5000000;i++){
        pthread_mutex_lock(&mutelock);
        n++;
        pthread_mutex_unlock(&mutelock);
    }
}

int main(){
    pthread_t pid1, pid2;
    pthread_mutex_init(&mutelock, NULL);
    pthread_create(&pid1, NULL, func1, NULL);
    pthread_create(&pid2, NULL, func2, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    printf("n:\t%d\n", n);
    pthread_attr_destroy(&mutelock);
    return 0;
}