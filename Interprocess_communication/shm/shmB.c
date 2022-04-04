#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

int *p = NULL;

void handle(){
    shmdt(p);

    printf("bye bye!");
    exit(0);
}

int main(){
    // exit signal process
    signal(2, handle);
    // get key of ipc
    int key = ftok("./mem", 0);
    if(-1 == key) printf("get the key of ipc failed! %m"), exit(-1);
    printf("get the key of ipc success!\n");
    // use the shared memory
    int shmid = shmget(key, 1024, IPC_CREAT|0644);
    if(-1 == shmid) printf("create the shared memory failed: %m\n");
    printf("create the shared memory success!\n");
    // share memory mounting
    p = (int *)shmat(shmid, NULL, 0);
    if(NULL == p) printf("share memory mounting failed!%m\n");
    printf("can use the memory! address: %p\n", p);
    // use the memory
    int n = 0;
    while(1){
        *p = n;
        sleep(1);
        n++;
    }
    return 0;
}

