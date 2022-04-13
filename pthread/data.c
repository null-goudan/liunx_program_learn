#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int n = 0;

void func1(void *arg){
    for(int i=0;i<5000000;i++){
        n++;
    }
}

void func2(void *arg){
    for(int i=0;i<5000000;i++){
        n++;
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