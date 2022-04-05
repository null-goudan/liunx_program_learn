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

int main()
{
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
        int rnt = msgrcv(msgid, &msgs, 1024, 1, 0);
        if (rnt == -1)
            printf("dont send !\n'");
        printf("data: %s\n", msgs.mtext);
        if(strncmp(msgs.mtext, "QUIT", 4)==0)
            break;
    }
}
