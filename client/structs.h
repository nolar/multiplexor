#ifndef __TCPMUX_STRUCTS_H
#define __TCPMUX_STRUCTS_H

#include <pthread.h>
#include "../common/queue.h"

extern struct queue_t	transport_queue;
extern pthread_mutex_t	transport_queue_mutex;
extern pthread_cond_t	transport_queue_cond;

#endif
