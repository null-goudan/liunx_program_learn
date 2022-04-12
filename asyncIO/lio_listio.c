#include <aio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFF_SIZE 4096
#define ARRAY_SIZE 2
int main(){
    // create the ptr of struct aiocb array
    struct aiocb* aiocb_list[ARRAY_SIZE] = {0};
    // prepare read buffer
    struct aiocb rcb = {0};
    int rfd = open("test.txt", O_RDONLY);
    if(-1 == rfd) printf("can't open file %m"), exit(-1);
    rcb.aio_buf = malloc(BUFF_SIZE + 1);
    memset(rcb.aio_buf, 0 , BUFF_SIZE);
    rcb.aio_fildes = rfd;
    rcb.aio_nbytes = BUFF_SIZE;
    rcb.aio_lio_opcode = LIO_READ;
    aiocb_list[0] = &rcb;
    // prepare write buffer 
    struct aiocb wcb = {0};
    int wfd = open("test.txt", O_WRONLY|O_APPEND);
    if(-1 == wfd) printf("can't open file %m"), close(rfd),free(rcb.aio_buf), exit(-1);
    wcb.aio_buf = malloc(BUFF_SIZE + 1);
    strcpy(wcb.aio_buf,"yyx niubilealhdfasd\n");
    wcb.aio_fildes = wfd;
    wcb.aio_nbytes = strlen("yyx niubilealhdfasd\n");
    wcb.aio_lio_opcode = LIO_WRITE;
    aiocb_list[1] = &wcb;
    // operator of asyncIO : aio_read
    int r = aio_read(&rcb);
    if(-1 == r){
        printf("async read failed! %m\n");
        close(rfd);
        close(wfd);
        free(rcb.aio_buf);
        exit(-1);
    }
    printf("async read successful!\n");
    // operator of asyncIO :  aio_write
    r = aio_write(&wcb);
    if(-1 == r){
        printf("async write failed! %m\n");
        close(rfd);
        close(wfd);
        free(rcb.aio_buf);
        exit(-1);
    }
    printf("async write successful!\n");
    // use lio_listio to watch the data read and write (block) (normaly used)
    r = lio_listio(LIO_WAIT,aiocb_list,1, NULL);
    if(r==-1){
        printf("lio_listio failed! %m\n");
        close(rfd);
        close(wfd);
        free(rcb.aio_buf);
        exit(-1);
    }
    printf("lio_listio successfully!\n");
    // get the data of async
    r = aio_return(&rcb);
    if(r>0){
        printf("Get: r: %d, data: %s\n", r, rcb.aio_buf);
    }
    r = aio_return(&wcb);
    if(r>0){
        printf("Write data: %s\n", wcb.aio_buf);
    }
    // the end
    free(rcb.aio_buf);
    close(rfd);
    return 0;
}

