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
#include "thread_outgoing_writer.h"

const char *	thread_outgoing_writer_tslogid = "outgoing_writer";
#define TSLOGID	thread_outgoing_writer_tslogid


#define LISTEN_PORT 4321
#define LISTEN_ADDR INADDR_ANY //0.0.0.0
#define TARGET_PORT 1234
#define TARGET_ADDR 0x7F000001 //127.0.0.1


void * thread_outgoing_writer (void * arg)
{
	tslog(0, TSLOGID, "Started.");

	int			socket_fd;
//???	struct pollfd		socket_poll[1];
	struct sockaddr_in	socket_addr;

	int code, b;//???

	while (1)
	{
		tslog(0, TSLOGID, "Allocating transport socket.");
		socket_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1)
		{
			tslog(0, TSLOGID, "Error while creating transport socket: %s", strerror(errno));
			tslog(0, TSLOGID, "Sleeping for a while.");
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, TSLOGID, "Socket allocated.");
		}

		bzero(&socket_addr, sizeof socket_addr);
		socket_addr.sin_family		= AF_INET;
		socket_addr.sin_port		= htons(TARGET_PORT);
		socket_addr.sin_addr.s_addr	= htonl(TARGET_ADDR);

		tslog(0, TSLOGID, "Connectin transport socket.");
		code = connect(socket_fd, (struct sockaddr *)&socket_addr, sizeof socket_addr);
		if (code == -1) {
			tslog(0, TSLOGID, "Error while connecting transport socket: %s", strerror(errno));
			tslog(0, TSLOGID, "Sleeping for a while.");
			close(socket_fd);
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, TSLOGID, "Socket connected.");
		}

		do_this_again:
		pthread_mutex_lock(&transport_queue_mutex);
		size_t count_to_handle, count_actual;
		if (count_to_handle = queue_size_used(&transport_queue))
		{
			tslog(0, TSLOGID, "Found data in queue. Will handle %d bytes.", count_to_handle);

			char buffer[10240];
			count_actual = queue_data_get(&transport_queue, buffer, 10240);
			code = write(socket_fd, buffer, count_actual);
			if (code == -1)
			{
				tslog(0, TSLOGID, "Error while writing to socket: %s", strerror(errno));
				tslog(0, TSLOGID, "Sleeping for a while.");
				close(socket_fd);
				usleep(1000000);
				continue;
			}
		} else
		{
			tslog(0, TSLOGID, "No data. Will wait for them.");
			pthread_cond_wait(&transport_queue_cond, &transport_queue_mutex);
			tslog(0, TSLOGID, "Got signal. Will recheck data queue.");
		}
		pthread_mutex_unlock(&transport_queue_mutex);
		goto do_this_again;
	}
}
