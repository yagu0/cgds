#include <stdlib.h>
#include "cgds/List.h"
#include "test/helpers.h"
#include "test/lut.h"

void t_list_clear()
{
	List* L = list_new(int);

	list_insert_front(L, 0);
	list_insert_back(L, 0);
	list_insert_after(L, L->head, 0);

	list_clear(L);
	lu_assert(list_empty(L));

	list_destroy(L);
}

void t_list_size()
{
	List* L = list_new(double);

	list_insert_front(L, 0.0);
	list_insert_back(L, 0.0);
	list_insert_front(L, 0.0);
	lu_assert_int_eq(list_size(L), 3);

	list_insert_back(L, 0.0);
	list_insert_before(L, L->tail, 0.0);
	lu_assert_int_eq(list_size(L), 5);

	list_insert_after(L, L->head->next, 0.0);
	list_insert_before(L, L->tail->prev, 0.0);
	list_insert_after(L, L->head, 0.0);
	lu_assert_int_eq(list_size(L), 8);

	list_set(L, L->head->next, 42);
	list_remove_front(L);
	int a;
	list_get(L->head, a);
	lu_assert_int_eq(a, 42);
	list_remove(L, L->head->next);
	lu_assert_int_eq(list_size(L), 6);
	list_set(L, L->tail, 32);
	list_remove(L, L->tail->prev);
	list_get(L->tail, a);
	lu_assert_int_eq(a, 32);
	lu_assert_int_eq(list_size(L), 5);

	list_destroy(L);
}

void t_list_push_pop_basic()
{
	int n = 10;

	List* L = list_new(double);
	for (int i = 0; i < n; i++) list_insert_back(L, (double) i);
	// iterate and check values
	ListIterator* li = list_get_iterator(L);
	double ckValue = 0.0;
	while (listI_has_data(li))
	{
		double d;
		listI_get(li, d);
		lu_assert_dbl_eq(d, ckValue);
		ckValue += 1.0;
		listI_move_next(li);
	}

	// same, from end to beginning
	ckValue = n - 1;
	listI_reset_tail(li);
	while (listI_has_data(li))
	{
		double d;
		listI_get(li, d);
		lu_assert_dbl_eq(d, ckValue);
		ckValue -= 1.0;
		listI_move_prev(li);
	}
	list_destroy(L);
	listI_destroy(li);
}

void t_list_push_pop_evolved()
{
	int n = 10;

	List* L = list_new(StructTest1);
	StructTest1* st1 = (StructTest1*) malloc(n * sizeof (StructTest1));
	for (int i = 0; i < n; i++)
	{
		st1[i].a = rand() % 42;
		st1[i].b = (double) rand() / RAND_MAX;
		list_insert_back(L, *(st1 + i));
	}
	ListCell* lc = L->head;
	for (int i = 0; i < n; i++)
	{
		//another way to access elements
		StructTest1 st1Cell;
		list_get(lc, st1Cell);
		lu_assert_int_eq(st1Cell.a, st1[i].a);
		lu_assert_dbl_eq(st1Cell.b, st1[i].b);
		lc = lc->next;
	}
	safe_free(st1);
	list_destroy(L);

	L = list_new(StructTest2*);
	StructTest2* st2 = (StructTest2*) malloc(n * sizeof (StructTest2));
	for (int i = 0; i < n; i++)
	{
		st2[i].a = (float) rand() / RAND_MAX;
		st2[i].b = (StructTest1*) malloc(sizeof (StructTest1));
		st2[i].b->a = rand() % 42;
		st2[i].b->b = (double) rand() / RAND_MAX;
		list_insert_back(L, st2 + i);
	}
	lc = L->head;
	for (int i = 0; i < n; i++)
	{
		StructTest2* st2Cell;
		list_get(lc, st2Cell);
		lu_assert_dbl_eq(st2Cell->a, st2[i].a);
		lu_assert_int_eq(st2Cell->b->a, st2[i].b->a);
		lu_assert_dbl_eq(st2Cell->b->b, st2[i].b->b);
		safe_free(st2Cell->b);
		lc = lc->next;
	}
	safe_free(st2);
	list_destroy(L);
}

void t_list_copy()
{
	int n = 10;

	List* L = list_new(int);
	for (int i = 0; i < n; i++)
		list_insert_front(L, rand() % 42);
	List* Lc = list_copy(L);

	lu_assert_int_eq(L->size, Lc->size);
	ListCell* lCell = L->head;
	ListCell* lcCell = Lc->head;
	int a, b;
	for (int i = 0; i < n; i++)
	{
		list_get(lCell, a);
		list_get(lcCell, b);
		lu_assert_int_eq(a, b);
		lCell = lCell->next;
		lcCell = lcCell->next;
	}
	list_destroy(L);
	list_destroy(Lc);
}
