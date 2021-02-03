/**
 * @file Queue.c
 */

#include "cgds/Queue.h"

void _queue_init(Queue* queue, size_t dataSize)
{
  queue->dataSize = dataSize;
  _list_init(queue->list, dataSize);
}

Queue* _queue_new(size_t dataSize)
{
  Queue* queue = (Queue*) safe_malloc(sizeof (Queue));
  queue->list = _list_new(dataSize);
  _queue_init(queue, dataSize);
  return queue;
}

Queue* queue_copy(Queue* queue)
{
  Queue* queueCopy = (Queue*) safe_malloc(sizeof (Queue));
  queueCopy->dataSize = queue->dataSize;
  List* listCopy = list_copy(queue->list);
  queueCopy->list = listCopy;
  return queueCopy;
}

bool queue_empty(Queue* queue)
{
  return list_empty(queue->list);
}

UInt queue_size(Queue* queue)
{
  return list_size(queue->list);
}

void _queue_push(Queue* queue, void* data)
{
  _list_insert_back(queue->list, data);
}

void* _queue_peek(Queue* queue)
{
  return _list_get(queue->list->head);
}

void queue_pop(Queue* queue)
{
  list_remove_front(queue->list);
}

void queue_clear(Queue* queue)
{
  list_clear(queue->list);
}

void queue_destroy(Queue* queue)
{
  list_destroy(queue->list);
  safe_free(queue);
}
