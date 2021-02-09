/**
 * @file BufferTop.c
 */

#include "cgds/BufferTop.h"

// NOTE: no init() method here, since BufferTop has no specific initialization

BufferTop* _buffertop_new(
  size_t dataSize, UInt capacity, OrderType bType, UInt arity)
{
  BufferTop* bufferTop = (BufferTop*) safe_malloc(sizeof (BufferTop));
  bufferTop->capacity = capacity;
  bufferTop->bType = bType; //redondant, but facilitate understanding
  // WARNING: heap must have opposite type: "smallest" element first
  bufferTop->heap =
    _heap_new(dataSize, (bType == MAX_T ? MIN_T : MAX_T), arity);
  return bufferTop;
}

BufferTop* buffertop_copy(BufferTop* bufferTop)
{
  BufferTop* bufferTopCopy = (BufferTop*) safe_malloc(sizeof (BufferTop));
  bufferTopCopy->capacity = bufferTop->capacity;
  bufferTopCopy->bType = bufferTop->bType;
  bufferTopCopy->heap = heap_copy(bufferTop->heap);
  return bufferTopCopy;
}

List* buffertop_2list(BufferTop* bufferTop)
{
  // Copy the buffer, and then use the copy to build the list
  BufferTop* bufferTopCopy = buffertop_copy(bufferTop);
  List* bufferInList = _list_new(bufferTop->heap->items->dataSize);
  while (!buffertop_empty(bufferTopCopy))
  {
    void* topItem = _heap_top(bufferTopCopy->heap).item;
    // NOTE: list_insert_front(), to reverse (wrong) items order
    // ==> in the returned list, top element is at head.
    _list_insert_front(bufferInList, topItem);
    buffertop_pop(bufferTopCopy);
  }
  buffertop_destroy(bufferTopCopy);
  return bufferInList;
}

bool buffertop_empty(BufferTop* bufferTop)
{
  return (heap_size(bufferTop->heap) == 0);
}

UInt buffertop_size(BufferTop* bufferTop)
{
  return heap_size(bufferTop->heap);
}

void _buffertop_tryadd(BufferTop* bufferTop, void* item, Real value)
{
  if (heap_size(bufferTop->heap) >= bufferTop->capacity) {
    Real topValue = *((Real*) (bufferTop->heap->values->datas));
    if (
      (bufferTop->bType == MIN_T && value >= topValue) ||
      (bufferTop->bType == MAX_T && value <= topValue)
    ) {
      // Shortcut : if value "worse" than top->value and buffer is full, skip
      return;
    }
  }

  // Insertion somewhere in the item-values heap
  _heap_insert(bufferTop->heap, item, value);

  if (heap_size(bufferTop->heap) > bufferTop->capacity)
    // We must remove current root
    heap_pop(bufferTop->heap);
}

ItemValue _buffertop_first(BufferTop* bufferTop)
{
  return _heap_top(bufferTop->heap);
}

void buffertop_pop(BufferTop* bufferTop)
{
  heap_pop(bufferTop->heap);
}

void buffertop_clear(BufferTop* bufferTop)
{
  heap_clear(bufferTop->heap);
}

void buffertop_destroy(BufferTop* bufferTop)
{
  heap_destroy(bufferTop->heap);
  safe_free(bufferTop);
}
