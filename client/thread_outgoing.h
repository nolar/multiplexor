#ifndef __TCPMUX_THREAD_OUTGOING_H
#define __TCPMUX_THREAD_OUTGOING_H

#include <sys/types.h>
#include <pthread.h>

struct thread_outgoing_synchroblock
{
	pthread_mutex_t	mutex;

	int		ready_reader;
	int		ready_writer;
	pthread_cond_t	ready_cond;

	int		socket_fd;
	int		socket_ready;
	pthread_cond_t	socket_cond;

	pthread_cond_t	error_cond;
};

#endif
