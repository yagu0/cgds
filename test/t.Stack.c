#include <stdlib.h>
#include "cgds/Stack.h"
#include "test/helpers.h"
#include "test/lut.h"

void t_stack_clear() //FTEST
{
	Stack* s = stack_new(int);

	stack_push(s, 0);
	stack_push(s, 0);
	stack_push(s, 0);

	stack_clear(s);
	lu_assert(stack_empty(s));

	stack_destroy(s);
}

void t_stack_size() //FTEST
{
	Stack* s = stack_new(int);

	stack_push(s, 0);
	stack_push(s, 0);
	stack_push(s, 0);
	lu_assert_int_eq(stack_size(s), 3);

	stack_push(s, 0);
	stack_push(s, 0);
	lu_assert_int_eq(stack_size(s), 5);

	stack_push(s, 0);
	stack_push(s, 0);
	stack_push(s, 0);
	lu_assert_int_eq(stack_size(s), 8);

	stack_destroy(s);
}

void t_stack_push_pop_basic() //FTEST
{

	int n = 10;

	Stack* s = stack_new(double);
	for (int i = 0; i < n; i++) stack_push(s, (double) i);
	// iterate and check values
	double ckValue = n - 1;
	while (!stack_empty(s))
	{
		double d;
		stack_top(s, d);
		lu_assert_dbl_eq(d, ckValue);
		ckValue -= 1.0;
		stack_pop(s);
	}

	lu_assert(stack_empty(s));
	stack_destroy(s);
}

void t_stack_push_pop_evolved() //FTEST
{
	Stack* s = stack_new(StructTest1);

	int n = 10;
	StructTest1* st1 = (StructTest1*) malloc(n * sizeof (StructTest1));
	for (int i = n - 1; i >= 0; i--)
	{
		st1[i].a = rand() % 42;
		st1[i].b = (double) rand() / RAND_MAX;
		stack_push(s, *(st1 + i));
	}
	for (int i = 0; i < n; i++)
	{
		StructTest1 st1Cell;
		stack_top(s, st1Cell);
		lu_assert_int_eq(st1Cell.a, st1[i].a);
		lu_assert_dbl_eq(st1Cell.b, st1[i].b);
		stack_pop(s);
	}
	safe_free(st1);
	stack_destroy(s);

	s = stack_new(StructTest2*);
	StructTest2* st2 = (StructTest2*) malloc(n * sizeof (StructTest2));
	for (int i = n - 1; i >= 0; i--)
	{
		st2[i].a = (float) rand() / RAND_MAX;
		st2[i].b = (StructTest1*) malloc(sizeof (StructTest1));
		st2[i].b->a = rand() % 42;
		st2[i].b->b = (double) rand() / RAND_MAX;
		stack_push(s, st2 + i);
	}
	for (int i = 0; i < n; i++)
	{
		StructTest2* st2Cell;
		stack_top(s, st2Cell);
		lu_assert_dbl_eq(st2Cell->a, st2[i].a);
		lu_assert_int_eq(st2Cell->b->a, st2[i].b->a);
		lu_assert_dbl_eq(st2Cell->b->b, st2[i].b->b);
		stack_pop(s);
		safe_free(st2Cell->b);
	}
	safe_free(st2);
	stack_destroy(s);
}

void t_stack_copy() //FTEST
{
	int n = 10;

	Stack* s = stack_new(int);
	for (int i = 0; i < n; i++)
		stack_push(s, rand() % 42);
	Stack* sc = stack_copy(s);

	lu_assert_int_eq(s->size, sc->size);
	int a, b;
	for (int i = 0; i < n; i++)
	{
		stack_top(s, a);
		stack_top(sc, b);
		lu_assert_int_eq(a, b);
		stack_pop(s);
		stack_pop(sc);
	}
	stack_destroy(s);
	stack_destroy(sc);
}
