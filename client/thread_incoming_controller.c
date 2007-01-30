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
#include "thread_incoming_writer.h"
#include "thread_incoming_controller.h"

#define BACKLOG 4
#define CONN_COUNT 128
#define BUFFER_SIZE 1024
#define LISTEN_PORT 4321
#define LISTEN_ADDR INADDR_ANY //0.0.0.0
#define TARGET_PORT 1234

void * thread_incoming_controller (void * arg)
{
	tslog(0, "LISTENER", "Listener thread has started.");

	int			socket_fd;
//???	struct pollfd		socket_poll[1];
	struct sockaddr_in	socket_addr;

	int code, b;//???

	while (1)
	{
		tslog(0, "LISTENER", "Allocating listener socket.");
		socket_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1)
		{
			tslog(0, "LISTENER", "Error while creating listener socket: %s", strerror(errno));
			tslog(0, "LISTENER", "Sleeping for a while.");
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, "LISTENER", "Socket allocated.");
		}

		bzero(&socket_addr, sizeof socket_addr);
		socket_addr.sin_family		= AF_INET;
		socket_addr.sin_port		= htons(LISTEN_PORT);
		socket_addr.sin_addr.s_addr	= htonl(LISTEN_ADDR);

		tslog(0, "LISTENER", "Binding socket.");
		code = bind(socket_fd, (struct sockaddr *)&socket_addr, sizeof socket_addr);
		if (code == -1) {
			tslog(0, "LISTENER", "Error while binding listener socket: %s", strerror(errno));
			tslog(0, "LISTENER", "Sleeping for a while.");
			close(socket_fd);
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, "LISTENER", "Socket binded.");
		}

		tslog(0, "LISTENER", "Listening socket.");
		code = listen(socket_fd, BACKLOG);
		if (code == -1) {
			tslog(0, "LISTENER", "Error while listening socket: %s", strerror(errno));
			tslog(0, "LISTENER", "Sleeping for a while.");
			close(socket_fd);
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, "LISTENER", "Socket listened.");
		}

		do_this_again:
		tslog(0, "LISTENER", "Accepting socket.");
		b = sizeof socket_addr;
		code = accept(socket_fd, (struct sockaddr *)&socket_addr, &b);
		if (code == -1) {
			tslog(0, "LISTENER", "Error while accepting socket: %s", strerror(errno));
			tslog(0, "LISTENER", "Sleeping for a while.");
			close(socket_fd);
			usleep(1000000);
			continue;
		} else
		{
			tslog(0, "LISTENER", "Connection accepted.");
		}

		int * thread_arg = malloc(sizeof code);
		*thread_arg = code;

		pthread_t thread;
		code = pthread_create(&thread, NULL, thread_incoming_reader, thread_arg);
		if (code)
		{
			tslog(0, "LISTENER", "Error while creatign thread. Closing this connection.");
			close(*thread_arg);
		} else
		{
			tslog(0, "LISTENER", "Thread created for a connection.");
		}

		goto do_this_again;
	}
}
