#include <stdlib.h>
#include "cgds/BufferTop.h"
#include "test/helpers.h"
#include "test/lut.h"

void t_buffertop_clear()
{
	BufferTop* bt = buffertop_new(int, 10, MIN_T, 2);

	// NOTE: items with same values are supported; 
	// since it is unused in this test, we arbitrarily choose 0.0
	buffertop_tryadd(bt, 0, 0.0);
	buffertop_tryadd(bt, 0, 0.0);
	buffertop_tryadd(bt, 0, 0.0);

	buffertop_clear(bt);
	lu_assert(buffertop_empty(bt));

	buffertop_destroy(bt);
}

void t_buffertop_size()
{
	BufferTop* bt = buffertop_new(double, 10, MAX_T, 3);

	buffertop_tryadd(bt, 0.0, 0.0);
	buffertop_tryadd(bt, 0.0, 0.0);
	buffertop_tryadd(bt, 0.0, 0.0);
	lu_assert_int_eq(buffertop_size(bt), 3);

	buffertop_tryadd(bt, 0.0, 0.0);
	buffertop_tryadd(bt, 0.0, 0.0);
	lu_assert_int_eq(buffertop_size(bt), 5);

	buffertop_tryadd(bt, 0.0, 0.0);
	buffertop_tryadd(bt, 0.0, 0.0);
	buffertop_tryadd(bt, 0.0, 0.0);
	lu_assert_int_eq(buffertop_size(bt), 8);
	buffertop_destroy(bt);

	// Now try to add beyond capacity, with reorganizing
	bt = buffertop_new(double, 3, MAX_T, 2);
	buffertop_tryadd(bt, 0.0, 0.0);
	buffertop_tryadd(bt, 0.0, 1.0);
	buffertop_tryadd(bt, 0.0, 2.0);
	buffertop_tryadd(bt, 0.0, 3.0);
	buffertop_tryadd(bt, 0.0, 4.0);
	buffertop_tryadd(bt, 0.0, 5.0);
	buffertop_tryadd(bt, 0.0, 6.0);
	buffertop_tryadd(bt, 0.0, 7.0);
	lu_assert_int_eq(buffertop_size(bt), 3);

	buffertop_clear(bt);
	lu_assert(buffertop_empty(bt));
	buffertop_destroy(bt);
}

void t_buffertop_push_pop_basic()
{
	BufferTop* bt = buffertop_new(int, 5, MIN_T, 3);

	buffertop_tryadd(bt, 1, 2.0);
	buffertop_tryadd(bt, 2, 6.0);
	buffertop_tryadd(bt, 3, 4.0);
	buffertop_tryadd(bt, 4, 8.0);
	buffertop_tryadd(bt, 5, 3.0);
	buffertop_tryadd(bt, 6, 1.0);
	buffertop_tryadd(bt, 7, 5.0);
	buffertop_tryadd(bt, 8, 7.0);

	int a;
	buffertop_first(bt, a);
	lu_assert_int_eq(a, 7); //7 -> 5.0
	buffertop_pop(bt);
	buffertop_first(bt, a);
	lu_assert_int_eq(a, 3); //3 -> 4.0
	buffertop_pop(bt);
	buffertop_first(bt, a);
	lu_assert_int_eq(a, 5); //5 -> 3.0
	buffertop_pop(bt);
	buffertop_first(bt, a);
	lu_assert_int_eq(a, 1); //1 -> 2.0
	buffertop_pop(bt);
	buffertop_first(bt, a);
	lu_assert_int_eq(a, 6); //6 has "highest" priority (1.0, MIN_T)
	buffertop_pop(bt);

	lu_assert(buffertop_empty(bt));
	buffertop_destroy(bt);
}

void t_buffertop_push_pop_evolved()
{
	int n = 10;

	BufferTop* bt = buffertop_new(StructTest1, 5, MAX_T, 2);
	StructTest1* st1 = (StructTest1*) safe_malloc(n * sizeof (StructTest1));
	for (int i = n - 1; i >= 0; i--)
	{
		st1[i].a = i;
		st1[i].b = (double) rand() / RAND_MAX;
		buffertop_tryadd(bt, *(st1 + i), i); //item i has value i
	}
	for (int i = n - buffertop_size(bt); i < n; i++)
	{
		StructTest1 st1Cell;
		buffertop_first(bt, st1Cell);
		lu_assert_int_eq(st1Cell.a, i);
		buffertop_pop(bt);
	}
	safe_free(st1);
	buffertop_destroy(bt);

	bt = buffertop_new(StructTest2*, 15, MAX_T, 4);
	StructTest2* st2 = (StructTest2*) safe_malloc(n * sizeof (StructTest2));
	for (int i = n - 1; i >= 0; i--)
	{
		st2[i].a = (float) rand() / RAND_MAX;
		st2[i].b = (StructTest1*) safe_malloc(sizeof (StructTest1));
		st2[i].b->a = i;
		st2[i].b->b = (double) rand() / RAND_MAX;
		// item i has value i+1 if i is even, i-1 if i is odd
		// that is to say, values are 1 0 3 2 5 4 ...
		buffertop_tryadd(bt, st2 + i, i % 2 == 0 ? i + 1 : i - 1);
	}
	for (int i = 0; i < n; i++)
	{
		StructTest2* st2Cell;
		buffertop_first(bt, st2Cell);
		// NOTE: i |--> i%2==0 ? i+1 : i-1 is an involution
		lu_assert_int_eq(st2Cell->b->a, i % 2 == 0 ? i + 1 : i - 1);
		buffertop_pop(bt);
		safe_free(st2Cell->b);
	}
	safe_free(st2);
	buffertop_destroy(bt);
}

void t_buffertop_copy()
{
	int n = 10;

	BufferTop* bt = buffertop_new(int, n, MIN_T, 3);
	for (int i = 0; i < n; i++)
		buffertop_tryadd(bt, rand() % 42, (double) rand() / RAND_MAX);
	BufferTop* btc = buffertop_copy(bt);

	lu_assert_int_eq(buffertop_size(bt), buffertop_size(btc));
	int a, b;
	for (int i = 0; i < n; i++)
	{
		buffertop_first(bt, a);
		buffertop_first(btc, b);
		lu_assert_int_eq(a, b);
		buffertop_pop(bt);
		buffertop_pop(btc);
	}
	buffertop_destroy(bt);
	buffertop_destroy(btc);
}
