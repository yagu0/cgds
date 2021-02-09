/**
 * @file Heap.h
 */

#ifndef CGDS_HEAP_H
#define CGDS_HEAP_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cgds/types.h"
#include "cgds/Vector.h"
#include "cgds/safe_alloc.h"

/**
 * @brief Generic d-ary heap.
 */
typedef struct Heap {
  OrderType hType; ///< Type of heap: max first (MAX_T) or min first (MIN_T).
  UInt arity; ///< Arity of the underlying tree.
  Vector* items; ///< Vector of items (any type).
  Vector* values; ///< Vector of items values (real numbers).
} Heap;

/**
 * @brief Return an allocated and initialized heap.
 */
Heap* _heap_new(
  size_t dataSize, ///< Size in bytes of a heap element.
  OrderType hType, ///< Type of heap: max first (MAX_T) or min first (MIN_T).
  UInt arity ///< Arity of the underlying tree.
);

/**
 * @brief Return an allocated and initialized heap.
 * @param type Type of a buffer item (int, char*, ...).
 * @param hType Type of heap: max first (MAX_T) or min first (MIN_T).
 * @param arity Arity of the underlying tree.
 *
 * Usage: Heap* heap_new(<Type> type, OrderType hType, UInt arity)
 */
#define heap_new(type, hType, arity) \
  _heap_new(sizeof(type), hType, arity)

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
Heap* heap_copy(
  Heap* heap ///< "this" pointer.
);

/**
 * @brief Check if the heap is empty.
 */
bool heap_empty(
  Heap* heap ///< "this" pointer.
);

/**
 * @brief Return the size of current heap.
 */
UInt heap_size(
  Heap* heap ///< "this" pointer.
);

/**
 * @brief "Bubble up" an item-value inserted somewhere in the tree
 * in O(log(n)) operations.
 */
void _heap_bubble_up(
  Heap* heap, ///< "this" pointer.
  UInt startIndex ///< Index to bubble up.
);

/**
 * @brief "Bubble down" an item-value inserted somewhere in the tree
 * in O(log(n)) operations.
 */
void _heap_bubble_down(
  Heap* heap, ///< "this" pointer.
  UInt startIndex ///< Index to bubble down.
);

/**
 * @brief Insert a pair (item,value) inside the heap.
 */
void _heap_insert(
  Heap* heap, ///< "this" pointer.
  void* item, ///< Pointer to an item of type as defined in the constructor.
  Real value ///< Value associated with the item.
);

/**
 * @brief Insert a pair (item,value) inside the heap.
 * @param heap "this" pointer.
 * @param item Item of type as defined in the constructor.
 * @param value Value associated with the item.
 *
 * Usage: void heap_insert(Heap* heap, void item, Real value)
 */
#define heap_insert(heap, item, value) \
{ \
  typeof(item) tmp = item; \
  _heap_insert(heap, &tmp, value); \
}

/**
 * @brief Change the value of an item at a given index.
 */
void _heap_modify(
  Heap* heap, ///< "this" pointer.
  void* item, ///< Pointer to item to modify.
  Real newValue ///< New value for the item.
);

/**
 * @brief Change the value of an item inside the heap.
 * @param heap "this" pointer.
 * @param item Item to modify.
 * @param newValue New value for the item.
 * @note If several similar items are present, only the first is affected.
 *
 * Usage: void heap_modify(Heap* heap, void item, Real newValue)
 */
#define heap_modify(heap, item, newValue) \
{ \
  typeof(item) item_ = item; \
  _heap_modify(heap, &item_, newValue); \
}

/**
 * @brief Remove an item-value at a given index.
 */
void _heap_remove(
  Heap* heap, ///< "this" pointer.
  void* item ///< Pointer to item to remove.
);

/**
 * @brief Remove an item-value inside the heap.
 * @param heap "this" pointer.
 * @param item Item to remove.
 * @note If several similar items are present, only the first is deleted.
 *
 * Usage: void heap_remove(Heap* heap, void item)
 */
#define heap_remove(heap, item) \
{ \
  typeof(item) item_ = item; \
  _heap_remove(heap, &item_); \
}

/**
 * @brief Return what is at the beginning of the heap.
 */
ItemValue _heap_top(
  Heap* heap ///< "this" pointer.
);

/**
 * @brief Get the top heap element in 'item'.
 * @param heap "this" pointer.
 * @param item_ Item to affect.
 *
 * Usage: void heap_top(Heap* heap, void item_)
 */
#define heap_top(heap, item_) \
{ \
  ItemValue iv = _heap_top(heap); \
  item_ = *((typeof(item_)*) iv.item); \
}

/**
 * @brief Remove the top of the heap.
 */
void heap_pop(
  Heap* heap ///< "this" pointer.
);

/**
 * @brief Clear the entire heap.
 */
void heap_clear(
  Heap* heap ///< "this" pointer.
);

/**
 * @brief Destroy the heap: clear it, and free 'heap' pointer.
 */
void heap_destroy(
  Heap* heap ///< "this" pointer.
);

#endif
