#include <stdlib.h>
#include "cgds/HashTable.h"
#include "helpers.h"
#include "lut.h"

void t_hashtable_clear()
{
  HashTable* h = hashtable_new(int, 16);
  lu_assert(hashtable_empty(h));

  hashtable_set(h, "key1", 0);
  hashtable_set(h, "key2", 1);
  hashtable_set(h, "key3", 2);

  hashtable_destroy(h);
  h = hashtable_new(int, 8);

  hashtable_set(h, "key1", 1);
  hashtable_set(h, "key2", 2);
  hashtable_set(h, "key3", 3);

  hashtable_clear(h);
  lu_assert(hashtable_empty(h));

  hashtable_destroy(h);
}

void t_hashtable_size()
{
  HashTable* h = hashtable_new(int, 16);
  lu_assert(hashtable_empty(h));

  hashtable_set(h, "key1", 0);
  hashtable_set(h, "key2", 1);
  hashtable_set(h, "key3", 2);
  lu_assert_int_eq(hashtable_size(h), 3);

  hashtable_set(h, "key4", 3);
  hashtable_set(h, "key5", 4);
  lu_assert_int_eq(hashtable_size(h), 5);

  hashtable_set(h, "key6", 5);
  hashtable_set(h, "key7", 6);
  hashtable_set(h, "key8", 7);
  lu_assert_int_eq(hashtable_size(h), 8);

  hashtable_destroy(h);
}

void t_hashtable_set_remove_basic()
{
  int n = 10;

  HashTable* h = hashtable_new(double, 4);
  char key[] = "key_";
  for (int i = 0; i < n; i++)
  {
    key[3] = (char)(48 + i);
    hashtable_set(h, key, (double)i);
  }
  lu_assert_int_eq(hashtable_size(h), n);

  // Check values
  double ckValue = 0.0;
  for (int i = 0; i < n; i++)
  {
    double* d;
    key[3] = (char)(48 + i);
    hashtable_get(h, key, d);
    lu_assert_dbl_eq(*d, ckValue);
    ckValue += 1.0;
  }

  //Remove keys / values
  for (int i = 0; i < n; i++)
  {
    key[3] = (char)(48 + i);
    hashtable_delete(h, key);
  }
  lu_assert_int_eq(hashtable_size(h), 0);

  hashtable_destroy(h);
}

void t_hashtable_getnull_modify()
{
  int n = 10;

  HashTable* h = hashtable_new(StructTest1, 4);
  StructTest1* st1 = (StructTest1*) malloc(n * sizeof(StructTest1));
  char key[] = "key_";
  for (int i = 0; i < n; i++)
  {
    key[3] = (char)(48 + i);
    st1[i].a = random() % 42;
    st1[i].b = (double) random() / RAND_MAX;
    hashtable_set(h, key, *(st1 + i));
  }
  for (int i = 0; i < n; i++)
  {
    //another way to access elements
    key[3] = (char)(48 + i);
    StructTest1* st1Cell;
    hashtable_get(h, key, st1Cell);
    lu_assert_int_eq(st1Cell->a, st1[i].a);
    lu_assert_dbl_eq(st1Cell->b, st1[i].b);
  }
  safe_free(st1);

  // Get null:
  StructTest1* stmp;
  hashtable_get(h, "key12", stmp);
  lu_assert(stmp == NULL);
  hashtable_get(h, "key32", stmp);
  lu_assert(stmp == NULL);
  // Modify:
  StructTest1 stMod;
  stMod.a = -17;
  stMod.b = 3.0;
  hashtable_set(h, "key1", stMod);
  hashtable_get(h, "key1", stmp);
  lu_assert_int_eq(stmp->a, stMod.a);
  lu_assert_dbl_eq(stmp->b, stMod.b);
  stMod.a = -7;
  stMod.b = 3.5;
  hashtable_set(h, "key5", stMod);
  hashtable_get(h, "key5", stmp);
  lu_assert_int_eq(stmp->a, stMod.a);
  lu_assert_dbl_eq(stmp->b, stMod.b);

  hashtable_destroy(h);
}

void t_hashtable_copy()
{
  int n = 10;

  HashTable* h = hashtable_new(int, 8);
  char key[] = "key_";
  for (int i = 0; i < n; i++)
  {
    key[3] = (char)(48 + i);
    hashtable_set(h, key, random() % 42);
  }
  HashTable* hc = hashtable_copy(h);

  lu_assert_int_eq(h->size, hc->size);
  int *a, *b;
  for (int i = 0; i < n; i++)
  {
    key[3] = (char)(48 + i);
    hashtable_get(h, key, a);
    hashtable_get(hc, key, b);
    lu_assert_int_eq(*a, *b);
  }
  hashtable_destroy(h);
  hashtable_destroy(hc);
}
