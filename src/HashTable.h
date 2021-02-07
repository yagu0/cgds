/**
 * @file HashTable.h
 */

#ifndef CGDS_HASH_TABLE_H
#define CGDS_HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include "cgds/safe_alloc.h"
#include "cgds/types.h"

/**
 * @brief Cell of a dictionary.
 */
typedef struct HashCell {
  char* key; ///< Key (as a string).
  void* data; ///< Generic data contained in this cell.
  struct HashCell* next; ///< Pointer to next cell in the list.
} HashCell;

/**
 * @brief Generic dictionary string --> any data.
 */
typedef struct HashTable {
  UInt size; ///< Count elements in the dictionary.
  size_t dataSize; ///< Size of a dict cell element in bytes.
  size_t hashSize; ///< (Maximum) Number of stored hash keys.
  HashCell** head; ///< Pointers to the first cell in a list.
} HashTable;

/**
 * @brief Initialize an empty dictionary.
 */
void _hashtable_init(
  HashTable* hashTable, ///< "this" pointer.
  size_t dataSize, ///< Size in bytes of a dictionary element.
  size_t hashSize ///< (Maximum) Number of stored hash keys.
);

/**
 * @brief Return an allocated and initialized dictionary.
 */
HashTable* _hashtable_new(
  size_t dataSize, ///< Size in bytes of a dictionary element.
  size_t hashSize ///< (Maximum) Number of stored hash keys.
);

/**
 * @brief Return an allocated and initialized vector.
 * @param type Type of a vector element (int, char*, ...).
 *
 * Usage: HashTable* hashtable_new(<Type> type, UInt hash_size)
 */
#define hashtable_new(type, hsize) \
  _hashtable_new(sizeof(type), hsize)

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
HashTable* hashtable_copy(
  HashTable* hashTable ///< "this" pointer.
);

/**
 * @brief Check if the dictionary is empty.
 */
bool hashtable_empty(
  HashTable* hastTable ///< "this" pointer.
);

/**
 * @brief Return current size.
 */
UInt hashtable_size(
  HashTable* hastTable ///< "this" pointer.
);

/**
 * @brief Lookup element of given key.
 */
void* _hashtable_get(
  HashTable* hashTable, ///< "this" pointer.
  char* key ///< Key of the element to retrieve.
);

/**
 * @brief Lookup element of given key.
 * @param hashTable "this" pointer.
 * @param key Key of the element to retrieve..
 * @param data 'out' variable (ptr) to contain the result.
 *
 * Usage: void hashtable_get(HashTable* hashTable, char* key, void* data)
 */
#define hashtable_get(hashTable, key, data) \
{ \
  data = (typeof(data))_hashtable_get(hashTable, key); \
}

/**
 * @brief Add the entry (key, value) to dictionary.
 */
void _hashtable_set(
  HashTable* hashTable, ///< "this" pointer.
  char* key, ///< Key of the element to add or modify.
  void* data ///< Pointer to new data at given key.
);

/**
 * @brief Add the entry (key, value) to dictionary.
 * @param hashTable "this" pointer.
 * @param key Key of the element to add or modify.
 * @param data New data at given key.
 *
 * Usage: void hashtable_set(HashTable* hashTable, char* key, void data)
 */
#define hashtable_set(hashTable, key, data) \
{ \
  typeof(data) tmp = data; \
  _hashtable_set(hashTable, key, &tmp); \
}

/**
 * @brief Remove the given key (+ associated value).
 *
 * Usage: void hashtable_delete(HashTable* hashTable, char* key)
 */
void hashtable_delete(
  HashTable* hashTable, ///< "this" pointer.
  char* key ///< Key of the element to delete.
);

/**
 * @brief Clear the entire dictionary.
 */
void hashtable_clear(
  HashTable* hashTable ///< "this" pointer.
);

/**
 * @brief Destroy the dictionary: clear it, and free hashes array.
 */
void hashtable_destroy(
  HashTable* hashTable ///< "this" pointer.
);

#endif
