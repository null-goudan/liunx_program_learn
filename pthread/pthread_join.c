#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int num = 0;

struct stu
{
    char name[32];
    int score;
};


void pfuc(void *arg){
    int ret = 0;
    for(int i=0;i<10; i++){
        printf("1-thread: num : %d\n", num++);
        sleep(1);
    }
    ret = num;
    return &ret;
}

void pfuc2(void *arg){
    int ret = 0;
    struct stu* data = (struct stu*)arg;
    for(int i=0;i<10; i++){
        printf("2-thread: num : %d\ndata: name: %s, score: %d\n", num++, data->name, data->score);
        sleep(1);
    }
    ret = 9999;
    return &ret;
}

int main(){
    int *ret1 = malloc(4);
    int *ret2 = malloc(4);
    pthread_t pid, pid2;
    struct stu stu1 = {
        "yyx",
        18
    };
    *ret1 = 10;
    *ret2 = 20;
    printf("before thread finish: ret1 : %d , ret2 : %d\n", *ret1, *ret2);
    pthread_create(&pid, NULL, pfuc, NULL);
    pthread_create(&pid2, NULL, pfuc2, &stu1);
    for(int i = 0;i<11;i++){
        printf("main-thread: num :%d\n", num++);
        sleep(1);
    }
    pthread_join(pid, (void**)ret1);
    printf("ret1: %d\n", *ret1);
    pthread_join(pid2, (void**)ret2);
    printf("ret2: %d\n", *ret2);
    return 0;
}
