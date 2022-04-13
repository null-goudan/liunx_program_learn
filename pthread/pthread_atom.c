#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define atomic_inc(x)  __sync_fetch_and_add(x, 1)
int n = 0;

void func1(void *arg){
    for(int i=0;i<5000000;i++){
        atomic_inc(&n);
    }
}

void func2(void *arg){
    for(int i=0;i<5000000;i++){
        atomic_inc(&n);
    }
}

int main(){
    pthread_t pid1, pid2;
    pthread_create(&pid1, NULL, func1, NULL);
    pthread_create(&pid2, NULL, func2, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    printf("n:\t%d\n", n);
    return 0;
}