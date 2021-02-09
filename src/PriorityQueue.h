/**
 * @file PriorityQueue.h
 */

#ifndef CGDS_PRIORITY_QUEUE_H
#define CGDS_PRIORITY_QUEUE_H

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "cgds/Heap.h"
#include "cgds/safe_alloc.h"

/**
 * @brief Priority queue data structure (wrapper around Heap).
 */
typedef struct PriorityQueue {
  Heap* heap; ///< Internal heap.
} PriorityQueue;

/**
 * @brief Return an allocated and initialized Queue.
 */
PriorityQueue* _priorityqueue_new(
  size_t dataSize, ///< Size in bytes of a priority queue element.
  OrderType pType, ///< Type of priority queue: max or min first (MAX_T or MIN_T).
  UInt arity ///< Arity of the wrapped heap: any integer >=2.
);

/**
 * @brief Return an allocated and initialized Queue.
 * @param type Type of a priority queue item (int, char*, ...).
 * @param pType type of priority queue: max or min first (MAX_T or MIN_T).
 * @param arity Arity of the wrapped heap: any integer >=2.
 *
 * Usage: PriorityQueue* priorityqueue_new(<Type> type, OrderType pType, UInt arity)
 */
#define priorityqueue_new(type, pType, arity) \
  _priorityqueue_new(sizeof(type), pType, arity)

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
PriorityQueue* priorityqueue_copy(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

/**
 * @brief Check if the priority queue is empty.
 */
bool priorityqueue_empty(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

/**
 * @brief Return the size of current priority queue.
 */
UInt priorityqueue_size(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

/**
 * @brief Add an (item,priority) inside the priority queue.
 * @param priorityQueue "this" pointer.
 * @param item Item to be added.
 * @param priority Priority of the added item.
 *
 * Usage: void priorityqueue_insert(PriorityQueue* priorityQueue, void item, Real priority)
 */
#define priorityqueue_insert(priorityQueue, item, priority) \
  heap_insert(priorityQueue->heap, item, priority)

/**
 * @brief Change the priority of an item in the queue.
 * @param priorityQueue "this" pointer.
 * @param item Item to be modified.
 * @param newPriority New priority of the modified item.
 * @note If several similar items are present, only the first is affected.
 *
 * Usage: void priorityqueue_set_priority(PriorityQueue* priorityQueue, void item, Real newPriority)
 */
#define priorityqueue_set(priorityQueue, item, newPriority) \
  heap_modify(priorityQueue->heap, item, newPriority)

/**
 * @brief Remove an item in the queue.
 * @param priorityQueue "this" pointer.
 * @param item Item to be removed.
 * @note If several similar items are present, only the first is deleted.
 *
 * Usage: void priorityqueue_remove(PriorityQueue* priorityQueue, void item)
 */
#define priorityqueue_remove(priorityQueue, item) \
  heap_remove(priorityQueue->heap, item)

/**
 * @brief Return what is at the beginning of the queue.
 * @return An ItemValue* 'iv' with iv.item = data, and iv.value its priority.
 */
ItemValue _priorityqueue_peek(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

/**
 * @brief Peek the item at the beginning of the queue.
 * @param priorityQueue "this" pointer.
 * @param item Item to be assigned.
 *
 * Usage: void priorityqueue_peek(PriorityQueue* priorityQueue, void item)
 */
#define priorityqueue_peek(priorityQueue, item) \
  heap_top(priorityQueue->heap, item)

/**
 * @brief Remove the top element in the queue.
 */
void priorityqueue_pop(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

/**
 * @brief Clear the entire queue.
 */
void priorityqueue_clear(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

/**
 * @brief Destroy the queue: clear it and free 'priorityQueue' memory.
 */
void priorityqueue_destroy(
  PriorityQueue* priorityQueue ///< "this" pointer.
);

#endif
