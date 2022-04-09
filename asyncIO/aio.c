#include <aio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

