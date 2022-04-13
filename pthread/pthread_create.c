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
    while(1){
        printf("1-thread: num :%d\n", num++);
        sleep(1);
    }
}

void pfuc2(void *arg){
    struct stu* data = (struct stu*)arg;
    while(1){
        printf("2-thread: num : %d\n data: name: %s, score: %d\n", num++, data->name, data->score);
        sleep(1);
    }
}

int main(){
    pthread_t pid, pid2;
    struct stu stu1 = {
        "yyx",
        18
    };
    pthread_create(&pid, NULL, pfuc, NULL);
    pthread_create(&pid2, NULL, pfuc2, &stu1);
    while(1){
        printf("main-thread: num :%d\n", num++);
        sleep(1);
    }
    return 0;
}
