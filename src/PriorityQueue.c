/**
 * @file PriorityQueue.c
 */

#include "cgds/PriorityQueue.h"

// NOTE: no _init() method here,
// since PriorityQueue has no specific initialization

PriorityQueue* _priorityqueue_new(size_t dataSize, OrderType pType, UInt arity)
{
	PriorityQueue* priorityQueue =
    (PriorityQueue*) safe_malloc(sizeof (PriorityQueue));
	Heap* heap = _heap_new(dataSize, pType, arity);
	priorityQueue->heap = heap;
	return priorityQueue;
}

PriorityQueue* priorityqueue_copy(PriorityQueue* priorityQueue)
{
	PriorityQueue* priorityQueueCopy = _priorityqueue_new(
		priorityQueue->heap->array->dataSize,
		priorityQueue->heap->hType, priorityQueue->heap->arity);
	heap_destroy(priorityQueueCopy->heap); //TODO: bad style...
	priorityQueueCopy->heap = heap_copy(priorityQueue->heap);
	return priorityQueueCopy;
}

bool priorityqueue_empty(PriorityQueue* priorityQueue)
{
	return heap_empty(priorityQueue->heap);
}

UInt priorityqueue_size(PriorityQueue* priorityQueue)
{
	return heap_size(priorityQueue->heap);
}

ItemValue* priorityqueue_peek_raw(PriorityQueue* priorityQueue)
{
	return heap_top_raw(priorityQueue->heap);
}

void priorityqueue_pop(PriorityQueue* priorityQueue)
{
	heap_pop(priorityQueue->heap);
}

void priorityqueue_clear(PriorityQueue* priorityQueue)
{
	heap_clear(priorityQueue->heap);
}

void priorityqueue_destroy(PriorityQueue* priorityQueue)
{
	heap_destroy(priorityQueue->heap);
	safe_free(priorityQueue);
}
