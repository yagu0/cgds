#include <stdlib.h>

int main(int argc, char** argv)
{
	//file ./t.Stack.c :
	t_stack_clear();
	t_stack_size();
	t_stack_push_pop_basic();
	t_stack_push_pop_evolved();
	t_stack_copy();

	//file ./t.Heap.c :
	t_heap_clear();
	t_heap_size();
	t_heap_push_pop_basic();
	t_heap_push_pop_evolved();
	t_heap_copy();

	//file ./t.Tree.c :
	t_tree_clear();
	t_tree_size();
	t_tree_add_remove();
	t_tree_iterate();
	t_tree_copy();

	//file ./t.PriorityQueue.c :
	t_priorityqueue_clear();
	t_priorityqueue_size();
	t_priorityqueue_push_pop_basic();
	t_priorityqueue_push_pop_evolved();
	t_priorityqueue_copy();

	//file ./t.List.c :
	t_list_clear();
	t_list_size();
	t_list_push_pop_basic();
	t_list_push_pop_evolved();
	t_list_copy();

	//file ./t.Vector.c :
	t_vector_clear();
	t_vector_size();
	t_vector_push_pop_basic();
	t_vector_push_pop_evolved();
	t_vector_copy();

	//file ./t.Queue.c :
	t_queue_clear();
	t_queue_size();
	t_queue_push_pop_basic();
	t_queue_push_pop_evolved();
	t_queue_copy();

	//file ./t.BufferTop.c :
	t_buffertop_clear();
	t_buffertop_size();
	t_buffertop_push_pop_basic();
	t_buffertop_push_pop_evolved();
	t_buffertop_copy();

	return 0;
}
