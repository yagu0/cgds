/**
 * @file Heap.c
 */

#include "cgds/Heap.h"

// NOTE: no _init() method here, since Heap has no specific initialization

Heap* _heap_new(size_t dataSize, OrderType hType, UInt arity)
{
	Heap* heap = (Heap*)safe_malloc(sizeof(Heap));
	heap->arity = arity;
	heap->hType = hType;
	heap->dataSize = dataSize;
	heap->array = _vector_new(sizeof(ItemValue));
	return heap;
}

Heap* heap_copy(Heap* heap)
{
	Heap* heapCopy = _heap_new(heap->dataSize, heap->hType, heap->arity);
	// HACK: vector_copy is not enough,
  // since we also have to allocate ItemValue(->item)
	heapCopy->array->size = heap->array->size;
	heapCopy->array->capacity = heap->array->capacity;
	heapCopy->array->datas =
    (void**)safe_malloc(heap->array->capacity*sizeof(void*));
	for (UInt i=0; i<heap->array->size; i++)
	{
		heapCopy->array->datas[i] = safe_malloc(sizeof(ItemValue));
		ItemValue itemValueCopy = (ItemValue){
			.item=safe_malloc(heap->dataSize),
			.value=((ItemValue*)(heap->array->datas[i]))->value};
		memcpy(
      itemValueCopy.item,
      ((ItemValue*)(heap->array->datas[i]))->item,
      heap->dataSize);
		memcpy(heapCopy->array->datas[i], &itemValueCopy, sizeof(ItemValue));
	}
	return heapCopy;
}

bool heap_empty(Heap* heap)
{
	return vector_empty(heap->array);
}

UInt heap_size(Heap* heap)
{
	return vector_size(heap->array);
}

// NOTE: [perf] in two following methods, full heap[k] exchanges are
// not needed; we keep track of the moving element without assigning it at
// every step, thus saving array accesses and affectations + 'aux' tmp memory.
// --> this is not the most natural way of writing these functions.

void _heap_bubble_up(Heap* heap, UInt startIndex)
{
	UInt currentIndex = startIndex;
	ItemValue* startItemValue = heap->array->datas[startIndex];
	while (true)
	{
		// get parent
		UInt nextIndex = currentIndex / heap->arity;
		Real nextValue = ((ItemValue*)(heap->array->datas[nextIndex]))->value;
		// compare to parent (if applicable)
		if (currentIndex == 0 ||
			(heap->hType == MIN_T && startItemValue->value >= nextValue) ||
			(heap->hType == MAX_T && startItemValue->value <= nextValue))
		{
			// moving element has landed: apply final affectation
			heap->array->datas[currentIndex] = startItemValue;
			break;
		}
		// move one level up: the parent goes one level down
		heap->array->datas[currentIndex] = heap->array->datas[nextIndex];
		currentIndex = nextIndex;
	}
}

void _heap_bubble_down(Heap* heap, UInt startIndex)
{
	UInt currentIndex = startIndex;
	ItemValue* startItemValue = heap->array->datas[startIndex];
	while (true)
	{
		if (currentIndex * heap->arity >= heap->array->size)
		{
			// moving element has landed (in a leaf): apply final affectation
			heap->array->datas[currentIndex] = startItemValue;
			break;
		}
		// find top child (min or max)
		UInt topChildIndex;
		Real topChildValue = (heap->hType == MIN_T ? INFINITY : -INFINITY);
		for (Int i=0; i<heap->arity; i++)
		{
			UInt childIndex = i + currentIndex * heap->arity;
			if (childIndex >= heap->array->size)
				break;
			Real childValue = ((ItemValue*)(heap->array->datas[childIndex]))->value;
			if ((heap->hType == MIN_T && childValue < topChildValue) ||
				(heap->hType == MAX_T && childValue > topChildValue))
			{
				topChildIndex = childIndex;
				topChildValue = childValue;
			}
		}
		// compare to top child
		if ((heap->hType == MIN_T && startItemValue->value > topChildValue) ||
			(heap->hType == MAX_T && startItemValue->value < topChildValue))
		{
			// move one level down: the child goes one level up
			heap->array->datas[currentIndex] = heap->array->datas[topChildIndex];
		}
		else
		{
			// moving element has landed: apply final affectation
			heap->array->datas[currentIndex] = startItemValue;
			break;
		}
		currentIndex = topChildIndex;
	}
}

void _heap_insert(Heap* heap, void* item, Real value)
{
	ItemValue itemValue =
    (ItemValue){.item=safe_malloc(heap->dataSize), .value=value};
	memcpy(itemValue.item, item, heap->dataSize);
	_vector_push(heap->array, &itemValue);
	_heap_bubble_up(heap, heap->array->size-1);
}

void _heap_modify(Heap* heap, UInt index, Real newValue)
{
	double oldValue = ((ItemValue*)(heap->array->datas[index]))->value;
	((ItemValue*)(heap->array->datas[index]))->value = newValue;
	if ((heap->hType == MIN_T && newValue > oldValue) ||
		(heap->hType == MAX_T && newValue < oldValue))
	{
		_heap_bubble_down(heap, index);
	}
	else
		_heap_bubble_up(heap, index);
}

void _heap_remove(Heap* heap, UInt index)
{
	safe_free(((ItemValue*)(heap->array->datas[index]))->item);
	ItemValue* tmp = heap->array->datas[index];
	heap->array->datas[index] = heap->array->datas[heap_size(heap)-1];
	heap->array->datas[heap_size(heap)-1] = tmp;
	vector_pop(heap->array);
	if (heap->array->size > 0)
		_heap_bubble_down(heap, index);
}

ItemValue* heap_top_raw(Heap* heap)
{
	return (ItemValue*)(heap->array->datas[0]);
}

void heap_pop(Heap* heap)
{
	_heap_remove(heap, 0);
}

void heap_clear(Heap* heap)
{
	for (UInt i = 0; i < heap->array->size; i++)
	{
		// Extra memory releases which wouldn't be done in vector_clear()
		safe_free(((ItemValue*)(heap->array->datas[i]))->item);
		//safe_free((ItemValue*)heap->array->datas[i]);
	}
	vector_clear(heap->array);
}

void heap_destroy(Heap* heap)
{
	heap_clear(heap);
	safe_free(heap->array);
	safe_free(heap);
}
