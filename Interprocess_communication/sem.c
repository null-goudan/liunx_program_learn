#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
//创建信号量
sem_t mySem;
//设置总票数
int ticket_sum = 10;
//模拟买票过程
void *sell_ticket(void *arg) {
    printf("当前线程ID：%u\n", pthread_self());
    int i;
    int flag;
    for (i = 0; i < 10; i++)
    {
        //完成信号量"减 1"操作，否则暂停执行
        flag = sem_wait(&mySem);
        if (flag == 0) {
            if (ticket_sum > 0)
            {
                sleep(1);
                printf("%u 卖第 %d 张票\n", pthread_self(), 10 - ticket_sum + 1);
                ticket_sum--;
            }
            //执行“加1”操作
            sem_post(&mySem);
            sleep(1);
        }
    }
    return 0;
}

int main() {
    int flag;
    int i;
    void *ans;
    //创建 4 个线程
    pthread_t tids[4];
    //初始化信号量
    flag = sem_init(&mySem, 0, 1);
    if (flag != 0) {
        printf("初始化信号量失败\n");
    }
    for (i = 0; i < 4; i++)
    {
        flag = pthread_create(&tids[i], NULL, &sell_ticket, NULL);
        if (flag != 0) {
            printf("线程创建失败!");
            return 0;
        }
    }
    sleep(10);
    for (i = 0; i < 4; i++)
    {
        flag = pthread_join(tids[i], &ans);
        if (flag != 0) {
            printf("tid=%d 等待失败！", tids[i]);
            return 0;
        }
    }
    //执行结束前，销毁信号量
    sem_destroy(&mySem);
    return 0;
}