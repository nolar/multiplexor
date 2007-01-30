#include <sys/types.h>
#include <sys/errno.h>
#include <stdio.h>
#include <pthread.h>
#include "../common/queue.h"
#include "../common/tslog.h"
#include "structs.h"
#include "thread_incoming_controller.h"
#include "thread_outgoing_controller.h"

int main()
{
	int code;
	pthread_t thread1;
	pthread_t thread2;
//	pthread_t thread_of_connection[CONN_COUNT];

//???	pthread_mutex_init(&log_mutex, NULL);
	queue_init(&transport_queue, 1024);

	code = pthread_create(&thread1, NULL, thread_incoming_controller, NULL);
	if (code)
	{
		printf("pthread_create(listener)\n");
		exit(-1);
	}

	code = pthread_create(&thread2, NULL, thread_outgoing_controller, NULL);
	if (code)
	{
		printf("pthread_create(transport)\n");
		exit(-1);
	}

	pthread_exit(NULL);
}
