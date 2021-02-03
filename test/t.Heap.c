#include <stdlib.h>
#include "cgds/Heap.h"
#include "helpers.h"
#include "lut.h"

void t_heap_clear()
{
	Heap* h = heap_new(int, MIN_T, 2);

	// NOTE: items with same priorities are supported;
	// since it is unused in this test, we arbitrarily choose 0.0
	heap_insert(h, 0, 0.0);
	heap_insert(h, 0, 0.0);
	heap_insert(h, 0, 0.0);

	heap_clear(h);
	lu_assert(heap_empty(h));

	heap_destroy(h);
}

void t_heap_size()
{
	Heap* h = heap_new(double, MAX_T, 3);

	heap_insert(h, 0.0, 0.0);
	heap_insert(h, 0.0, 0.0);
	heap_insert(h, 0.0, 0.0);
	lu_assert_int_eq(heap_size(h), 3);

	heap_insert(h, 0.0, 0.0);
	heap_insert(h, 0.0, 0.0);
	lu_assert_int_eq(heap_size(h), 5);

	heap_insert(h, 0.0, 0.0);
	heap_insert(h, 0.0, 0.0);
	heap_insert(h, 0.0, 0.0);
	lu_assert_int_eq(heap_size(h), 8);

	heap_destroy(h);
}

void t_heap_push_pop_basic()
{
	Heap* h = heap_new(int, MIN_T, 3);

	heap_insert(h, 1, 1.0);
	heap_insert(h, 2, 3.0);
	heap_insert(h, 3, 2.0);
	heap_insert(h, 4, 4.0);
	heap_insert(h, 5, 0.0);

	int a;
	heap_top(h, a);
	lu_assert_int_eq(a, 5); //5 has "highest" priority (0.0, MIN_T)
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 1); //1 -> 1.0
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 3); //3 -> 2.0
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 2); //2 -> 3.0
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 4); //4 -> 4.0
	heap_pop(h);
	lu_assert(heap_empty(h));

	h->hType = MAX_T; //HACK... (why not?)
	heap_insert(h, 1, 1.0);
	heap_insert(h, 2, 3.0);
	heap_insert(h, 3, 2.0);
	heap_insert(h, 4, 4.0);
	heap_insert(h, 5, 7.0);
	heap_insert(h, 6, 5.0);
	heap_insert(h, 7, 6.0);
	heap_remove(h, 4);
	heap_remove(h, 2);
	heap_modify(h, 3, 4.0);
	heap_modify(h, 7, 3.0);

	heap_top(h, a);
	lu_assert_int_eq(a, 5); //5 has highest priority (7.0, MAX_T)
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 6); //6 -> 5.0
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 3); //3 -> 4.0
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 7); //7 -> 3.0
	heap_pop(h);
	heap_top(h, a);
	lu_assert_int_eq(a, 1); //1 -> 1.0
	heap_pop(h);
	lu_assert(heap_empty(h));

	heap_destroy(h);
}

void t_heap_push_pop_evolved()
{
	int n = 10;

	Heap* h = heap_new(StructTest1, MAX_T, 2);
	StructTest1* st1 = (StructTest1*) safe_malloc(n * sizeof (StructTest1));
	for (int i = n - 1; i >= 0; i--)
	{
		st1[i].a = i;
		st1[i].b = (double) rand() / RAND_MAX;
		heap_insert(h, *(st1 + i), i); //item i has value i
	}
	for (int i = n - 1; i >= 0; i--)
	{
		StructTest1 st1Cell;
		heap_top(h, st1Cell);
		lu_assert_int_eq(st1Cell.a, i);
		heap_pop(h);
	}
	safe_free(st1);
	heap_destroy(h);

	h = heap_new(StructTest2*, MAX_T, 4);
	StructTest2* st2 = (StructTest2*) safe_malloc(n * sizeof (StructTest2));
	for (int i = n - 1; i >= 0; i--)
	{
		st2[i].a = (float) rand() / RAND_MAX;
		st2[i].b = (StructTest1*) safe_malloc(sizeof (StructTest1));
		st2[i].b->a = i;
		st2[i].b->b = (double) rand() / RAND_MAX;
		// item i has value i+1 if i is even, i-1 if i is odd
		// that is to say, values are 1 0 3 2 5 4 ...
		heap_insert(h, st2 + i, i % 2 == 0 ? i + 1 : i - 1);
	}
	for (int i = n - 1; i >= 0; i--)
	{
		StructTest2* st2Cell;
		heap_top(h, st2Cell);
		// NOTE: i |--> i%2==0 ? i+1 : i-1 is an involution
		lu_assert_int_eq(st2Cell->b->a, i % 2 == 0 ? i + 1 : i - 1);
		heap_pop(h);
		safe_free(st2Cell->b);
	}
	safe_free(st2);
	heap_destroy(h);
}

void t_heap_copy()
{
	int n = 10;

	Heap* h = heap_new(int, MIN_T, 2);
	for (int i = 0; i < n; i++)
		heap_insert(h, rand() % 42, (double) rand() / RAND_MAX);
	Heap* hc = heap_copy(h);

	lu_assert_int_eq(heap_size(h), heap_size(hc));
	int a, b;
	for (int i = 0; i < n; i++)
	{
		heap_top(h, a);
		heap_top(hc, b);
		lu_assert_int_eq(a, b);
		heap_pop(h);
		heap_pop(hc);
	}
	heap_destroy(h);
	heap_destroy(hc);
}
