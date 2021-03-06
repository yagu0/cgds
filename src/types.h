/**
 * @file types.h
 * @brief A few useful data types.
 */

#ifndef CGDS_TYPES_H
#define CGDS_TYPES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Signed integer type.
 */
typedef int64_t Int;

/**
 * @brief Unsigned integer type.
 */
typedef uint64_t UInt;

/**
 * @brief Data type for a real number.
 */
typedef double Real;

/**
 * @brief Enumeration for the type of buffer or heap.
 */
typedef enum {
  MIN_T = 0, ///< Minimum element first.
  MAX_T = 1 ///< Maximum element first.
} OrderType;

/**
 * @brief Generic item-value type; 'value' may correspond e.g. to distance.
 */
typedef struct ItemValue {
  void* item; ///< Pointer to an item of any type.
  Real value; ///< Value associated with the item.
} ItemValue;

#endif
