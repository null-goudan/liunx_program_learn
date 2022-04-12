#include <aio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFF_SIZE 4096

        //     #include <aiocb.h>
        //     struct aiocb {
        //        /* The order of these fields is implementation-dependent */

        //        int             aio_fildes;     /* File descriptor */
        //        off_t           aio_offset;     /* File offset */
        //        volatile void  *aio_buf;        /* Location of buffer */
        //        size_t          aio_nbytes;     /* Length of transfer */
        //        int             aio_reqprio;    /* Request priority */
        //        struct sigevent aio_sigevent;   /* Notification method */
        //        int             aio_lio_opcode; /* Operation to be performed;
        //                                           lio_listio() only */

        //        /* Various implementation-internal fields not shown */
        //    };

        //    /* Operation codes for 'aio_lio_opcode': */

int main(){
    // prepare buffer
    struct aiocb aio = {0};
    int fd = open("test.txt", O_RDONLY);
    if(-1 == fd) printf("can't open file %m");
    aio.aio_buf = malloc(BUFF_SIZE + 1);
    aio.aio_fildes = fd;
    aio.aio_nbytes = BUFF_SIZE;
    // operator of asyncIO : aio_read, aio_write, aio_error
    int r = aio_read(&aio);
    if(-1 == r){
        printf("async read failed! %m\n");
        close(fd);
        free(aio.aio_buf);
        exit(-1);
    }
    // check operator if finished
    int n = 0;
    while(aio_error(&aio)) n++;
    // get the data of async
    r = aio_return(&aio);
    if(r>0){
        printf("Get: n: %d, r: %d, data: %s", n, r, aio.aio_buf);
    }
    // the end
    free(aio.aio_buf);
    close(fd);
    return 0;
}

