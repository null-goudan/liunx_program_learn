#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/signal.h>
#include <string.h>

int msgid = 0;

struct msgbuf
{
    long mtype;    /* message type, must be > 0 */
    char mtext[512]; /* message data */
};

void quitHandler(){

    if(msgctl(msgid, IPC_RMID, NULL)<0){
        printf("del failed! %m\n");
    }

    printf("bye bye");
    exit(-1);
}

int main()
{
    signal(2,quitHandler);
    int key = ftok(".", 66);
    if (-1 == key)
        printf("don't create key ! %m"), exit(-1);
    printf("key create successful\n");
    msgid = msgget(key, IPC_CREAT | 0666);
    if (-1 == msgid)
        printf("don't get the msg queue! %m"), exit(-1);
    printf("msg queue create successful\n");
    struct msgbuf msgs;
    while (1)
    {
        char buffer[1024];
        msgs.mtype = 1;
        scanf("%s", buffer);
        strcpy(msgs.mtext, buffer);
        int rnt = msgsnd(msgid, &msgs, 1024, 0);
        if (rnt == -1)
            printf("dont send !\n'");
    }
}
