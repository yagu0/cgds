#include <stdlib.h>
#include "cgds/PriorityQueue.h"
#include "helpers.h"
#include "lut.h"

void t_priorityqueue_clear()
{
  PriorityQueue* pq = priorityqueue_new(int, MIN_T, 2);

  // NOTE: items with same priorities are supported;
  // since it is unused in this test, we arbitrarily choose 0.0
  priorityqueue_insert(pq, 0, 0.0);
  priorityqueue_insert(pq, 0, 0.0);
  priorityqueue_insert(pq, 0, 0.0);

  priorityqueue_clear(pq);
  lu_assert(priorityqueue_empty(pq));

  priorityqueue_destroy(pq);
}

void t_priorityqueue_size()
{
  PriorityQueue* pq = priorityqueue_new(double, MAX_T, 3);

  priorityqueue_insert(pq, 0.0, 0.0);
  priorityqueue_insert(pq, 0.0, 0.0);
  priorityqueue_insert(pq, 0.0, 0.0);
  lu_assert_int_eq(priorityqueue_size(pq), 3);

  priorityqueue_insert(pq, 0.0, 0.0);
  priorityqueue_insert(pq, 0.0, 0.0);
  lu_assert_int_eq(priorityqueue_size(pq), 5);

  priorityqueue_insert(pq, 0.0, 0.0);
  priorityqueue_insert(pq, 0.0, 0.0);
  priorityqueue_insert(pq, 0.0, 0.0);
  lu_assert_int_eq(priorityqueue_size(pq), 8);

  priorityqueue_destroy(pq);
}

void t_priorityqueue_push_pop_basic()
{
  PriorityQueue* pq = priorityqueue_new(int, MIN_T, 3);

  priorityqueue_insert(pq, 1, 1.0);
  priorityqueue_insert(pq, 2, 3.0);
  priorityqueue_insert(pq, 3, 2.0);
  priorityqueue_insert(pq, 4, 4.0);
  priorityqueue_insert(pq, 5, 0.0);

  int a;
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 5); //5 has "highest" priority (0.0, MIN_T)
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 1); //1 -> 1.0
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 3); //3 -> 2.0
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 2); //2 -> 3.0
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 4); //4 -> 4.0
  priorityqueue_pop(pq);
  lu_assert(priorityqueue_empty(pq));

  pq->heap->hType = MAX_T; //HACK... (why not?)
  priorityqueue_insert(pq, 1, 1.0);
  priorityqueue_insert(pq, 2, 3.0);
  priorityqueue_insert(pq, 3, 2.0);
  priorityqueue_insert(pq, 4, 4.0);
  priorityqueue_insert(pq, 5, 0.0);
  priorityqueue_insert(pq, 6, 0.0);
  priorityqueue_insert(pq, 7, 6.0);
  priorityqueue_remove(pq, 6);
  priorityqueue_remove(pq, 7);
  priorityqueue_set(pq, 3, 5.0);
  priorityqueue_set(pq, 5, 2.0);

  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 3); //3 has highest priority (5.0, MAX_T)
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 4); //4 -> 4.0
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 2); //2 -> 3.0
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 5); //5 -> 2.0
  priorityqueue_pop(pq);
  priorityqueue_peek(pq, a);
  lu_assert_int_eq(a, 1); // 1 -> 1.0
  priorityqueue_pop(pq);

  lu_assert(priorityqueue_empty(pq));
  priorityqueue_destroy(pq);
}

void t_priorityqueue_push_pop_evolved()
{
  int n = 10;

  PriorityQueue* pq = priorityqueue_new(StructTest1, MAX_T, 2);
  StructTest1* st1 = (StructTest1*) safe_malloc(n * sizeof (StructTest1));
  for (int i = n - 1; i >= 0; i--)
  {
    st1[i].a = i;
    st1[i].b = (double) rand() / RAND_MAX;
    priorityqueue_insert(pq, *(st1 + i), i); //item i has priority i
  }
  for (int i = n - 1; i >= 0; i--)
  {
    StructTest1 st1Cell;
    priorityqueue_peek(pq, st1Cell);
    lu_assert_int_eq(st1Cell.a, i);
    priorityqueue_pop(pq);
  }
  safe_free(st1);
  priorityqueue_destroy(pq);

  pq = priorityqueue_new(StructTest2*, MAX_T, 4);
  StructTest2* st2 = (StructTest2*) safe_malloc(n * sizeof (StructTest2));
  for (int i = n - 1; i >= 0; i--)
  {
    st2[i].a = (float) rand() / RAND_MAX;
    st2[i].b = (StructTest1*) safe_malloc(sizeof (StructTest1));
    st2[i].b->a = i;
    st2[i].b->b = (double) rand() / RAND_MAX;
    // item i has priority i+1 if i is even, i-1 if i is odd
    // that is to say, priorities are 1 0 3 2 5 4 ...
    priorityqueue_insert(pq, st2 + i, i % 2 == 0 ? i + 1 : i - 1);
  }
  for (int i = n - 1; i >= 0; i--)
  {
    StructTest2* st2Cell;
    priorityqueue_peek(pq, st2Cell);
    // NOTE: i |--> i%2==0 ? i+1 : i-1 is an involution
    lu_assert_int_eq(st2Cell->b->a, i % 2 == 0 ? i + 1 : i - 1);
    priorityqueue_pop(pq);
    safe_free(st2Cell->b);
  }
  safe_free(st2);
  priorityqueue_destroy(pq);
}

void t_priorityqueue_copy()
{
  int n = 10;

  PriorityQueue* pq = priorityqueue_new(int, MIN_T, 3);
  for (int i = 0; i < n; i++)
    priorityqueue_insert(pq, rand() % 42, (double) rand() / RAND_MAX);
  PriorityQueue* pqc = priorityqueue_copy(pq);

  lu_assert_int_eq(priorityqueue_size(pq), priorityqueue_size(pqc));
  int a, b;
  for (int i = 0; i < n; i++)
  {
    priorityqueue_peek(pq, a);
    priorityqueue_peek(pqc, b);
    lu_assert_int_eq(a, b);
    priorityqueue_pop(pq);
    priorityqueue_pop(pqc);
  }
  priorityqueue_destroy(pq);
  priorityqueue_destroy(pqc);
}
