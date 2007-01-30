#include <pthread.h>
#include "../common/queue.h"
#include "structs.h"

struct queue_t	transport_queue;
pthread_mutex_t transport_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	transport_queue_cond  = PTHREAD_COND_INITIALIZER;
