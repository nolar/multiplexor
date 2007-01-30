#ifndef __TCPMUX_QUEUE_H
#define __TCPMUX_QUEUE_H

#include <sys/types.h>

struct queue_t
{
	void * start;
	void * end;
	void * reader;
	void * writer;
	int have_data;
};

extern void   queue_init	(struct queue_t * queue, size_t size);
extern void   queue_free	(struct queue_t * queue);
extern size_t queue_size_free	(struct queue_t * queue);
extern size_t queue_size_used	(struct queue_t * queue);
extern size_t queue_data_put	(struct queue_t * queue, void * data, size_t size);
extern size_t queue_data_get	(struct queue_t * queue, void * data, size_t size);

#endif
