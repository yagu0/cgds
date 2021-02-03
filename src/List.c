/**
 * @file List.c
 */

#include "cgds/List.h"

////////////////
// List logic //
////////////////

void _list_init(List* list, size_t dataSize)
{
  list->size = 0;
  list->dataSize = dataSize;
  list->head = NULL;
  list->tail = NULL;
}

List* _list_new(size_t dataSize)
{
  List* list = (List*) safe_malloc(sizeof (List));
  _list_init(list, dataSize);
  return list;
}

List* list_copy(List* list)
{
  List* listCopy = _list_new(list->dataSize);
  ListCell* listCell = list->head;
  while (listCell != NULL)
  {
    _list_insert_back(listCopy, listCell->data);
    listCell = listCell->next;
  }
  return listCopy;
}

bool list_empty(List* list)
{
  return (list->size == 0);
}

UInt list_size(List* list)
{
  return list->size;
}

void* _list_get(ListCell* listCell)
{
  return listCell->data;
}

void _list_set(List* list, ListCell* listCell, void* data)
{
  memcpy(listCell->data, data, list->dataSize);
}

void _list_insert_first_element(List* list, void* data)
{
  ListCell* newListCell = (ListCell*) safe_malloc(sizeof (ListCell));
  newListCell->data = safe_malloc(list->dataSize);
  memcpy(newListCell->data, data, list->dataSize);
  newListCell->prev = NULL;
  newListCell->next = NULL;
  list->head = newListCell;
  list->tail = newListCell;
  list->size = 1;
}

void _list_insert_before(List* list, ListCell* listCell, void* data)
{
  ListCell* newListCell = (ListCell*) safe_malloc(sizeof (ListCell));
  newListCell->data = safe_malloc(list->dataSize);
  memcpy(newListCell->data, data, list->dataSize);
  newListCell->prev = listCell->prev;
  newListCell->next = listCell;
  if (listCell->prev != NULL)
    listCell->prev->next = newListCell;
  else
    list->head = newListCell;
  listCell->prev = newListCell;
  list->size++;
}

void _list_insert_after(List* list, ListCell* listCell, void* data)
{
  ListCell* newListCell = (ListCell*) safe_malloc(sizeof (ListCell));
  newListCell->data = safe_malloc(list->dataSize);
  memcpy(newListCell->data, data, list->dataSize);
  newListCell->prev = listCell;
  newListCell->next = listCell->next;
  if (listCell->next != NULL)
    listCell->next->prev = newListCell;
  else
    list->tail = newListCell;
  listCell->next = newListCell;
  list->size++;
}

void _list_insert_front(List* list, void* data)
{
  if (list->head != NULL)
    _list_insert_before(list, list->head, data);
  else
    _list_insert_first_element(list, data);
}

void _list_insert_back(List* list, void* data)
{
  if (list->tail != NULL)
    _list_insert_after(list, list->tail, data);
  else
    _list_insert_first_element(list, data);
}

void list_remove(List* list, ListCell* listCell)
{
  if (listCell->prev != NULL)
    listCell->prev->next = listCell->next;
  else
    list->head = listCell->next;
  if (listCell->next != NULL)
    listCell->next->prev = listCell->prev;
  else
    list->tail = listCell->prev;
  safe_free(listCell->data);
  safe_free(listCell);
  list->size--;
}

void list_remove_front(List* list)
{
  list_remove(list, list->head);
}

void list_remove_back(List* list)
{
  list_remove(list, list->tail);
}

void list_clear(List* list)
{
  ListCell* current = list->head;
  while (current != NULL)
  {
    safe_free(current->data);
    ListCell* nextListCell = current->next;
    safe_free(current);
    current = nextListCell;
  }
  _list_init(list, list->dataSize);
}

void list_destroy(List* list)
{
  list_clear(list);
  safe_free(list);
}

////////////////////
// Iterator logic //
////////////////////

ListIterator* list_get_iterator(List* list)
{
  ListIterator* listI = (ListIterator*) safe_malloc(sizeof (ListIterator));
  listI->list = list;
  listI->current = NULL;
  listI_reset_head(listI);
  return listI;
}

void listI_reset_head(ListIterator* listI)
{
  listI->current = listI->list->head;
}

void listI_reset_tail(ListIterator* listI)
{
  listI->current = listI->list->tail;
}

bool listI_has_data(ListIterator* listI)
{
  return (listI->current != NULL);
}

void listI_remove(ListIterator* listI, Direction direction)
{
  ListCell* toTrash = listI->current;
  switch (direction)
  {
  case FORWARD:
    listI->current = listI->current->next;
    break;
  case BACKWARD:
    listI->current = listI->current->prev;
    break;
  }
  list_remove(listI->list, toTrash);
}

void listI_move_next(ListIterator* listI)
{
  if (listI->current != NULL)
    listI->current = listI->current->next;
}

void listI_move_prev(ListIterator* listI)
{
  if (listI->current != NULL)
    listI->current = listI->current->prev;
}

void listI_destroy(ListIterator* listI)
{
  safe_free(listI);
}
