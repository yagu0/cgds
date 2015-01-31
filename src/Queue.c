/**
 * @file Queue.c
 */

#include "cgds/Queue.h"

void _queue_init(Queue* queue, size_t dataSize)
{
	queue->dataSize = dataSize;
	_vector_init(queue->array, dataSize);
}

Queue* _queue_new(size_t dataSize)
{
	Queue* queue = (Queue*) safe_malloc(sizeof (Queue));
	_queue_init(queue, dataSize);
	return queue;
}

Queue* queue_copy(Queue* queue)
{
	Queue* queueCopy = (Queue*) safe_malloc(sizeof (Queue));
	queueCopy->dataSize = queue->dataSize;
	Vector* arrayCopy = vector_copy(queue->array);
	queueCopy->array = arrayCopy;
	return queueCopy;
}

Bool queue_empty(Queue* queue)
{
	return vector_empty(queue->array);
}

UInt queue_size(Queue* queue)
{
	return vector_size(queue->array);
}

void _queue_push(Queue* queue, void* data)
{
	_vector_push(queue->array, data);
}

void* _queue_peek(Queue* queue)
{
	return vector_get(queue->array, 0);
}

void queue_pop(Queue* queue)
{
	//remove first vector element and shift its internal array
	safe_free(queue->array->datas[0]);
	queue->array->datas++;
	//NOTE: we remove first element, so capacity decrease too
	queue->array->size--;
	queue->array->capacity--;
}

void queue_clear(Queue* queue)
{
	vector_clear(queue->array);
}

void queue_destroy(Queue* queue)
{
	queue_clear(queue);
	safe_free(queue);
}
