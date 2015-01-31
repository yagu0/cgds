/**
 * @file Queue.h
 */

#ifndef CGDS_QUEUE_H
#define CGDS_QUEUE_H

#include <stdlib.h>
#include <string.h>
#include "cgds/types.h"
#include "cgds/safe_alloc.h"
#include "cgds/Vector.h"

/**
 * @brief Queue containing generic data.
 * @param dataSize Size in bytes of a queue element.
 */
typedef struct Queue {
	size_t dataSize; ///< Size in bytes of a queue element.
	Vector* array; ///< Internal vector representation
} Queue;

/**
 * @brief Initialize an empty queue.
 * @param queue "this" pointer.
 * @param dataSize Size in bytes of a queue element.
 */
void _queue_init(
	Queue* queue, ///< "this" pointer.
	size_t dataSize ///< Size in bytes of a queue element.
);

/** 
 * @brief Return an allocated and initialized queue.
 */
Queue* _queue_new(
	size_t dataSize ///< Size in bytes of a queue element.
);

/** 
 * @brief Return an allocated and initialized queue.
 * @param type Type of a queue element (int, char*, ...).
 * 
 * Usage: Queue* queue_new(<Type> type)
 */
#define queue_new(type) \
	_queue_new(sizeof(type))

/**
 * @brief Copy constructor (works well if items do not have allocated sub-pointers).
 */
Queue* queue_copy(
	Queue* queue ///< "this" pointer.
);

/**
 * @brief Check if the queue is empty.
 */
Bool queue_empty(
	Queue* queue ///< "this" pointer.
);

/**
 * @brief Return size of the current queue.
 */
UInt queue_size(
	Queue* queue ///< "this" pointer.
);

/**
 * @brief Add something at the end of the queue.
 */
void _queue_push(
	Queue* queue, ///< "this" pointer.
	void* data ///< Data to be pushed.
);

/**
 * @brief Add something at the end of the queue.
 * @param queue "this" pointer
 * @param data Data to be pushed.
 * 
 * Usage: void queue_push(Queue* queue, void data)
 */
#define queue_push(queue, data) \
{ \
	typeof((data)) tmp = data; \
	_queue_push(queue, &tmp); \
}

/**
 * @brief Return what is at the beginning of the queue.
 */
void* _queue_peek(
	Queue* queue ///< "this" pointer.
);

/**
 * @brief Return what is at the beginning of the queue.
 * @param queue "this" pointer.
 * @param data Data to be assigned.
 * 
 * Usage: void queue_peek(Queue* queue, void data)
 */
#define queue_peek(queue, data) \
{ \
	void* pData = _queue_peek(queue); \
	data = *((typeof(&data))pData); \
}

/**
 * @brief Remove the beginning of the queue.
 */
void queue_pop(
	Queue* queue ///< "this" pointer.
);

/**
 * @brief Clear the entire queue.
 */
void queue_clear(
	Queue* queue ///< "this" pointer.
);

/**
 * @brief Destroy the queue: clear it, and free 'queue' pointer.
 */
void queue_destroy(
	Queue* queue ///< "this" pointer.
);

#endif
