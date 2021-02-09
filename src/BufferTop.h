/**
 * @file BufferTop.h
 */

#ifndef CGDS_BUFFER_TOP_H
#define CGDS_BUFFER_TOP_H

#include <stdlib.h>
#include <string.h>
#include "cgds/types.h"
#include "cgds/Heap.h"
#include "cgds/List.h"
#include "cgds/safe_alloc.h"

/**
 * @brief Data structure to store top (MAX or MIN) elements in a buffer.
 */
typedef struct BufferTop {
  UInt capacity; ///< Buffer capacity (in items count).
  OrderType bType; ///< Type of buffer: keep max or min items (MAX_T or MIN_T).
  Heap* heap; ///< Items + values are internally organized into a heap.
} BufferTop;

/**
 * @brief Return an allocated and initialized buffer.
 */
BufferTop* _buffertop_new(
  size_t dataSize, ///< Size in bytes of a buffer element.
  UInt capacity, ///< Maximum number of elements that the buffer can contain.
  OrderType bType, ///< Type of buffer: keep max or min items (MAX_T or MIN_T).
  UInt arity ///< Arity of the wrapped heap: any integer >=2.
);

/**
 * @brief Return an allocated and initialized buffer.
 * @param type Type of a buffer item (int, char*, ...).
 * @param capacity Maximum number of elements that the buffer can contain.
 * @param bType type of buffer: keep max or min items (MAX_T or MIN_T).
 * @param arity Arity of the wrapped heap: any integer >=2.
 *
 * Usage: BufferTop* buffertop_new(<Type> type, UInt capacity, OrderTypebType, UInt arity)
 */
#define buffertop_new(type, capacity, bType, arity) \
  _buffertop_new(sizeof(type), capacity, bType, arity)

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
BufferTop* buffertop_copy(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief Turn the buffer into a list to scan its content linearly.
 */
List* buffertop_2list(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief Check if the buffer is empty.
 */
bool buffertop_empty(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief Return the size of current buffer (<= capacity).
 */
UInt buffertop_size(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief (Try to) add an item-value in the buffer.
 */
void _buffertop_tryadd(
  BufferTop* bufferTop, ///< "this" pointer.
  void* item, ///< Pointer to an item of type as defined in the constructor.
  Real value ///< Value associated with the item.
);

/**
 * @brief (Try to) add an item-value in the buffer.
 * @param bufferTop "this" pointer.
 * @param item Item of type as defined in the constructor.
 * @param value Value associated with the item.
 *
 * Usage: void buffertop_tryadd(BufferTop* bufferTop, void item, Real value)
 */
#define buffertop_tryadd(bufferTop, item, value) \
{ \
  typeof(item) tmp = item; \
  _buffertop_tryadd(bufferTop, &tmp, value); \
}

/**
 * @brief Return the top ("worst among best") ItemValue inside current buffer.
 */
ItemValue _buffertop_first(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief Set item_ to the top ("worst among best") item inside current buffer.
 * @param bufferTop "this" pointer.
 * @param item_ Variable to be assigned.
 *
 * Usage: void buffertop_first(BufferTop* bufferTop, void item_)
 */
#define buffertop_first(bufferTop, item_) \
{ \
  ItemValue iv = _buffertop_first(bufferTop); \
  item_ = *((typeof(item_)*)(iv.item)); \
}

/**
 * @brief Remove the top ("worst among best") item-value inside the buffer.
 */
void buffertop_pop(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief Clear the entire buffer.
 */
void buffertop_clear(
  BufferTop* bufferTop ///< "this" pointer.
);

/**
 * @brief Destroy the buffer: clear it, and free 'bufferTop' pointer.
 */
void buffertop_destroy(
  BufferTop* bufferTop ///< "this" pointer.
);

#endif
