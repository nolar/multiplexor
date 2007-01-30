#ifndef __TCPMUX_TSLOG_H
#define __TCPMUX_TSLOG_H

/*
#include <stdio.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <stdarg.h>
#include <pthread.h>
*/

extern void tslog (int level, const char * id, const char * format, ...);

#endif
