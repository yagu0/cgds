#include <stdlib.h>
#include "cgds/Tree.h"
#include "test/helpers.h"
#include "test/lut.h"

void t_tree_clear() //FTEST
{
	Tree* t = tree_new(int);

	tree_set_root(t, 0);
	tree_add_child(t, t->root, 1);
	tree_add_child(t, t->root, 2);
	tree_add_child(t, t->root, 3);
	tree_add_child(t, t->root->firstChild, 1);
	tree_add_child(t, t->root->firstChild, 2);
	tree_add_child(t, t->root->firstChild->next, 1);
	tree_add_child(t, t->root->firstChild->next, 2);
	tree_add_child(t, t->root->firstChild->next, 3);
	tree_add_child(t, t->root->firstChild->next, 4);

	tree_clear(t);
	lu_assert(tree_empty(t));

	tree_destroy(t);
}

void t_tree_size() //FTEST
{
	Tree* t = tree_new(int);

	tree_set_root(t, 0);
	tree_add_child(t, t->root, 1);
	tree_add_child(t, t->root, 2);
	tree_add_child(t, t->root, 3);
	lu_assert_int_eq(tree_size(t), 4);

	tree_add_child(t, t->root->firstChild, 1);
	tree_add_child(t, t->root->firstChild, 2);
	lu_assert_int_eq(tree_size(t), 6);

	tree_add_child(t, t->root->firstChild->next, 1);
	tree_add_child(t, t->root->firstChild->next, 2);
	tree_add_child(t, t->root->firstChild->next, 3);
	tree_add_child(t, t->root->firstChild->next, 4);
	lu_assert_int_eq(tree_size(t), 10);

	tree_destroy(t);
}

void t_tree_add_remove() //FTEST
{
	Tree* t = tree_new(int);

	tree_set_root(t, 0);
	tree_add_child(t, t->root, 1);
	tree_add_child(t, t->root, 2);
	tree_add_child(t, t->root, 3);
	tree_add_child(t, t->root->firstChild, 1);
	tree_add_child(t, t->root->firstChild, 2);
	tree_add_child(t, t->root->firstChild->next, 1);
	tree_add_child(t, t->root->firstChild->next, 2);
	tree_add_child(t, t->root->firstChild->next, 3);
	tree_add_child(t, t->root->firstChild->next, 4);
	tree_add_child(t, t->root->lastChild, 1);
	tree_add_child(t, t->root->lastChild, 2);
	tree_add_child(t, t->root->lastChild, 3);
	lu_assert_int_eq(tree_size(t), 13);

	tree_remove(t, t->root->lastChild);
	lu_assert_int_eq(tree_size(t), 9);
	tree_rm_childs(t, t->root->firstChild);
	lu_assert_int_eq(tree_size(t), 7);

	tree_destroy(t);
}

void t_tree_iterate() //FTEST
{
	Tree* t = tree_new(int);

	tree_set_root(t, 0);
	tree_add_child(t, t->root, 1);
	tree_add_child(t, t->root, 2);
	tree_add_child(t, t->root, 3);
	tree_add_child(t, t->root->firstChild, 4);
	tree_add_child(t, t->root->firstChild, 5);
	tree_add_child(t, t->root->firstChild->next, 6);
	tree_add_child(t, t->root->firstChild->next, 7);
	tree_add_child(t, t->root->firstChild->next, 8);
	tree_add_child(t, t->root->firstChild->next, 9);

	TreeIterator* ti = tree_get_iterator(t, IN_BREADTH);
	int a;
	for (int i = 0; i < 10; i++)
	{
		lu_assert(treeI_has_data(ti));
		treeI_get(ti, a);
		lu_assert_int_eq(a, i);
		treeI_move_next(ti);
	}

	treeI_destroy(ti);
	tree_destroy(t);
}

void t_tree_copy() //FTEST
{
	Tree* t = tree_new(int);

	tree_set_root(t, 0);
	tree_add_child(t, t->root, 1);
	tree_add_child(t, t->root, 2);
	tree_add_child(t, t->root, 3);
	tree_add_child(t, t->root->firstChild, 1);
	tree_add_child(t, t->root->firstChild, 2);
	tree_add_child(t, t->root->firstChild->next, 1);
	tree_add_child(t, t->root->firstChild->next, 2);
	tree_add_child(t, t->root->firstChild->next, 3);
	tree_add_child(t, t->root->firstChild->next, 4);

	Tree* tc = tree_copy(t);
	lu_assert_int_eq(t->size, tc->size);

	TreeIterator* ti = tree_get_iterator(t, IN_DEPTH);
	TreeIterator* tci = tree_get_iterator(tc, IN_DEPTH);
	int a, b;
	while (treeI_has_data(ti))
	{
		treeI_get(ti, a);
		treeI_get(tci, b);
		lu_assert_int_eq(a, b);
		treeI_move_next(ti);
		treeI_move_next(tci);
	}
	treeI_destroy(ti);
	treeI_destroy(tci);

	tree_destroy(t);
	tree_destroy(tc);
}
