#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <sanitizer/dfsan_interface.h>
#include "debug.h"
#include "config.h"
#include "label.h"

static int fd = 0;

static void assign_taint_labels(void *buf, long offset, size_t size) 
{
    static char str[50];
    for (size_t i = 0; i < size; i += granularity) 
    {
        sprintf(str, "%lu", offset + i);

        dfsan_label L = dfsan_create_label(str, 0);
    
        if (size < i + granularity)
        {
            dfsan_set_label(L, (char *)(buf) + offset + i, size - i);
            dfsan_warning("The last label is less than granularity(value:%d) in func %s \n", granularity, __func__);
            set_label(offset + i, offset + size - 1, L);
        }
        else
        {
            dfsan_set_label(L, (char *)(buf) + offset + i, granularity);
            set_label(offset + i, offset + i + granularity - 1, L);
        }
    }
}

static void assign_taint_labels_exf(void *buf, long offset, size_t ret, size_t size) 
{
    if (offset < 0)
        offset = 0;
    // if count is not so huge!
    int len = ret;
    // if (ret < size) 
    // {
    //     int res = size - ret;
    //     len = res < 1024? len + res : len + 1024;
    //     dfsan_warning("The number read is less than [size] in func %s \n", __func__);
    // }
    assign_taint_labels(buf, offset, len);
}

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
__attribute__((visibility("default"))) ssize_t 
__dfsw_recv (int __fd, void *__buf, size_t __n, int __flags,
            dfsan_label __fd_label, dfsan_label __buf_label, dfsan_label __n_label, dfsan_label __flags_label,
            dfsan_label *ret_label)
{
    ssize_t ret = recv(__fd, __buf, __n, __flags);

    if(ret != -1)
    {
        assign_taint_labels_exf(__buf, fd, ret, __n);
        fd += ret;
    }

    dfsan_debug("%s called, recv %d bytes\n", __func__, ret);

    *ret_label = 0;
    return ret;
}


/* Read N bytes into BUF through socket FD.
   If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of
   the sender, and store the actual size of the address in *ADDR_LEN.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
__attribute__((visibility("default"))) ssize_t 
__dfsw_recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags, 
                __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len,
                dfsan_label __fd_label, dfsan_label __buf_label, dfsan_label __n_label, 
                dfsan_label __flags_label, dfsan_label __addr_label, dfsan_label __addr_len_label,
                dfsan_label *ret_label)
{
    ssize_t ret = recvfrom(__fd, __buf, __n, __flags, __addr, __addr_len);

    dfsan_debug("%s called, recv %d bytes\n", __func__, ret);

    *ret_label = 0;
    return ret;
}


/* Receive a message as described by MESSAGE from socket FD.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
__attribute__((visibility("default"))) ssize_t 
__dfsw_recvmsg (int __fd, struct msghdr *__message, int __flags,
                dfsan_label __fd_label, dfsan_label __message_label, dfsan_label __flags_label,
                dfsan_label *ret_label)
{
    ssize_t ret = recvmsg(__fd, __message, __flags);

    dfsan_debug("%s called, recv %d bytes\n", __func__, ret);

    *ret_label = 0;
    return ret;
}

 #ifdef __USE_GNU
/* Receive up to VLEN messages as described by VMESSAGES from socket FD.
   Returns the number of messages received or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
__attribute__((visibility("default"))) int 
__dfsw_recvmmsg (int __fd, struct mmsghdr *__vmessages, unsigned int __vlen, 
                int __flags, struct timespec *__tmo,
                dfsan_label __fd_label, dfsan_label __vmessages_label, dfsan_label __vlen_label, 
                dfsan_label __flags_label, dfsan_label __tmo_label, 
                dfsan_label *ret_label)
{
    int ret = recvmmsg(__fd, __vmessages, __vlen, __flags, __tmo);

    dfsan_debug("%s called, recv %d messages\n", __func__, ret);

    *ret_label = 0;
    return ret;
}
#endif



