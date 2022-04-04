#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

int main(){
    // get key of ipc
    int key = ftok("./mem", 0);
    if(-1 == key) printf("get the key of ipc failed! %m"), exit(-1);
    printf("get the key of ipc success!\n");
    // use the shared memory
    int shmid = shmget(key, 1024, IPC_CREAT|0644);
    if(-1 == shmid) printf("create the shared memory failed: %m\n"), exit(-1);
    printf("create the shared memory success! shmid: %d\n", shmid);
    // delete the memory
    int rnt = shmctl(shmid, IPC_RMID, NULL);
    if(-1 == rnt) printf("delete memory failed\n"), exit(-1);
    printf("successful");
    return 0;
}

