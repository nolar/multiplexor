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
#include "thread_incoming_reader.h"

void * thread_incoming_reader (void * arg)
{
	int socket_fd = *((int*)arg);
	tslog(0, "ACCEPTOR", "Connection thread has started for fd %d.", socket_fd);

	char buffer[10240];
	int code, b;//???

	while (1)
	{
		code = read(socket_fd, buffer, 10240);
		if (code == -1)
		{
			tslog(0, "ACCEPTOR", "Error while reading socket %d: %s", socket_fd, strerror(errno));
			tslog(0, "ACCEPTOR", "Exiting connection %d.", socket_fd);
			close(socket_fd);
			break;
		} else
		if (code == 0)
		{
			tslog(0, "ACCEPTOR", "EOF at socket %d.", socket_fd);
			tslog(0, "ACCEPTOR", "Exiting connection %d.", socket_fd);
			close(socket_fd);
			break;
		} else
		{
			tslog(0, "ACCEPTOR", "Got %d bytes from socket %d.", code, socket_fd);
			size_t count_total = code, count_queued;
			do
			{
				pthread_mutex_lock(&transport_queue_mutex);
				count_queued = queue_data_put(&transport_queue, buffer, count_total);
				count_total -= count_queued;
				tslog(0, "ACCEPTOR", "Put %d bytes to queue, %d bytes waiting.", count_queued, count_total);
				pthread_cond_broadcast(&transport_queue_cond);
				pthread_mutex_unlock(&transport_queue_mutex);
			} while (count_total);
		}
	}

	return NULL;
}
