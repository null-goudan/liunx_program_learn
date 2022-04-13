#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int n = 0;
pthread_rwlock_t rwlock;
void func1(void *arg){
    for(int i=0;i<500000;i++){
        pthread_rwlock_rdlock(&rwlock);
        n++;
        pthread_rwlock_unlock(&rwlock);
    }
}

void func2(void *arg){
    for(int i=0;i<500000;i++){
        pthread_rwlock_rdlock(&rwlock);
        n++;
        pthread_rwlock_unlock(&rwlock);
    }
}

void func3(void *arg){
    for(int i=0;i<500000;i++){
        pthread_rwlock_wrlock(&rwlock);
        n++;
        pthread_rwlock_unlock(&rwlock);
    }
}

int main(){
    pthread_t pid1, pid2;
    pthread_rwlock_init(&rwlock, NULL);
    pthread_create(&pid1, NULL, func1, NULL);
    // pthread_create(&pid2, NULL, func2, NULL);  rr
    pthread_create(&pid2, NULL, func3, NULL); // rw
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    printf("n:\t%d\n", n);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}