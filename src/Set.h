/**
 * @file Set.h
 */

#ifndef CGDS_SET_H
#define CGDS_SET_H

#include <stdlib.h>
#include <string.h>
#include "cgds/safe_alloc.h"
#include "cgds/types.h"
#include "cgds/Vector.h"

/**
 * @brief Cell of a set.
 */
typedef struct SetCell {
  void* item; ///< Generic data (key) contained in this cell.
  struct SetCell* next; ///< Pointer to next cell in the list.
} SetCell;

/**
 * @brief Generic set containing any data (of same size).
 */
typedef struct Set {
  UInt size; ///< Count elements in the set.
  size_t dataSize; ///< Size of a set cell element in bytes.
  size_t hashSize; ///< (Maximum) Number of stored hash keys.
  SetCell** head; ///< Pointers to the first cell in a list.
  UInt (*getHash)(void*, size_t); ///< Custom hash function (optional)
} Set;

/**
 * @brief Initialize an empty set.
 */
void _set_init(
  Set* set, ///< "this" pointer.
  size_t dataSize, ///< Size in bytes of a set element.
  size_t hashSize, ///< (Maximum) Number of stored hash keys.
  UInt (*getHash)(void*, size_t); ///< Custom hash function (optional)
);

/**
 * @brief Return an allocated and initialized set.
 */
Set* _set_new(
  size_t dataSize, ///< Size in bytes of a set element.
  size_t hashSize, ///< (Maximum) Number of stored hash keys.
  UInt (*getHash)(void*, size_t) ///< Custom hash function (nullable)
);

/**
 * @brief Return an allocated and initialized set.
 * @param type Type of a set element (int, char*, ...).
 * @param hsize Size of the internal pointers array.
 * @param getHash Custom hash function (nullable)
 *
 * Usage: Set* set_new(<Type> type, UInt hash_size, UInt (*getHash)(void*, size_t))
 */
#define set_new(type, hsize, getHash) \
  _set_new(sizeof(type), hsize, getHash)

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
Set* set_copy(
  Set* set ///< "this" pointer.
);

/**
 * @brief Check if the set is empty.
 */
bool set_empty(
  Set* set ///< "this" pointer.
);

/**
 * @brief Return current size.
 */
UInt set_size(
  Set* set ///< "this" pointer.
);

/**
 * @brief Lookup given element.
 *
 * Usage: bool set_has(Set* set, void* item)
 */
bool set_has(
  Set* set, ///< "this" pointer.
  void* item ///< Element to search.
);

/**
 * @brief Add an item to the set.
 */
void _set_add(
  Set* set, ///< "this" pointer.
  void* item ///< Element to add.
);

/**
 * @brief Add a key to the set.
 * @param set "this" pointer.
 * @param item Element to add.
 *
 * Usage: void set_add(Set* set, void item)
 */
#define set_add(set, item) \
{ \
  typeof(item) tmp = item; \
  _set_add(set, &tmp); \
}

/**
 * @brief Remove the given item.
 */
void _set_delete(
  Set* set, ///< "this" pointer.
  void* item ///< Element to delete.
);

/**
 * @brief Remove the given item.
 * @param item Element to remove.
 *
 * Usage: void set_delete(Set* set, void item)
 */
#define set_delete(set, item) \
{ \
  typeof(item) tmp = item; \
  _set_delete(set, &tmp); \
}

/**
 * @brief Initialize a vector with (pointers to) set elements.
 */
Vector* set_to_vector(
  Set* set ///< "this" pointer.
);

/**
 * @brief Clear the entire set.
 */
void set_clear(
  Set* set ///< "this" pointer.
);

/**
 * @brief Destroy the set: clear it, and free hashes array.
 */
void set_destroy(
  Set* set ///< "this" pointer.
);

#endif
