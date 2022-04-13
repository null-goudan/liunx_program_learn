#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int num = 0;

void pfuc(void *arg){
    while(1){
        printf("1-thread: num :%d\n", num++);
        sleep(1);
    }
}

int main(){
    pthread_t pid;
    pthread_create(&pid, NULL, pfuc, NULL);
    while(1){
        printf("main-thread: num :%d\n", num++);
        sleep(1);
    }
    return 0;
}
