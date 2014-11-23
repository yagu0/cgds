/**
 * @file Queue.c
 */

#include "cgds/Queue.h"

void _queue_init(Queue* queue, size_t dataSize)
{
	queue->size = 0;
	queue->dataSize = dataSize;
	queue->front = NULL;
	queue->back = NULL;
}

Queue* _queue_new(size_t dataSize)
{
	Queue* queue = (Queue*) safe_malloc(sizeof (Queue));
	_queue_init(queue, dataSize);
	return queue;
}

Queue* queue_copy(Queue* queue)
{
	Queue* queueCopy = _queue_new(queue->dataSize);
	QueueCell* queueCell = queue->front;
	while (queueCell != NULL)
	{
		_queue_push(queueCopy, queueCell->data);
		queueCell = queueCell->next;
	}
	return queueCopy;
}

Bool queue_empty(Queue* queue)
{
	return (queue->size == 0);
}

UInt queue_size(Queue* queue)
{
	return queue->size;
}

void _queue_push(Queue* queue, void* data)
{
	QueueCell* newQueueBack = (QueueCell*) safe_malloc(sizeof (QueueCell));
	newQueueBack->data = safe_malloc(queue->dataSize);
	memcpy(newQueueBack->data, data, queue->dataSize);
	newQueueBack->next = NULL;
	if (queue->size > 0) 
		queue->back->next = newQueueBack;
	queue->back = newQueueBack;
	if (queue->size == 0) 
		queue->front = newQueueBack;
	queue->size++;
}

void* _queue_peek(Queue* queue)
{
	return queue->front->data;
}

void queue_pop(Queue* queue)
{
	QueueCell* toTrash = queue->front;
	queue->front = queue->front->next;
	if (queue->front == NULL) 
		queue->back = NULL;
	safe_free(toTrash->data);
	safe_free(toTrash);
	queue->size--;
}

void queue_clear(Queue* queue)
{
	while (queue->size > 0) 
		queue_pop(queue);
	_queue_init(queue, queue->dataSize);
}

void queue_destroy(Queue* queue)
{
	queue_clear(queue);
	safe_free(queue);
}
