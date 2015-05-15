#include <stdlib.h>

int main(int argc, char** argv)
{
	//file ./t.Vector.c :
	void t_vector_clear();
	void t_vector_size();
	void t_vector_push_pop_basic();
	void t_vector_push_pop_evolved();
	void t_vector_copy();

	//file ./t.BufferTop.c :
	void t_buffertop_clear();
	void t_buffertop_size();
	void t_buffertop_push_pop_basic();
	void t_buffertop_push_pop_evolved();
	void t_buffertop_copy();

	//file ./t.Tree.c :
	void t_tree_clear();
	void t_tree_size();
	void t_tree_add_remove();
	void t_tree_iterate();
	void t_tree_copy();

	//file ./t.Heap.c :
	void t_heap_clear();
	void t_heap_size();
	void t_heap_push_pop_basic();
	void t_heap_push_pop_evolved();
	void t_heap_copy();

	//file ./t.List.c :
	void t_list_clear();
	void t_list_size();
	void t_list_push_pop_basic();
	void t_list_push_pop_evolved();
	void t_list_copy();

	//file ./t.Queue.c :
	void t_queue_clear();
	void t_queue_size();
	void t_queue_push_pop_basic();
	void t_queue_push_pop_evolved();
	void t_queue_copy();

	//file ./t.Stack.c :
	void t_stack_clear();
	void t_stack_size();
	void t_stack_push_pop_basic();
	void t_stack_push_pop_evolved();
	void t_stack_copy();

	//file ./t.PriorityQueue.c :
	void t_priorityqueue_clear();
	void t_priorityqueue_size();
	void t_priorityqueue_push_pop_basic();
	void t_priorityqueue_push_pop_evolved();
	void t_priorityqueue_copy();

	return 0;
}
