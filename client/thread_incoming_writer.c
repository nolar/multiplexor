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
#include "thread_incoming_writer.h"

const char *	thread_incoming_writer_tslogid = "incoming_writer";
#define TSLOGID	thread_incoming_writer_tslogid


#define LISTEN_PORT 4321
#define LISTEN_ADDR INADDR_ANY //0.0.0.0


void * thread_incoming_writer (void * arg)
{
	tslog(0, TSLOGID, "(skipped)");
}
