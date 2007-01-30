#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void queue_init (struct queue_t * queue, size_t size)
{
	queue->start = malloc(size);
	queue->end = queue->start + size;
	queue->reader = queue->writer = queue->start;
	queue->have_data = 0;
}

void queue_free (struct queue_t * queue)
{
	free(queue->start);
	queue->have_data = 0;
	queue->start = queue->end = queue->reader = queue->writer = NULL;
}

size_t queue_size_free (struct queue_t * queue)
{
	size_t count0, count1, count2;
	if ((queue->reader < queue->writer) || !queue->have_data)
	{
		count1 = (queue->end    - queue->writer);
		count2 = (queue->reader - queue->start );
		count0 = (count1 + count2);
	} else
	{
		count0 = (queue->reader - queue->writer);
	}
	return count0;
}

size_t queue_size_used (struct queue_t * queue)
{
	size_t count0, count1, count2;
	if ((queue->reader < queue->writer) || !queue->have_data)
	{
		count0 = (queue->writer - queue->reader);
	} else
	{
		count1 = (queue->end    - queue->reader);
		count2 = (queue->writer - queue->start );
		count0 = (count1 + count2);
	}
	return count0;
}

size_t queue_data_put (struct queue_t * queue, void * data, size_t size)
{
	size_t result = 0, count0, count1, count2;
	if ((queue->reader < queue->writer) || !queue->have_data)
	{
		count1 = (queue->end    - queue->writer);
		count2 = (queue->reader - queue->start );
		count0 = (count1 + count2);
		result = (size <= count0) ? size : count0;
		if (result <= count1)
		{
			memcpy(queue->writer, data, result);
			queue->writer += result;
		} else
		{
			memcpy(queue->writer, data         ,          count1);
			memcpy(queue->start , data + count1, result - count1);
			queue->writer = queue->start + (result - count1);
		}
	} else
	{
		count0 = (queue->reader - queue->writer);
		result = (size <= count0) ? size : count0;
		{
			memcpy(queue->writer, data, result);
			queue->writer += result;
		}
	}
	if (queue->writer == queue->end)
		queue->writer = queue->start;
	queue->have_data = 1;
	return result;
}

size_t queue_data_get (struct queue_t * queue, void * data, size_t size)
{
	size_t result = 0, count0, count1, count2;
	if ((queue->reader < queue->writer) || !queue->have_data)
	{
		count0 = (queue->writer - queue->reader);
		result = (size <= count0) ? size : count0;
		{
			memcpy(data, queue->reader, result);
			queue->reader += result;
		}
	} else
	{
		count1 = (queue->end    - queue->reader);
		count2 = (queue->writer - queue->start );
		count0 = (count1 + count2);
		result = (size <= count0) ? size : count0;
		if (result <= count1)
		{
			memcpy(data, queue->reader, result);
			queue->reader += result;
		} else
		{
			memcpy(data         , queue->reader,          count1);
			memcpy(data + count1, queue->start , result - count1);
			queue->reader = queue->start + (result - count1);
		}
	}
	if (queue->reader == queue->end)
		queue->reader = queue->start;
	if (queue->reader == queue->writer)
		queue->have_data = 0;
	return result;
}
