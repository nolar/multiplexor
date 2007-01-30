//???#include <poll.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include "../common/queue.h"
#include "../common/tslog.h"
#include "structs.h"
#include "thread_outgoing.h"
#include "thread_outgoing_reader.h"

const char *	thread_outgoing_reader_tslogid = "outgoing_reader";
#define TSLOGID	thread_outgoing_reader_tslogid

void * thread_outgoing_reader (void * arg)
{
	tslog(0, TSLOGID, "Started.");
	struct thread_outgoing_synchroblock * synchroblock = arg;
	int code;

	do
	{
		tslog(0, TSLOGID, "Signalling that we are a ready and waiting for socket fd.");
		pthread_mutex_lock(&(synchroblock->mutex));
		synchroblock->ready_reader = 1;
		pthread_cond_broadcast(&(synchroblock->ready_cond));
		while (!synchroblock->socket_ready)
		{
			pthread_cond_wait(&(synchroblock->socket_cond), &(synchroblock->mutex));
		}
		int fd = synchroblock->socket_fd;
		pthread_mutex_unlock(&(synchroblock->mutex));


		//!!! read and parse it here.
		//!!! on error or eof: pthread_cond_broadcast(&(synchroblock->error_cond)); then usleep() and "continue";

		tslog(0, TSLOGID, "typa, rabotaem...");
		code = -1; usleep(7000000);// EMULATION OF WORK and ERROR

		if (code == -1)
		{
			tslog(0, TSLOGID, "typa, error...");
			pthread_cond_broadcast(&(synchroblock->error_cond));
			usleep(1000000);
			continue;
		}
	} while (1);
}
