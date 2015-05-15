#include <stdlib.h>
#include "cgds/Queue.h"
#include "test/helpers.h"
#include "test/lut.h"

void t_queue_clear() //FTEST
{
	Queue* q = queue_new(int);

	queue_push(q, 0);
	queue_push(q, 0);
	queue_push(q, 0);

	queue_clear(q);
	lu_assert(queue_empty(q));

	queue_destroy(q);
}

void t_queue_size() //FTEST
{
	Queue* q = queue_new(int);

	queue_push(q, 0);
	queue_push(q, 0);
	queue_push(q, 0);
	lu_assert_int_eq(queue_size(q), 3);

	queue_push(q, 0);
	queue_push(q, 0);
	lu_assert_int_eq(queue_size(q), 5);

	queue_push(q, 0);
	queue_push(q, 0);
	queue_push(q, 0);
	lu_assert_int_eq(queue_size(q), 8);

	queue_destroy(q);
}

void t_queue_push_pop_basic() //FTEST
{
	int n = 10;

	Queue* q = queue_new(double);
	for (int i = 0; i < n; i++) 
		queue_push(q, (double) i);
	// iterate and check values
	double ckValue = 0.0;
	while (!queue_empty(q))
	{
		double d;
		queue_peek(q, d);
		lu_assert_dbl_eq(d, ckValue);
		ckValue += 1.0;
		queue_pop(q);
	}

	lu_assert(queue_empty(q));
	queue_destroy(q);
}

void t_queue_push_pop_evolved() //FTEST
{
	int n = 10;

	Queue* q = queue_new(StructTest1);
	StructTest1* st1 = (StructTest1*) safe_malloc(n * sizeof (StructTest1));
	for (int i = 0; i < n; i++)
	{
		st1[i].a = rand() % 42;
		st1[i].b = (double) rand() / RAND_MAX;
		queue_push(q, *(st1 + i));
	}
	for (int i = 0; i < n; i++)
	{
		StructTest1 st1Cell;
		queue_peek(q, st1Cell);
		lu_assert_int_eq(st1Cell.a, st1[i].a);
		lu_assert_dbl_eq(st1Cell.b, st1[i].b);
		queue_pop(q);
	}
	safe_free(st1);
	queue_destroy(q);

	q = queue_new(StructTest2*);
	StructTest2* st2 = (StructTest2*) safe_malloc(n * sizeof (StructTest2));
	for (int i = 0; i < n; i++)
	{
		st2[i].a = (float) rand() / RAND_MAX;
		st2[i].b = (StructTest1*) safe_malloc(sizeof (StructTest1));
		st2[i].b->a = rand() % 42;
		st2[i].b->b = (double) rand() / RAND_MAX;
		queue_push(q, st2 + i);
	}
	for (int i = 0; i < n; i++)
	{
		StructTest2* st2Cell;
		queue_peek(q, st2Cell);
		lu_assert_dbl_eq(st2Cell->a, st2[i].a);
		lu_assert_int_eq(st2Cell->b->a, st2[i].b->a);
		lu_assert_dbl_eq(st2Cell->b->b, st2[i].b->b);
		queue_pop(q);
		safe_free(st2Cell->b);
	}
	safe_free(st2);
	queue_destroy(q);
}

void t_queue_copy() //FTEST
{
	int n = 10;

	Queue* q = queue_new(int);
	for (int i = 0; i < n; i++)
		queue_push(q, rand() % 42);
	Queue* qc = queue_copy(q);

	lu_assert_int_eq(queue_size(q), queue_size(qc));
	int a, b;
	for (int i = 0; i < n; i++)
	{
		queue_peek(q, a);
		queue_peek(qc, b);
		lu_assert_int_eq(a, b);
		queue_pop(q);
		queue_pop(qc);
	}
	queue_destroy(q);
	queue_destroy(qc);
}
