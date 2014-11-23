/**
 * @file safe_alloc.h
 * @brief Safe memory management.
 */

#ifndef CGDS_SAFE_ALLOC_H
#define CGDS_SAFE_ALLOC_H

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Wrapper around stdlib malloc function.
 * @return A pointer to the newly allocated area; exit program if fail.
 */
void* safe_malloc(
	size_t size ///< Size of the block to allocate, in bytes.
);

/**
 * @brief Wrapper around stdlib calloc function.
 * @return A pointer to the newly allocated area; exit program if fail.
 */
void* safe_calloc(
	size_t count, ///< Number of elements to allocate.
	size_t size ///< Size of the element to allocate, in bytes.
);

/**
 * @brief Wrapper around stdlib realloc function.
 * @return A pointer to the newly allocated area; exit program if fail.
 */
void* safe_realloc(
	void* ptr, ///< Pointer on the area to be relocated.
	size_t size ///< Size of the block to reallocate, in bytes.
);

/**
 * @brief Wrapper around stdlib free function.
 */
void safe_free(
	void* ptr ///< Pointer on the area to be destroyed.
);

#endif
