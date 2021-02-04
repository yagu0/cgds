/**
 * @file Stack.h
 */

#ifndef CGDS_STACK_H
#define CGDS_STACK_H

#include <stdlib.h>
#include <string.h>
#include "cgds/types.h"
#include "cgds/safe_alloc.h"
#include "cgds/Vector.h"

/**
 * @brief Stack containing generic data.
 */
typedef struct Stack {
  size_t dataSize; ///< Size in bytes of a stack element.
  Vector* array; ///< Internal data structure: resizeable array.
} Stack;

/**
 * @brief Initialize an empty stack.
 */
void _stack_init(
  Stack* stack, ///< "this" pointer.
  size_t dataSize ///< Size in bytes of a stack element.
);

/**
 * @brief Return an allocated and initialized stack.
 */
Stack* _stack_new(
  size_t dataSize ///< Size in bytes of a stack element.
);

/**
 * @brief Return an allocated and initialized stack.
 * @param type Type of a stack element (int, char*, ...).
 *
 * Usage: Stack* stack_new(<Type> type)
 */
#define stack_new(type) \
  _stack_new(sizeof(type))

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
Stack* stack_copy(
  Stack* stack ///< "this" pointer.
);

/**
 * @brief Check if the stack is empty.
 */
bool stack_empty(
  Stack* stack ///< "this" pointer.
);

/**
 * @brief Return size of the current stack.
 */
UInt stack_size(
  Stack* stack ///< "this" pointer.
);

/**
 * @brief Add something on top of the stack.
 */
void _stack_push(
  Stack* stack, ///< "this" pointer.
  void* data ///< Data to be added.
);

/**
 * @brief Add something on top of the stack.
 * @param stack "this" pointer.
 * @param data Data to be added.
 *
 * Usage: void stack_push(Stack* stack, void data)
 */
#define stack_push(stack, data) \
{ \
  typeof(data) tmp = data; \
  _stack_push(stack, &tmp); \
}

/**
 * @brief Return what is on top of the stack.
 */
void* _stack_top(
  Stack* stack ///< "this" pointer.
);

/**
 * @brief Return what is on top of the stack.
 * @param stack "this" pointer.
 * @param data Data to be assigned.
 *
 * Usage: void stack_top(Stack* stack, void data)
 */
#define stack_top(stack, data) \
{ \
  void* pData = _stack_top(stack); \
  data = *((typeof(&data))pData); \
}

/**
 * @brief Remove the top of the stack.
 */
void stack_pop(
  Stack* stack ///< "this" pointer.
);

/**
 * @brief Clear the entire stack.
 */
void stack_clear(
  Stack* stack ///< "this" pointer.
);

/**
 * @brief Destroy the stack: clear it, and free 'stack' pointer.
 */
void stack_destroy(
  Stack* stack ///< "this" pointer.
);

#endif
