/**
 * @file Set.c
 */

#include "cgds/Set.h"

void _set_init(Set* set, size_t dataSize, size_t hashSize,
               UInt (*getHash)(void*, size_t))
{
  set->hashSize = hashSize;
  set->dataSize = dataSize;
  set->head = safe_malloc(hashSize * sizeof(SetCell*));
  for (UInt i = 0; i < hashSize; i++)
    set->head[i] = NULL;
  set->size = 0;
  set->getHash = getHash; //may be NULL
}

Set* _set_new(size_t dataSize, size_t hashSize, UInt (*getHash)(void*, size_t))
{
  Set* set = (Set*) safe_malloc(sizeof(Set));
  _set_init(set, dataSize, hashSize, getHash);
  return set;
}

Set* set_copy(Set* set)
{
  Set* setCopy = _set_new(set->dataSize, set->hashSize, set->getHash);
  setCopy->size = set->size;
  for (UInt i = 0; i < set->hashSize; i++)
  {
    SetCell *cell = set->head[i],
             *cellCopy = setCopy->head[i],
             *prev = NULL;
    while (cell != NULL)
    {
      // cellCopy == NULL (from empty list)
      cellCopy = (SetCell*) safe_malloc(sizeof(SetCell));
      cellCopy->item = safe_malloc(set->dataSize);
      memcpy(cellCopy->item, cell->item, set->dataSize);
      if (prev == NULL) setCopy->head[i] = cellCopy;
      else prev->next = cellCopy;
      prev = cellCopy;
      cell = cell->next;
    }
    if (cellCopy != NULL) cellCopy->next = NULL;
  }
  return setCopy;
}

bool set_empty(Set* set)
{
  return (set->size == 0);
}

UInt set_size(Set* set)
{
  return set->size;
}

// Function (string) key --> (integer) hash [internal usage]
// Default function. Can be changed (see hashtable_new())
UInt _set_compute_hash(void* key, size_t dataSize, size_t hashSize)
{
  UInt res = 0;
  // Interpret the bytes in key as a piece of string
  unsigned char* keyStr = (unsigned char*)key;
  for (size_t i = 0; i < dataSize; i++)
    // NOTE: '31' from here https://stackoverflow.com/a/4384446
    res = (*(keyStr+i) + 31 * res) % hashSize;
  return res;
}

// Get hash index from key [internal usage]
UInt _set_get_hindex(Set* set, void* key)
{
  if (set->getHash == NULL)
    return _set_compute_hash(key, set->dataSize, set->hashSize);
  return set->getHash(key, set->hashSize);
}

bool set_has(Set* set, void* item)
{
  UInt hashIdx = _set_get_hindex(set, item);
  SetCell* cell = set->head[hashIdx];
  while (cell != NULL)
  {
    if (memcmp(cell->item, item, set->dataSize) == 0)
      return true;
    cell = cell->next;
  }
  return false;
}

void _set_add(Set* set, void* item)
{
  UInt hashIdx = _set_get_hindex(set, item);
  SetCell *cell = set->head[hashIdx],
          *prev = NULL;
  while (cell != NULL)
  {
    if (memcmp(cell->item, item, set->dataSize) == 0)
      // Already here: nothing to do
      return;
    prev = cell;
    cell = cell->next;
  }
  // New element: insert after prev (which may be NULL)
  SetCell* newCell = (SetCell*) safe_malloc(sizeof(SetCell));
  newCell->item = safe_malloc(set->dataSize);
  memcpy(newCell->item, item, set->dataSize);
  newCell->next = NULL;
  if (prev == NULL)
    set->head[hashIdx] = newCell;
  else
    prev->next = newCell;
  set->size++;
}

void _set_delete(Set* set, void* item)
{
  UInt hashIdx = _set_get_hindex(set, item);
  SetCell *cell = set->head[hashIdx],
          *prev = NULL;
  while (cell != NULL)
  {
    if (memcmp(cell->item, item, set->dataSize) == 0)
    {
      if (prev == NULL)
        set->head[hashIdx] = cell->next;
      else
        prev->next = cell->next;
      safe_free(cell->item);
      safe_free(cell);
      set->size--;
      break;
    }
    prev = cell;
    cell = cell->next;
  }
}

Vector* set_to_vector(Set* set) {
  Vector* v = _vector_new(set->dataSize);
  for (UInt i = 0; i < set->hashSize; i++) {
    SetCell* cell = set->head[i];
    while (cell != NULL) {
      _vector_push(v, cell->item);
      cell = cell->next;
    }
  }
  return v;
}

void set_clear(Set* set)
{
  for (UInt i = 0; i < set->hashSize; i++)
  {
    SetCell* cell = set->head[i];
    while (cell != NULL)
    {
      SetCell* next = cell->next;
      safe_free(cell->item);
      safe_free(cell);
      cell = next;
    }
    set->head[i] = NULL;
  }
  set->size = 0;
}

void set_destroy(Set* set)
{
  set_clear(set);
  safe_free(set->head);
  safe_free(set);
}
