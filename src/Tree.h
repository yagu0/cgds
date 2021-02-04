/**
 * @file Tree.h
 */

#ifndef CGDS_TREE_H
#define CGDS_TREE_H

#include <stdlib.h>
#include <string.h>
#include "cgds/safe_alloc.h"
#include "cgds/types.h"

//***********
// Tree logic
//***********

/**
 * @brief Tree node, containing some generic data.
 */
typedef struct TreeNode {
  void* data; ///< Generic data contained in this node.
  struct TreeNode* parent; ///< Pointer to parent node (NULL if node is root).
  struct TreeNode* firstChild; ///< Pointer to the first child (if any).
  struct TreeNode* lastChild; ///< Pointer to the last child (if any).
  struct TreeNode* prev; ///< Pointer to the previous sibling (on the left).
  struct TreeNode* next; ///< Pointer to the next sibling (on the right).
} TreeNode;

/**
 * @brief Generic multi-ary tree.
 */
typedef struct Tree {
  TreeNode* root; ///< Root node of the tree.
  size_t dataSize; ///< Size of *data at a tree node, in bytes.
  UInt size; ///< Count nodes in the tree.
} Tree;

/**
 * @brief Initialize an empty tree.
 */
void _tree_init(
  Tree* tree, ///< "this" pointer.
  size_t dataSize ///< Size in bytes of a tree element.
);

/**
 * @brief Return an allocated and initialized tree.
 */
Tree* _tree_new(
  size_t dataSize ///< Size in bytes of a tree node element.
);

/**
 * @brief Return an allocated and initialized tree.
 * @param type Type at a tree node (int, char*, ...).
 *
 * Usage: Tree* tree_new(<Type> type)
 */
#define tree_new(type) \
  _tree_new(sizeof(type))

/**
 * @brief Copy constructor (shallow copy, ok for basic types).
 */
Tree* tree_copy(
  Tree* tree ///< "this" pointer.
);

/**
 * @brief Check if the tree is empty.
 */
bool tree_empty(
  Tree* tree ///< "this" pointer.
);

/**
 * @brief return current size of the tree (counting nodes).
 */
UInt tree_size(
  Tree* tree ///< "this" pointer.
);

/**
 * @brief Auxiliary function to get tree height.
 */
UInt _tree_height_rekursiv(
  TreeNode* treeNode ///< Pointer to a node in the "this" tree.
);

/**
 * @brief Return tree height (max depth from root to leaves).
 */
UInt tree_height(
  Tree* tree ///< "this" pointer.
);

/**
 * @brief Check if a sub-tree is a leaf (without children).
 */
bool tree_is_leaf(
  TreeNode* treeNode ///< Pointer to a node in the "this" tree.
);

/**
 * @brief Initialize tree root when the tree is empty.
 */
void _tree_set_root(
  Tree* tree, ///< "this" pointer.
  void* data ///< Pointer to data to be assigned.
);

/**
 * @brief Initialize tree root when the tree is empty.
 * @param tree "this" pointer.
 * @param data Data to be assigned.
 *
 * Usage: void tree_set_root(Tree* tree, void data)
 */
#define tree_set_root(tree, data) \
{ \
  typeof(data) tmp = data; \
  _tree_set_root(tree, &tmp); \
}

/**
 * @brief Return data contained in a given tree node.
 */
void* _tree_get(
  TreeNode* treeNode ///< Pointer to a node in the "this" tree.
);

/**
 * @brief Retrieve data contained in a given tree node.
 * @param treeNode Pointer to a node in the "this" tree.
 * @param data Data to be assigned.
 *
 * Usage: void tree_get(TreeNode* treeNode, void data)
 */
#define tree_get(treeNode, data) \
{ \
  void* pData = _tree_get(treeNode); \
  data = *((typeof(&data))pData); \
}

/**
 * @brief Set (alter) data at tree node passed as argument.
 */
void _tree_set(
  Tree* tree, ///< "this" pointer.
  TreeNode* treeNode, ///< Pointer to a node in the "this" tree.
  void* data ///< Pointer to data to be assigned.
);

/**
 * @brief Set (alter) data at tree node passed as argument.
 * @param tree "this" pointer.
 * @param treeNode Pointer to a node in the "this" tree.
 * @param data Data to be assigned.
 *
 * Usage: void tree_set(Tree* tree, TreeNode* treeNode, void data)
 */
#define tree_set(tree, treeNode, data) \
{ \
  typeof(data) tmp = data; \
  _tree_set(tree, treeNode, &tmp); \
}

/**
 * @brief Add a child to current node children.
 */
void _tree_add_child(
  Tree* tree, ///< "this" pointer.
  TreeNode* treeNode, ///< Pointer to a node in the "this" tree.
  void* data ///< Pointer to data to be added.
);

/**
 * @brief Add a child to current node children.
 * @param tree "this" pointer.
 * @param treeNode Pointer to a node in the "this" tree.
 * @param data Data to be added.
 *
 * Usage: void tree_add_child(Tree* tree, TreeNode* treeNode, void data)
 */
#define tree_add_child(tree,treeNode,data) \
{ \
  typeof(data) tmp = data; \
  _tree_add_child(tree, treeNode, &tmp); \
}

/**
 * @brief Add a sibling to current node (after it on the right).
 */
void _tree_add_sibling(
  Tree* tree, ///< "this" pointer.
  TreeNode* treeNode, ///< Pointer to a node in the "this" tree.
  void* data ///< Pointer to data to be added.
);

/**
 * @brief Add a sibling to current node (after it on the right).
 * @param tree "this" pointer.
 * @param treeNode Pointer to a node in the "this" tree.
 * @param data Data to be added.
 *
 * Usage: void tree_add_sibling(Tree* tree, TreeNode* treeNode, void data)
 */
#define tree_add_sibling(tree, treeNode, data) \
{ \
  typeof(data) tmp = data; \
  _tree_add_sibling(tree, treeNode, &tmp); \
}

/**
 * @brief Auxiliary to remove a subtree.
 */
void _tree_remove_rekursiv(
  Tree* tree, ///< "this" pointer.
  TreeNode* treeNode ///< Pointer to a node in the "this" tree.
);

/**
 * @brief Remove the whole subtree rooted at 'treeNode'.
 */
void tree_remove(
  Tree* tree, ///< "this" pointer.
  TreeNode* treeNode ///< Pointer to a node in the "this" tree.
);

/**
 * @brief Remove children of the tree node given as argument.
 */
void tree_rm_childs(
  Tree* tree, ///< "this" pointer.
  TreeNode* treeNode ///< Pointer to a node in the "this" tree.
);

/**
 * @brief Clear the entire tree.
 */
void tree_clear(
  Tree* tree ///< "this" pointer.
);

/**
 * @brief Destroy the tree: clear it, and free 'tree' pointer.
 */
void tree_destroy(
  Tree* tree ///< "this" pointer.
);

//***************
// Iterator logic
//***************

/**
 * @brief Type of tree search: depth first or breadth first walk.
 */
typedef enum TreeIteratorMode {
  IN_DEPTH = 0, ///< Depth first.
  IN_BREADTH = 1 ///< Breadth first.
} TreeIteratorMode;

/**
 * @brief Iterator on a tree object.
 */
typedef struct TreeIterator {
  Tree* tree; ///< Pointer to the tree to iterate over.
  TreeNode* current; ///< Current iterator position.
  TreeIteratorMode mode; ///< Mode of iteration (in depth or in breadth).
} TreeIterator;

/**
 * @brief Obtain an iterator object, starting at tree root.
 */
TreeIterator* tree_get_iterator(
  Tree* tree, ///< Pointer to the tree to iterate over.
  TreeIteratorMode mode ///< Mode of iteration (in depth or in breadth).
);

/**
 * @brief (Re)set current position inside tree to root.
 */
void treeI_reset(
  TreeIterator* treeI ///< "this" pointer.
);

/**
 * @brief Tell if there is some data at the current index.
 */
bool treeI_has_data(
  TreeIterator* treeI ///< "this" pointer.
);

/**
 * @brief Return current tree node.
 */
TreeNode* treeI_get_raw(
  TreeIterator* treeI ///< "this" pointer.
);

/**
 * @brief Get data at current tree node.
 * @param treeI "this" pointer.
 * @param data Data to be assigned.
 *
 * Usage: void treeI_get(TreeIterator* treeI, void data)
 */
#define treeI_get(treeI, data) \
  tree_get(treeI->current, data)

/**
 * @brief Set (alter) data at current tree node.
 * @param treeI "this" pointer.
 * @param data Data to be assigned.
 *
 * Usage: void treeI_set(TreeIterator* treeI, void data)
 */
#define treeI_set(treeI, data) \
  tree_set(treeI->tree, treeI->current, data)

/**
 * @brief Move current iterator position forward (toward leaves).
 */
void treeI_move_next(
  TreeIterator* treeI ///< "this" pointer.
);

/**
 * @brief Free memory allocated for the iterator.
 */
void treeI_destroy(
  TreeIterator* treeI ///< "this" pointer.
);

#endif
