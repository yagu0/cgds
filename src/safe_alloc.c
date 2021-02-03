/**
 * @file safe_alloc.c
 */

#include "cgds/safe_alloc.h"

void* safe_malloc(size_t size)
{
  void* res = malloc(size);
  if (res == NULL)
  {
    fprintf(stderr, "Error: unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  return res;
}

void* safe_calloc(size_t count, size_t size)
{
  void* res = calloc(count, size);
  if (res == NULL)
  {
    fprintf(stderr, "Error: unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  return res;
}

void* safe_realloc(void* ptr, size_t size)
{
  void* res = realloc(ptr, size);
  if (res == NULL)
  {
    fprintf(stderr, "Error: unable to reallocate memory\n");
    exit(EXIT_FAILURE);
  }
  return res;
}

void safe_free(void* ptr)
{
  if (ptr != NULL)
    free(ptr);
}
