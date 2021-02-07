#include <stdlib.h>
#include "cgds/Set.h"
#include "helpers.h"
#include "lut.h"

UInt getHash_int(void* item, size_t hashSize) {
  return *((int*)item) % hashSize;
}

void t_set_clear()
{
  Set* s = set_new(int, 16, getHash_int);
  lu_assert(set_empty(s));

  set_add(s, 0);
  set_add(s, 1);
  set_add(s, 2);

  set_destroy(s);
  s = set_new(int, 8, getHash_int);

  set_add(s, 1);
  set_add(s, 2);
  set_add(s, 3);

  set_clear(s);
  lu_assert(set_empty(s));

  set_destroy(s);
}

void t_set_size()
{
  Set* s = set_new(int, 16, getHash_int);
  lu_assert(set_empty(s));

  set_add(s, 0);
  set_add(s, 1);
  set_add(s, 2);
  lu_assert_int_eq(set_size(s), 3);

  set_add(s, 3);
  set_add(s, 4);
  lu_assert_int_eq(set_size(s), 5);

  set_add(s, 5);
  set_add(s, 6);
  set_add(s, 7);
  lu_assert_int_eq(set_size(s), 8);

  set_destroy(s);
}

void t_set_add_remove_basic()
{
  int n = 10;

  Set* s = set_new(double, 4, NULL);
  for (double i = 0.0; i < n; i++)
    set_add(s, i);
  lu_assert_int_eq(set_size(s), n);

  // Check values
  for (double i = 0.0; i < n; i++)
    lu_assert(set_has(s, &i));

  // Remove items
  for (double i = 0.0; i < n; i++)
    set_delete(s, i);
  lu_assert_int_eq(set_size(s), 0);

  set_destroy(s);
}

void t_set_getnull_modify()
{
  int n = 10;

  Set* s = set_new(StructTest1, 4, NULL);
  // NOTE: using calloc() because probably StructTest1 is 4 + 8 bytes,
  // aligned to 8 + 8 ==> 4 are left uninitialized ==> memcpy compares
  // some junk values. TODO: custom equality function instead.
  StructTest1* st1 = (StructTest1*) calloc(n , sizeof(StructTest1));
  for (int i = 0; i < n; i++)
  {
    st1[i].a = random() % 42;
    st1[i].b = (double) random() / RAND_MAX;
    set_add(s, *(st1 + i));
  }
  for (int i = 0; i < n; i++)
  {
    // Another way to access elements
    StructTest1* st1Cell = (StructTest1*) calloc(1, sizeof(StructTest1));
    st1Cell->a = st1[7].a;
    st1Cell->b = st1[7].b;
    lu_assert(set_has(s, st1Cell));
    free(st1Cell);
  }

  // has / has not:
  StructTest1* stmp = (StructTest1*) calloc(1, sizeof(StructTest1));
  stmp->a = 51;
  stmp->b = 2.0;
  lu_assert(!set_has(s, stmp));
  free(stmp);
  lu_assert(set_has(s, st1 + 4));
  free(st1);

  set_destroy(s);
}

void t_set_copy()
{
  int n = 10;

  Set* s = set_new(int, 8, getHash_int);
  for (int i = 0; i < n; i++)
    set_add(s, i + 1);
  Set* sc = set_copy(s);

  lu_assert_int_eq(s->size, sc->size);
  int *a, *b;
  for (int i = 0; i < n; i++) {
    int item = i + 1;
    lu_assert(set_has(sc, &item));
  }
  set_destroy(s);
  set_destroy(sc);
}

void t_set_tovect()
{
  int n = 10;

  Set* s = set_new(int, 8, getHash_int);
  for (int i = 0; i < n; i++)
    set_add(s, i);

  Vector* v = set_to_vector(s);
  lu_assert(vector_size(v) == set_size(s));
  vector_destroy(v);
  set_destroy(s);
}
