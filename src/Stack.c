/**
 * @file Stack.c
 */

#include "cgds/Stack.h"

void _stack_init(Stack* stack, size_t dataSize)
{
	stack->size = 0;
	stack->dataSize = dataSize;
	stack->top = NULL;
}

Stack* _stack_new(size_t dataSize)
{
	Stack* stack = (Stack*) safe_malloc(sizeof (Stack));
	_stack_init(stack, dataSize);
	return stack;
}

Stack* stack_copy(Stack* stack)
{
	// since a stack is a single-linked list, an auxiliary storage is required
	void** tmpStorage = (void**) malloc(stack->size * sizeof (void*));
	StackCell* stackCell = stack->top;
	for (UInt i = 0; i < stack->size; i++)
	{
		tmpStorage[stack->size - 1 - i] = stackCell->data;
		stackCell = stackCell->previous;
	}
	// now transfer tmpStorage contents (pushed in right order) in a new stack
	Stack* stackCopy = _stack_new(stack->dataSize);
	for (UInt i = 0; i < stack->size; i++)
		_stack_push(stackCopy, tmpStorage[i]);
	free(tmpStorage);
	return stackCopy;
}

Bool stack_empty(Stack* stack)
{
	return (stack->size == 0);
}

UInt stack_size(Stack* stack)
{
	return stack->size;
}

void _stack_push(Stack* stack, void* data)
{
	StackCell* newStackTop = (StackCell*) safe_malloc(sizeof (StackCell));
	newStackTop->data = safe_malloc(stack->dataSize);
	memcpy(newStackTop->data, data, stack->dataSize);
	newStackTop->previous = stack->top;
	stack->top = newStackTop;
	stack->size++;
}

void* _stack_top(Stack* stack)
{
	return stack->top->data;
}

void stack_pop(Stack* stack)
{
	StackCell* toTrash = stack->top;
	stack->top = stack->top->previous;
	safe_free(toTrash->data);
	safe_free(toTrash);
	stack->size--;
}

void stack_clear(Stack* stack)
{
	while (stack->size > 0) 
		stack_pop(stack);
	_stack_init(stack, stack->dataSize);
}

void stack_destroy(Stack* stack)
{
	stack_clear(stack);
	safe_free(stack);
}
