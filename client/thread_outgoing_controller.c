//???#include <poll.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../common/queue.h"
#include "../common/tslog.h"
#include "structs.h"
#include "thread_outgoing.h"
#include "thread_outgoing_reader.h"
#include "thread_outgoing_writer.h"
#include "thread_outgoing_controller.h"

const char *	thread_outgoing_controller_tslogid = "outgoing_controller";
#define TSLOGID	thread_outgoing_controller_tslogid


#define TARGET_PORT 1234
#define TARGET_ADDR 0x7F000001 //127.0.0.1


void * thread_outgoing_controller (void * arg)
{
	tslog(0, TSLOGID, "Started.");

	struct thread_outgoing_synchroblock synchroblock_data;
	struct thread_outgoing_synchroblock * synchroblock = &synchroblock_data;//or use malloc()
	synchroblock->mutex        = PTHREAD_MUTEX_INITIALIZER;
	synchroblock->ready_reader = 0;
	synchroblock->ready_writer = 0;
	synchroblock->ready_cond   = PTHREAD_COND_INITIALIZER;
	synchroblock->socket_fd    = 0;//NB: this value initially has no matter.
	synchroblock->socket_ready = 0;
	synchroblock->socket_cond  = PTHREAD_COND_INITIALIZER;
	synchroblock->error_cond   = PTHREAD_COND_INITIALIZER;

	int code;

	do
	{
		tslog(0, TSLOGID, "Creating outgoing reader thread.");
		pthread_t reader;
		code = pthread_create(&reader, NULL, thread_outgoing_reader, synchroblock);
		if (code)
		{       
			tslog(0, TSLOGID, "Creating outgoing reader thread failed.");
			usleep(1000000);
		} else
		{
			tslog(0, TSLOGID, "Creating outgoing reader thread successed.");
		}
	} while (code);

	do
	{
		tslog(0, TSLOGID, "Creating outgoing writer thread.");
		pthread_t writer;
		code = pthread_create(&writer, NULL, thread_outgoing_writer, synchroblock);
		if (code)
		{
			tslog(0, TSLOGID, "Creating outgoing writer thread failed.");
			usleep(1000000);
		} else
		{
			tslog(0, TSLOGID, "Creating outgoing writer thread successed.");
		}
	} while (code);

	//???!!! wait for reader and writer to finally initialize (HOW? FOR WHAT?)

	do
	{
		tslog(8, TSLOGID, "Creating socket.");
		int fd = socket(PF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
			tslog(1, TSLOGID, "Creating socket failed: %s", strerror(errno));
			usleep(1000000);
			continue;
		} else
		{
			tslog(8, TSLOGID, "Creating socket successed, fd = %d.", fd);
		}

		struct sockaddr_in sa;
		memset(&sa, 0, sizeof sa);
		sa.sin_family		= AF_INET;
		sa.sin_port		= htons(TARGET_PORT);
		sa.sin_addr.s_addr	= htonl(TARGET_ADDR);

		tslog(0, TSLOGID, "Connecting socket.");
		code = connect(fd, (struct sockaddr *)&sa, sizeof sa);
		if (code == -1) {
			tslog(0, TSLOGID, "Connecting socket failed: %s", strerror(errno));
			close(fd);
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, TSLOGID, "Connecting socket successed.");
		}

		tslog(8, TSLOGID, "Waiting for subthreads to be ready and feeding socket to them.");
		pthread_mutex_lock(&(synchroblock->mutex));
		while (!synchroblock->ready_reader || !synchroblock->ready_writer)
		{
			pthread_cond_wait(&(synchroblock->ready_cond), &(synchroblock->mutex));
		}

		tslog(8, TSLOGID, "Feeding socket to subthreads.");
		synchroblock->socket_fd = fd;
		synchroblock->socket_ready = 1;
		pthread_cond_broadcast(&(synchroblock->socket_cond));

		tslog(8, TSLOGID, "Waiting for error from subthreads.");
		pthread_cond_wait(&(synchroblock->error_cond), &(synchroblock->mutex));
		synchroblock->ready_reader = 0;
		synchroblock->ready_writer = 0;
		synchroblock->socket_ready = 0;
		pthread_mutex_unlock(&(synchroblock->mutex));

		tslog(8, TSLOGID, "Closing socket.");
		close(fd);
		usleep(1000000);
		continue;
	} while (1);
}
