/**
 * @file PriorityQueue.c
 */

#include "cgds/PriorityQueue.h"

// NOTE: no init() method here,
// since PriorityQueue has no specific initialization

PriorityQueue* _priorityqueue_new(size_t dataSize, OrderType pType, UInt arity)
{
  PriorityQueue* priorityQueue =
    (PriorityQueue*) safe_malloc(sizeof (PriorityQueue));
  priorityQueue->heap = _heap_new(dataSize, pType, arity);
  return priorityQueue;
}

PriorityQueue* priorityqueue_copy(PriorityQueue* priorityQueue)
{
  PriorityQueue* priorityQueueCopy =
    (PriorityQueue*) safe_malloc(sizeof (PriorityQueue));
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

ItemValue _priorityqueue_peek(PriorityQueue* priorityQueue)
{
  return _heap_top(priorityQueue->heap);
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
