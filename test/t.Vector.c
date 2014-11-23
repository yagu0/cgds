#include <stdlib.h>
#include "cgds/Vector.h"
#include "test/helpers.h"
#include "test/lut.h"

void t_vector_clear() //FTEST
{
	Vector* v = vector_new(int);
	lu_assert(vector_empty(v));

	vector_push(v, 0);
	vector_push(v, 0);
	vector_push(v, 0);

	vector_destroy(v);
	v = vector_new(int);

	vector_push(v, 0);
	vector_push(v, 0);
	vector_push(v, 0);

	vector_clear(v);
	lu_assert(vector_empty(v));

	vector_destroy(v);
}

void t_vector_size() //FTEST
{
	Vector* v = vector_new(int);
	lu_assert(vector_empty(v));

	vector_push(v, 0);
	vector_push(v, 0);
	vector_push(v, 0);
	lu_assert_int_eq(vector_size(v), 3);

	vector_push(v, 0);
	vector_push(v, 0);
	lu_assert_int_eq(vector_size(v), 5);

	vector_push(v, 0);
	vector_push(v, 0);
	vector_push(v, 0);
	lu_assert_int_eq(vector_size(v), 8);

	vector_destroy(v);
}

void t_vector_push_pop_basic() //FTEST
{
	int n = 10;

	Vector* v = vector_new(double);
	for (int i = 0; i < n; i++) vector_push(v, (double) i);
	// iterate and check values
	VectorIterator* vi = vector_get_iterator(v);
	double ckValue = 0.0;
	while (vectorI_has_data(vi))
	{
		double d;
		vectorI_get(vi, d);
		lu_assert_dbl_eq(d, ckValue);
		ckValue += 1.0;
		vectorI_move_next(vi);
	}

	// same, from end to beginning
	ckValue = n - 1;
	vectorI_reset_end(vi);
	while (vectorI_has_data(vi))
	{
		double d;
		vectorI_get(vi, d);
		lu_assert_dbl_eq(d, ckValue);
		ckValue -= 1.0;
		vectorI_move_prev(vi);
	}
	vector_destroy(v);
	vectorI_destroy(vi);
}

void t_vector_push_pop_evolved() //FTEST
{
	int n = 10;

	Vector* v = vector_new(StructTest1);
	StructTest1* st1 = (StructTest1*) malloc(n * sizeof (StructTest1));
	for (int i = 0; i < n; i++)
	{
		st1[i].a = rand() % 42;
		st1[i].b = (double) rand() / RAND_MAX;
		vector_push(v, *(st1 + i));
	}
	for (int i = 0; i < n; i++)
	{
		//another way to access elements
		StructTest1 st1Cell;
		vector_get(v, i, st1Cell);
		lu_assert_int_eq(st1Cell.a, st1[i].a);
		lu_assert_dbl_eq(st1Cell.b, st1[i].b);
	}
	safe_free(st1);
	vector_destroy(v);

	v = vector_new(StructTest2*);
	StructTest2* st2 = (StructTest2*) malloc(n * sizeof (StructTest2));
	for (int i = 0; i < n; i++)
	{
		st2[i].a = (float) rand() / RAND_MAX;
		st2[i].b = (StructTest1*) malloc(sizeof (StructTest1));
		st2[i].b->a = rand() % 42;
		st2[i].b->b = (double) rand() / RAND_MAX;
		vector_push(v, st2 + i);
	}
	for (int i = 0; i < n; i++)
	{
		StructTest2* st2Cell;
		vector_get(v, i, st2Cell);
		lu_assert_dbl_eq(st2Cell->a, st2[i].a);
		lu_assert_int_eq(st2Cell->b->a, st2[i].b->a);
		lu_assert_dbl_eq(st2Cell->b->b, st2[i].b->b);
		safe_free(st2Cell->b);
	}
	safe_free(st2);
	vector_destroy(v);
}

void t_vector_copy() //FTEST
{
	int n = 10;

	Vector* v = vector_new(int);
	for (int i = 0; i < n; i++)
		vector_push(v, rand() % 42);
	Vector* vc = vector_copy(v);

	lu_assert_int_eq(v->size, vc->size);
	int a, b;
	for (int i = 0; i < n; i++)
	{
		vector_get(v, i, a);
		vector_get(vc, i, b);
		lu_assert_int_eq(a, b);
	}
	vector_destroy(v);
	vector_destroy(vc);
}
