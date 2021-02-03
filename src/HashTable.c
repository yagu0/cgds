/**
 * @file HashTable.c
 */

#include "cgds/HashTable.h"

void _hashtable_init(HashTable* hashTable, size_t dataSize, size_t hashSize)
{
	hashTable->hashSize = hashSize;
  hashTable->dataSize = dataSize;
	hashTable->head = safe_malloc(hashSize * sizeof(HashCell*));
	for (UInt i = 0; i < hashSize; i++)
    hashTable->head[i] = NULL;
  hashTable->size = 0;
}

HashTable* _hashtable_new(size_t dataSize, size_t hashSize)
{
	HashTable* hashTable = (HashTable*) safe_malloc(sizeof (HashTable));
	_hashtable_init(hashTable, dataSize, hashSize);
	return hashTable;
}

HashTable* hashtable_copy(HashTable* hashTable)
{
	HashTable* hashTableCopy =
    _hashtable_new(hashTable->dataSize, hashTable->hashSize);
	hashTableCopy->size = hashTable->size;
  for (UInt i = 0; i < hashTable->hashSize; i++)
	{
    HashCell *cell = hashTable->head[i],
             *cellCopy = hashTableCopy->head[i],
             *prev = NULL;
    while (cell != NULL)
    {
      // cellCopy == NULL (from empty list)
      cellCopy = (HashCell*) safe_malloc(sizeof(HashCell*));
		  cellCopy->key = (char*) safe_malloc(strlen(cell->key) + 1);
      strcpy(cellCopy->key, cell->key);
		  cellCopy->data = safe_malloc(hashTable->dataSize);
		  memcpy(cellCopy->data, cell->data, hashTable->dataSize);
      if (prev == NULL) hashTableCopy->head[i] = cellCopy;
      else prev->next = cellCopy;
      prev = cellCopy;
      cell = cell->next;
    }
    if (cellCopy != NULL) cellCopy->next = NULL;
	}
	return hashTableCopy;
}

bool hashtable_empty(HashTable* hashTable)
{
	return (hashTable->size == 0);
}

UInt hashtable_size(HashTable* hashTable)
{
	return hashTable->size;
}

// Function (string) key --> (integer) hash [internal usage]
UInt _compute_hash(char* key, size_t hashSize)
{
  UInt res = 0;
  for (unsigned char* s = key; *s != '\0'; s++)
    // NOTE: '31' from here https://stackoverflow.com/a/4384446
    res += *s + 31 * res;
  return res % hashSize;
}

void* _hashtable_get(HashTable* hashTable, char* key)
{
  UInt hashIdx = _compute_hash(key, hashTable->hashSize);
  HashCell* cell = hashTable->head[hashIdx];
  while (cell != NULL)
  {
    if (strcmp(cell->key, key) == 0) return cell->data;
    cell = cell->next;
  }
  return NULL;
}

void _hashtable_set(HashTable* hashTable, char* key, void* data)
{
  UInt hashIdx = _compute_hash(key, hashTable->hashSize);
  HashCell
    *cell = hashTable->head[hashIdx],
    *prev = NULL;
  while (cell != NULL)
  {
    if (strcmp(cell->key, key) == 0)
    {
      // Modify:
      memcpy(cell->data, data, hashTable->dataSize);
      return;
    }
    prev = cell;
    cell = cell->next;
  }
  // New element: insert after prev (which may be NULL)
  HashCell* newCell = (HashCell*) safe_malloc(sizeof(HashCell));
  newCell->key = (char*) safe_malloc(strlen(key) + 1);
  strcpy(newCell->key, key);
  newCell->data = safe_malloc(hashTable->dataSize);
  memcpy(newCell->data, data, hashTable->dataSize);
  newCell->next = NULL;
  if (prev == NULL)
    hashTable->head[hashIdx] = newCell;
  else
    prev->next = newCell;
  hashTable->size++;
}

void hashtable_delete(HashTable* hashTable, char* key)
{
  UInt hashIdx = _compute_hash(key, hashTable->hashSize);
  HashCell
    *cell = hashTable->head[hashIdx],
    *prev = NULL;
  while (cell != NULL)
  {
    if (strcmp(cell->key, key) == 0)
    {
      if (prev == NULL)
        hashTable->head[hashIdx] = cell->next;
      else
        prev->next = cell->next;
      safe_free(cell->key);
      safe_free(cell->data);
      safe_free(cell);
      hashTable->size--;
      break;
    }
    prev = cell;
    cell = cell->next;
  }
}

void hashtable_clear(HashTable* hashTable)
{
	for (UInt i = 0; i < hashTable->hashSize; i++)
  {
    HashCell* cell = hashTable->head[i];
    while (cell != NULL)
    {
      HashCell* next = cell->next;
      safe_free(cell->key);
      safe_free(cell->data);
      safe_free(cell);
      cell = next;
    }
    hashTable->head[i] = NULL;
  }
	hashTable->size = 0;
}

void hashtable_destroy(HashTable* hashTable)
{
	hashtable_clear(hashTable);
	safe_free(hashTable->head);
  safe_free(hashTable);
}
