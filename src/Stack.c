/**
 * @file Stack.c
 */

#include "cgds/Stack.h"

void _stack_init(Stack* stack, size_t dataSize)
{
	stack->dataSize = dataSize;
	_vector_init(stack->array, dataSize);
}

Stack* _stack_new(size_t dataSize)
{
	Stack* stack = (Stack*) safe_malloc(sizeof (Stack));
	_stack_init(stack, dataSize);
	return stack;
}

Stack* stack_copy(Stack* stack)
{
	Stack* stackCopy = (Stack*) safe_malloc(sizeof (Stack));
	stackCopy->dataSize = stack->dataSize;
	Vector* arrayCopy = vector_copy(stack->array);
	stackCopy->array = arrayCopy;
	return stackCopy;
}

Bool stack_empty(Stack* stack)
{
	return vector_empty(stack->array);
}

UInt stack_size(Stack* stack)
{
	return vector_size(stack->array);
}

void _stack_push(Stack* stack, void* data)
{
	_vector_push(stack->array, data);
}

void* _stack_top(Stack* stack)
{
	return _vector_get(stack->array, vector_size(stack->array)-1);
}

void stack_pop(Stack* stack)
{
	vector_pop(stack->array);
}

void stack_clear(Stack* stack)
{
	vector_clear(stack->array);
}

void stack_destroy(Stack* stack)
{
	stack_clear(stack);
	safe_free(stack);
}
