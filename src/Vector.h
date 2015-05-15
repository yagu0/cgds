/**
 * @file Vector.h
 */

#ifndef CGDS_VECTOR_H
#define CGDS_VECTOR_H

#include <stdlib.h>
#include <string.h>
#include "cgds/safe_alloc.h"
#include "cgds/types.h"

//*************
// Vector logic
//*************

/**
 * @brief Generic resizable array.
 */
typedef struct Vector {
	void** datas; ///< Data array of fixed length (reallocated if needed).
	size_t dataSize; ///< Size in bytes of a vector element.
	UInt size; ///< Count elements in the vector.
	UInt capacity; ///< Current maximal capacity; always larger than size.
} Vector;

/**
 * @brief Initialize an empty vector.
 */
void _vector_init(
	Vector* vector, ///< "this" pointer.
	size_t dataSize ///< Size in bytes of a vector element.
);

/**
 * @brief Return an allocated and initialized vector.
 */
Vector* _vector_new(
	size_t dataSize ///< Size in bytes of a vector element.
);

/**
 * @brief Return an allocated and initialized vector.
 * @param type Type of a vector element (int, char*, ...).
 * 
 * Usage: Vector* vector_new(<Type> type)
 */
#define vector_new(type) \
	_vector_new(sizeof(type))

/**
 * @brief Copy constructor (works well if items do not have allocated sub-pointers).
 */
Vector* vector_copy(
	Vector* vector ///< "this" pointer.
);

/**
 * @brief Check if the vector is empty.
 */
Bool vector_empty(
	Vector* vector ///< "this" pointer.
);

/**
 * @brief Return current size.
 */
UInt vector_size(
	Vector* vector ///< "this" pointer.
);

/**
 * @brief Reallocate internal array.
 */
void _vector_realloc(
	Vector* vector, ///< "this" pointer.
	UInt newCapacity ///< New capacity of the vector (in number of elements).
);

/**
 * @brief Add data at the end.
 */
void _vector_push(
	Vector* vector, ///< "this" pointer.
	void* data ///< Data to be added.
);

/**
 * @brief Add data at the end.
 * @param vector "this" pointer.
 * @param data Data to be added.
 * 
 * Usage: void vector_push(Vector* vector, void data)
 */
#define vector_push(vector, data) \
{ \
	typeof((data)) tmp = data; \
	_vector_push(vector,&tmp); \
}

/**
 * @brief Remove the last pushed element.
 */
void vector_pop(
	Vector* vector ///< "this" pointer.
);

/**
 * @brief Get the element at given index.
 */
void* _vector_get(
	Vector* vector, ///< "this" pointer.
	UInt index ///< Index of the element to retrieve.
);

/**
 * @brief Get the element at given index.
 * @param vector "this" pointer.
 * @param index Index of the element to retrieve.
 * @param data 'out' variable to contain the result.
 * 
 * Usage: void vector_get(Vector* vector, size_t index, void data)
 */
#define vector_get(vector, index, data) \
{ \
	void* pData = _vector_get(vector,index); \
	data = *((typeof(&data))pData); \
}

/**
 * @brief Set the element at given index.
 */
void _vector_set(
	Vector* vector, ///< "this" pointer.
	UInt index, ///< Index of the element to be modified.
	void* data ///< Pointer to new data at given index.
);

/**
 * @brief Set the element at given index.
 * @param vector "this" pointer.
 * @param index Index of the element to be modified.
 * @param data New data at given index.
 * 
 * Usage: void vector_set(Vector* vector, size_t index, void data)
 */
#define vector_set(vector, index, data) \
{ \
	typeof((data)) tmp = data; \
	_vector_set(vector,index,&tmp); \
}

/**
 * @brief Clear the entire vector.
 */
void vector_clear(
	Vector* vector ///< "this" pointer.
);

/**
 * @brief Destroy the vector: clear it, and free 'vector' pointer.
 */
void vector_destroy(
	Vector* vector ///< "this" pointer.
);

//***************
// Iterator logic
//***************

/**
 * @brief Iterator on a generic vector.
 */
typedef struct VectorIterator {
	Vector* vector; ///< Vector to be iterated.
	void** current; ///< Current vector element.
} VectorIterator;

/**
 * @brief Obtain an iterator object, starting at vector beginning (index 0).
 */
VectorIterator* vector_get_iterator(
	Vector* vector ///< Pointer to the vector to iterate over.
);

/**
 * @brief (Re)set current position inside vector to beginning (0).
 */
void vectorI_reset_begin(
	VectorIterator* vectorI ///< "this" pointer.
);

/**
 * @brief (Re)set current position inside vector to end (vector->size-1).
 */
void vectorI_reset_end(
	VectorIterator* vectorI ///< "this" pointer.
);

/**
 * @brief Tell if there is some data at the current index.
 */
Bool vectorI_has_data(
	VectorIterator* vectorI ///< "this" pointer.
);

/**
 * @brief Get data contained at the current index.
 */
void* _vectorI_get(
	VectorIterator* vectorI ///< "this" pointer.
);

/**
 * @brief Get data contained at the current index.
 * @param vectorI "this" pointer.
 * @param data 'out' variable to contain the result.
 * 
 * Usage: void vectorI_get(VectorIterator* vectorI, void data);
 */
#define vectorI_get(vectorI, data) \
{ \
	void* pData = _vectorI_get(vectorI); \
	data = *((typeof(&data))pData); \
}

/**
 * @brief Set the element at current index.
 */
void _vectorI_set(
	VectorIterator* vectorI, ///< "this" pointer.
	void* data ///< Data to be assigned.
);

/**
 * @brief Set the element at current index.
 * @param vectorI "this" pointer.
 * @param data Data to be assigned.
 * 
 * Usage: void vectorI_set(VectorIterator* vectorI, void data)
 */
#define vectorI_set(vectorI, data) \
{ \
	typeof((data)) tmp = data; \
	_vectorI_set(vectorI,&tmp); \
}

/**
 * @brief Move current iterator position forward (toward last index).
 */
void vectorI_move_next(
	VectorIterator* vectorI ///< "this" pointer.
);

/**
 * @brief Move current iterator position backward (toward first index).
 */
void vectorI_move_prev(
	VectorIterator* vectorI ///< "this" pointer.
);

/**
 * @brief Free memory allocated for the iterator.
 */
void vectorI_destroy(
	VectorIterator* vectorI ///< "this" pointer.
);

#endif
