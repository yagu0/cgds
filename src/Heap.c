/**
 * @file Heap.c
 */

#include "cgds/Heap.h"

// NOTE: no init() method here, since Heap has no specific initialization

Heap* _heap_new(size_t dataSize, OrderType hType, UInt arity)
{
  Heap* heap = (Heap*) safe_malloc(sizeof(Heap));
  heap->arity = arity;
  heap->hType = hType;
  heap->items = _vector_new(dataSize);
  heap->values = _vector_new(sizeof(Real));
  return heap;
}

Heap* heap_copy(Heap* heap)
{
  Heap* heapCopy = (Heap*) safe_malloc(sizeof(Heap));
  heapCopy->arity = heap->arity;
  heapCopy->hType = heap->hType;
  heapCopy->items = vector_copy(heap->items);
  heapCopy->values = vector_copy(heap->values);
  return heapCopy;
}

bool heap_empty(Heap* heap)
{
  return vector_empty(heap->items);
}

UInt heap_size(Heap* heap)
{
  return vector_size(heap->items);
}

// NOTE: [perf] in two following methods, full heap[k] exchanges are
// not needed; we keep track of the moving element without assigning it at
// every step, thus saving array accesses and affectations + 'aux' tmp memory.
// --> this is not the most natural way of writing these functions.

void _heap_bubble_up(Heap* heap, UInt startIndex)
{
  if (startIndex == 0)
    // Nothing to do in this case
    return;
  UInt currentIndex = startIndex;
  void* startItem = _vector_get(heap->items, startIndex);
  Real startValue = *((Real*)(_vector_get(heap->values, startIndex)));
  bool startItemReallocated = false;
  while (true)
  {
    bool land = (currentIndex == 0), //at root: can't go up
         applyExchange = true;
    if (!land)
    {
      // Get parent and compare to it
      UInt nextIndex = (currentIndex - 1) / heap->arity;
      Real nextValue = *((Real*)(_vector_get(heap->values, nextIndex)));
      land = (
        (heap->hType == MIN_T && startValue >= nextValue) ||
        (heap->hType == MAX_T && startValue <= nextValue)
      );
      if (!land)
      {
        // Move one level up: the parent goes one level down
        if (currentIndex == startIndex)
        {
          // Save startItem (because *startItem is about to be changed)
          void* newStartItem = safe_malloc(heap->items->dataSize);
          memcpy(newStartItem, startItem, heap->items->dataSize);
          startItem = newStartItem;
          startItemReallocated = true;
        }
        void* nextItem = _vector_get(heap->items, nextIndex);
        _vector_set(heap->items, currentIndex, nextItem);
        _vector_set(heap->values, currentIndex, &nextValue);
        currentIndex = nextIndex;
        continue;
      }
      // At correct relative place: will now land
      if (currentIndex == startIndex)
        applyExchange = false;
    }
    if (applyExchange)
    {
      _vector_set(heap->items, currentIndex, startItem);
      _vector_set(heap->values, currentIndex, &startValue);
    }
    break;
  }
  if (startItemReallocated)
    safe_free(startItem);
}

void _heap_bubble_down(Heap* heap, UInt startIndex)
{
  if (startIndex * heap->arity + 1 >= heap->items->size)
    // Nothing to do: already in a leaf
    return;
  UInt currentIndex = startIndex;
  void* startItem = _vector_get(heap->items, startIndex);
  Real startValue = *((Real*)(_vector_get(heap->values, startIndex)));
  bool startItemReallocated = false;
  while (true)
  {
    bool land = (currentIndex * heap->arity + 1 >= heap->items->size),
         applyExchange = true;
    if (!land)
    {
      // Find top child (min or max)
      UInt topChildIndex;
      Real topChildValue = (heap->hType == MIN_T ? INFINITY : -INFINITY);
      for (UInt i = 1; i <= heap->arity; i++)
      {
        UInt childIndex = currentIndex * heap->arity + i;
        if (childIndex >= heap->items->size)
          break;
        Real childValue = *((Real*)(_vector_get(heap->values, childIndex)));
        if (
          (heap->hType == MIN_T && childValue < topChildValue) ||
          (heap->hType == MAX_T && childValue > topChildValue)
        ) {
          topChildIndex = childIndex;
          topChildValue = childValue;
        }
      }
      // Compare to top child
      land = (
        (heap->hType == MIN_T && startValue <= topChildValue) ||
        (heap->hType == MAX_T && startValue >= topChildValue)
      );
      if (!land)
      {
        // Move one level down: the child goes one level up
        if (currentIndex == startIndex)
        {
          // Save startItem (because *startItem is about to be changed)
          void* newStartItem = safe_malloc(heap->items->dataSize);
          memcpy(newStartItem, startItem, heap->items->dataSize);
          startItem = newStartItem;
          startItemReallocated = true;
        }
        void* topChildItem = _vector_get(heap->items, topChildIndex);
        _vector_set(heap->items, currentIndex, topChildItem);
        _vector_set(heap->values, currentIndex, &topChildValue);
        currentIndex = topChildIndex;
        continue;
      }
      // At correct relative place: will now land
      if (currentIndex == startIndex)
        applyExchange = false;
    }
    if (applyExchange)
    {
      // Moving element has landed (in a leaf): apply final affectation
      _vector_set(heap->items, currentIndex, startItem);
      _vector_set(heap->values, currentIndex, &startValue);
    }
    break;
  }
  if (startItemReallocated)
    safe_free(startItem);
}

void _heap_insert(Heap* heap, void* item, Real value)
{
  _vector_push(heap->items, item);
  _vector_push(heap->values, &value);
  _heap_bubble_up(heap, heap->items->size - 1);
}

Int _heap_get_index(Heap* heap, void* item)
{
  for (Int index = 0; index < heap->items->size; index++)
  {
    if (
      memcmp(
        heap->items->datas + index * heap->items->dataSize,
        item,
        heap->items->dataSize
      ) == 0
    ) {
      return index;
    }
  }
  return -1;
}

void _heap_modify(Heap* heap, void* item, Real newValue)
{
  // First, find index of the item:
  const Int index = _heap_get_index(heap, item);
  if (index < 0)
    // Element not found
    return;
  Real oldValue = *((Real*)_vector_get(heap->values, index));
  _vector_set(heap->values, index, &newValue);
  if (
    (heap->hType == MIN_T && newValue > oldValue) ||
    (heap->hType == MAX_T && newValue < oldValue)
  ) {
    _heap_bubble_down(heap, index);
  }
  else
    _heap_bubble_up(heap, index);
}

void _heap_remove_at_index(Heap* heap, Int index)
{
  const bool removeLast = (index == heap->items->size - 1);
  if (!removeLast)
  {
    _vector_set(
      heap->items,
      index,
      _vector_get(heap->items, heap->items->size - 1));
    _vector_set(
      heap->values,
      index,
      _vector_get(heap->values, heap->values->size - 1));
  }
  vector_pop(heap->items);
  vector_pop(heap->values);
  if (!removeLast && heap->items->size > 0)
    _heap_bubble_down(heap, index);
}

void _heap_remove(Heap* heap, void* item)
{
  // First, find index of the item:
  const Int index = _heap_get_index(heap, item);
  if (index >= 0)
    _heap_remove_at_index(heap, index);
}

ItemValue _heap_top(Heap* heap)
{
  ItemValue top;
  top.item = _vector_get(heap->items, 0);
  vector_get(heap->values, 0, top.value);
  return top;
}

void heap_pop(Heap* heap)
{
  _heap_remove_at_index(heap, 0);
}

void heap_clear(Heap* heap)
{
  vector_clear(heap->items);
  vector_clear(heap->values);
}

void heap_destroy(Heap* heap)
{
  vector_destroy(heap->items);
  vector_destroy(heap->values);
  safe_free(heap);
}
