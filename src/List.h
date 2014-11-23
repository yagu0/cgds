/**
 * @file List.h
 */

#ifndef CGDS_LIST_H
#define CGDS_LIST_H

#include <stdlib.h>
#include <string.h>
#include "cgds/safe_alloc.h"
#include "cgds/types.h"

////////////////
// List logic //
////////////////

/**
 * @brief Cell of a double-linked list.
 */
typedef struct ListCell {
	void* data; ///< Generic data contained in this cell.
	struct ListCell* prev; ///< Pointer to previous cell in the list.
	struct ListCell* next; ///< Pointer to next cell in the list.
} ListCell;

/**
 * @brief Double-linked list data structure.
 */
typedef struct List {
	UInt size; ///< Count elements in the list.
	size_t dataSize; ///< Size of a list cell elements in bytes.
	ListCell* head; ///< Pointer to the first cell in the list.
	ListCell* tail; ///< Pointer to the last cell in the list.
} List;

/**
 * @brief Initialize an empty list.
 */
void _list_init(
	List* list, ///< "this" pointer.
	size_t dataSize ///< Size of a list cell elements in bytes.
);

/**
 * @brief Return an allocated and initialized list.
 * @param dataSize Size in bytes of a list element.
 */
List* _list_new(
	size_t dataSize ///< Size of a list cell elements in bytes.
);

/**
 * @brief Return an allocated and initialized list.
 * @param type Type of a list element (int, char*, ...).
 * 
 * Usage: List* list_new(<Type> type)
 */
#define list_new(type) \
	_list_new(sizeof(type))

/**
 * @brief Copy constructor (works well if items do not have allocated sub-pointers).
 */
List* list_copy(
	List* list ///< "this" pointer.
);

/**
 * @brief Check if the list is empty.
 */
Bool list_empty(
	List* list ///< "this" pointer.
);

/**
 * @brief return the size of current list.
 */
UInt list_size(
	List* list ///< "this" pointer.
);

/**
 * @brief Get data at the given list cell argument.
 */
void* _list_get(
	ListCell* listCell ///< Pointer to a cell inside "this" list.
);

/**
 * @brief Get data at the given list cell argument.
 * @param listCell Pointer to a cell inside "this" list.
 * @param data Data to be assigned.
 * 
 * Usage: void list_get(ListCell* listCell, void data)
 */
#define list_get(listCell, data) \
{ \
	void* pData = _list_get(listCell); \
	data = *((typeof(&data))pData); \
}

/**
 * @brief Set data at the given list cell argument.
 */
void _list_set(
	List* list, ///< "this" pointer.
	ListCell* listCell, ///< Pointer to a cell inside "this" list.
	void* data ///< Pointer to data to be set.
);

/**
 * @brief Set data at the given list cell argument.
 * @param list "this" pointer.
 * @param listCell Pointer to a cell inside "this" list.
 * @param data Data to be set.
 * 
 * Usage: void list_set(List* list, ListCell* listCell, void data);
 */
#define list_set(list, listCell, data) \
{ \
	typeof((data)) tmp = data; \
	_list_set(list, listCell, &tmp); \
}

/**
 * @brief Add data to the list when list is empty.
 */
void _list_insert_first_element(
	List* list, ///< "this" pointer.
	void* data ///< Pointer to data to be added
);

/**
 * @brief Add data before list cell argument.
 */
void _list_insert_before(
	List* list, ///< "this" pointer.
	ListCell* listCell, ///< Pointer to a cell inside "this" list.
	void* data ///< Pointer to data to be added.
);

/**
 * @brief Add data before list cell argument.
 * @param list "this" pointer.
 * @param listCell Pointer to a cell inside "this" list.
 * @param data Data to be inserted.
 * 
 * Usage: void list_insert_before(List* list, ListCell* listCell, void data)
 */
#define list_insert_before(list, listCell, data) \
{ \
	typeof((data)) tmp = data; \
	_list_insert_before(list, listCell, &tmp); \
}

/**
 * @brief Add data after list cell argument.
 */
void _list_insert_after(
	List* list, ///< "this" pointer.
	ListCell* listCell, ///< Pointer to a cell inside "this" list.
	void* data ///< Pointer to data to be inserted.
);

/**
 * @brief Add data after list cell argument.
 * @param list "this" pointer.
 * @param listCell Pointer to a cell inside "this" list.
 * @param data Data to be inserted.
 * 
 * Usage: void list_insert_after(List* list, ListCell* listCell, void data)
 */
#define list_insert_after(list, listCell, data) \
{ \
	typeof((data)) tmp = data; \
	_list_insert_after(list, listCell, &tmp); \
}

/**
 * @brief Add data at the beginning of the list.
 */
void _list_insert_front(
	List* list, ///< "this" pointer.
	void* data ///< Pointer to data to be inserted.
);

/**
 * @brief Add data at the beginning of the list.
 * @param list "this" pointer.
 * @param data Data to be inserted.
 * 
 * Usage: void list_insert_front(List* list, void data)
 */
#define list_insert_front(list, data) \
{ \
	typeof((data)) tmp = data; \
	_list_insert_front(list, &tmp); \
}

/**
 * @brief Add data at the end of the list.
 */
void _list_insert_back(
	List* list, ///< "this" pointer.
	void* data ///< Pointer to data to be inserted.
);

/**
 * @brief Add data at the end of the list.
 * @param list "this" pointer.
 * @param data Data to be inserted.
 * 
 * Usage: void list_insert_back(List* list, void data)
 */
#define list_insert_back(list, data) \
{ \
	typeof((data)) tmp = data; \
	_list_insert_back(list, &tmp); \
}

/** 
 * @brief Remove data at position given by 'listCell'.
 */
void list_remove(
	List* list, ///< "this" pointer.
	ListCell* listCell ///< Pointer to a cell inside "this" list.
);

/**
 * @brief Remove data at the beginning of the list.
 */
void list_remove_front(
	List* list ///< "this" pointer.
);

/**
 * @brief Remove data at the end of the list.
 */
void list_remove_back(
	List* list ///< "this" pointer.
);

/**
 * @brief Clear the entire list.
 */
void list_clear(
	List* list ///< "this" pointer.
);

/**
 * @brief Destroy the list: clear it, and free 'list' pointer.
 */
void list_destroy(
	List* list ///< "this" pointer.
);

////////////////////
// Iterator logic //
////////////////////

/**
 * @brief Iterator on a double-linked list.
 */
typedef struct ListIterator {
	List* list; ///< The list to be iterate.
	ListCell* current; ///< The current iterated list cell.
} ListIterator;

/**
 * @brief Obtain an iterator object, starting at list beginning.
 */
ListIterator* list_get_iterator(
	List* list ///< Pointer to the list to be iterated over.
);

/**
 * @brief (Re)set current position inside list to head.
 */
void listI_reset_head(
	ListIterator* listI ///< "this" pointer.
);

/**
 * @brief (Re)set current position inside list to tail.
 */
void listI_reset_tail(
	ListIterator* listI ///< "this" pointer.
);

/**
 * @brief Tell if there is some data at the current index.
 */
Bool listI_has_data(
	ListIterator* listI ///< "this" pointer.
);

/**
 * @brief Return data contained in the current list cell.
 * @param listI "this" pointer.
 * @param data Data to be assigned.
 * 
 * Usage: void listI_get(ListIterator* listI, void data)
 */
#define listI_get(listI, data) \
	list_get(listI->current, data)

/**
 * @brief Set data at the current iterator position.
 * @param listI "this" pointer
 * @param data Data to assign.
 * 
 * Usage: void listI_set(ListIterator* listI, void data); 
 */
#define listI_set(listI, data) \
	list_set(listI->list, listI->current, data)

/**
 * @brief Add data before current list cell.
 * @param listI "this" pointer
 * @param data Data to be inserted.
 * 
 * Usage: void listI_insert_before(ListIteratorI* listI, void data)
 */
#define listI_insert_before(listI, data) \
	list_insert_before(listI->list, listI->current, data)

/**
 * @brief Add data after current list cell.
 * @param listI "this" pointer
 * @param data Data to be inserted.
 * 
 * Usage: void listI_insert_after(ListIteratorI* listI, void data)
 */
#define listI_insert_after(listI, data) \
	list_insert_after(listI->list, listI->current, data)

/**
 * @brief Type to encode a direction (forward / backward).
 */
typedef enum {
	BACKWARD = -1, ///< Move toward head.
	FORWARD = 1 ///< Move toward tail.
} Direction;

/**
 * @brief Remove data at the current iterator position.
 */
void listI_remove(
	ListIterator* listI, ///< "this" pointer.
	Direction direction ///< Indicate the position of iterator after removal.
);

/**
 * @brief Move current iterator position forward (toward tail).
 */
void listI_move_next(
	ListIterator* listI ///< "this" pointer.
);

/**
 * @brief Move current iterator position backward (toward head).
 */
void listI_move_prev(
	ListIterator* listI ///< "this" pointer.
);

/**
 * @brief Free memory allocated for the iterator.
 */
void listI_destroy(
	ListIterator* listI ///< "this" pointer.
);

#endif
