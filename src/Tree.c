/**
 * @file Tree.c
 */

#include "cgds/Tree.h"

////////////////
// Tree logic //
////////////////

void _tree_init(Tree* tree, size_t dataSize)
{
  tree->root = NULL;
  tree->dataSize = dataSize;
  tree->size = 0;
}

Tree* _tree_new(size_t dataSize)
{
  Tree* tree = (Tree*) safe_malloc(sizeof (Tree));
  _tree_init(tree, dataSize);
  return tree;
}

Tree* tree_copy(Tree* tree)
{
  Tree* treeCopy = _tree_new(tree->dataSize);
  if (tree->root == NULL)
    return treeCopy;
  _tree_set_root(treeCopy, tree->root->data);

  // Now parallel run on both trees (manual breadth-first walk)
  TreeNode* treeNode = tree->root;
  TreeNode* treeNodeCopy = treeCopy->root;
  while (treeNode != NULL)
  {
    // process current children
    TreeNode* child = treeNode->firstChild;
    while (child != NULL)
    {
      _tree_add_child(treeCopy, treeNodeCopy, child->data);
      child = child->next;
    }

    // try to go to next sibling (NOTE: already added as child of parent)
    if (treeNode->next != NULL)
    {
      treeNode = treeNode->next;
      treeNodeCopy = treeNodeCopy->next;
      continue;
    }

    // try to go to next "cousin" on same level
    if (treeNode->parent != NULL && treeNode->parent->next != NULL)
    {
      TreeNode* treeNodeParent = treeNode->parent->next;
      TreeNode* treeNodeParentCopy = treeNodeCopy->parent->next;
      while (treeNodeParent != NULL && tree_is_leaf(treeNodeParent))
      {
        treeNodeParent = treeNodeParent->next;
        treeNodeParentCopy = treeNodeParentCopy->next;
      }
      if (treeNodeParent != NULL)
      {
        treeNode = treeNodeParent->firstChild;
        treeNodeCopy = treeNodeParentCopy->firstChild;
        continue;
      }
    }

    // try to go to next level, and move treeNodeCopy accordingly
    while (treeNode->prev != NULL)
    {
      treeNode = treeNode->prev;
      treeNodeCopy = treeNodeCopy->prev;
    }
    while (treeNode != NULL && tree_is_leaf(treeNode))
    {
      treeNode = treeNode->next;
      treeNodeCopy = treeNodeCopy->next;
    }
    if (treeNode != NULL)
    {
      treeNode = treeNode->firstChild;
      treeNodeCopy = treeNodeCopy->firstChild;
    }
  }
  return treeCopy;
}

bool tree_empty(Tree* tree)
{
  return (tree->root == NULL);
}

UInt tree_size(Tree* tree)
{
  return tree->size;
}

UInt _tree_height_rekursiv(TreeNode* treeNode)
{
  if (tree_is_leaf(treeNode))
    return 1;
  TreeNode* child = treeNode->firstChild;
  UInt maxHeightChild = 0;
  while (child != NULL)
  {
    UInt heightChild = _tree_height_rekursiv(child);
    if (heightChild > maxHeightChild)
      maxHeightChild = heightChild;
    child = child->next;
  }
  return 1 + maxHeightChild;
}

UInt tree_height(Tree* tree)
{
  if (tree_empty(tree))
    return 0;
  return _tree_height_rekursiv(tree->root);
}

bool tree_is_leaf(TreeNode* treeNode)
{
  return (treeNode->firstChild == NULL);
}

void _tree_set_root(Tree* tree, void* data)
{
  tree->root = (TreeNode*) safe_malloc(sizeof (TreeNode));
  tree->root->data = safe_malloc(tree->dataSize);
  memcpy(tree->root->data, data, tree->dataSize);
  tree->root->parent = NULL;
  tree->root->firstChild = NULL;
  tree->root->lastChild = NULL;
  tree->root->prev = NULL;
  tree->root->next = NULL;
  tree->size = 1;
}

void* _tree_get(TreeNode* treeNode)
{
  return treeNode->data;
}

void _tree_set(Tree* tree, TreeNode* treeNode, void* data)
{
  memcpy(treeNode->data, data, tree->dataSize);
}

TreeNode* _tree_add_child(Tree* tree, TreeNode* treeNode, void* data)
{
  TreeNode* newChildNode = (TreeNode*) safe_malloc(sizeof (TreeNode));
  newChildNode->data = safe_malloc(tree->dataSize);
  memcpy(newChildNode->data, data, tree->dataSize);
  newChildNode->next = NULL;
  if (treeNode->lastChild != NULL)
    treeNode->lastChild->next = newChildNode;
  newChildNode->prev = treeNode->lastChild;
  treeNode->lastChild = newChildNode;
  if (treeNode->firstChild == NULL)
    treeNode->firstChild = newChildNode;
  newChildNode->parent = treeNode;
  newChildNode->firstChild = NULL;
  newChildNode->lastChild = NULL;
  tree->size++;
  return newChildNode;
}

TreeNode* _tree_add_sibling(Tree* tree, TreeNode* treeNode, void* data)
{
  TreeNode* newSiblingNode = (TreeNode*) safe_malloc(sizeof (TreeNode));
  newSiblingNode->data = safe_malloc(tree->dataSize);
  memcpy(newSiblingNode->data, data, tree->dataSize);
  newSiblingNode->next = treeNode->next;
  if (treeNode->next != NULL)
    treeNode->next->prev = newSiblingNode;
  newSiblingNode->prev = treeNode;
  treeNode->next = newSiblingNode;
  newSiblingNode->parent = treeNode->parent;
  newSiblingNode->firstChild = NULL;
  newSiblingNode->lastChild = NULL;
  tree->size++;
  return newSiblingNode;
}

void _tree_remove_rekursiv(Tree* tree, TreeNode* treeNode)
{
  TreeNode* child = treeNode->firstChild;
  while (child != NULL)
  {
    TreeNode* nextChild = child->next;
    _tree_remove_rekursiv(tree, child);
    child = nextChild;
  }
  safe_free(treeNode->data);
  safe_free(treeNode);
  tree->size--;
}

void tree_remove(Tree* tree, TreeNode* treeNode)
{
  if (treeNode->parent != NULL)
  {
    if (treeNode->prev == NULL)
      treeNode->parent->firstChild = treeNode->next;
    if (treeNode->next == NULL)
      treeNode->parent->lastChild = treeNode->prev;
  }
  if (treeNode->next != NULL)
    treeNode->next->prev = treeNode->prev;
  if (treeNode->prev != NULL)
    treeNode->prev->next = treeNode->next;
  _tree_remove_rekursiv(tree, treeNode);
}

void tree_rm_childs(Tree* tree, TreeNode* treeNode)
{
  TreeNode* child = treeNode->firstChild;
  while (child != NULL)
  {
    TreeNode* nextChild = child->next;
    _tree_remove_rekursiv(tree, child);
    child = nextChild;
  }
  treeNode->firstChild = NULL;
  treeNode->lastChild = NULL;
}

void tree_clear(Tree* tree)
{
  if (tree->root != NULL)
    _tree_remove_rekursiv(tree, tree->root);
  _tree_init(tree, tree->dataSize);
}

void tree_destroy(Tree* tree)
{
  if (tree->root != NULL)
    tree_clear(tree);
  safe_free(tree);
}

////////////////////
// Iterator logic //
////////////////////

TreeIterator* tree_get_iterator(Tree* tree, TreeIteratorMode mode)
{
  TreeIterator* treeI = (TreeIterator*) safe_malloc(sizeof (TreeIterator));
  treeI->tree = tree;
  treeI->mode = mode;
  treeI_reset(treeI);
  return treeI;
}

void treeI_reset(TreeIterator* treeI)
{
  treeI->current = treeI->tree->root;
}

bool treeI_has_data(TreeIterator* treeI)
{
  return (treeI->current != NULL);
}

TreeNode* treeI_get_raw(TreeIterator* treeI)
{
  return treeI->current;
}

void treeI_move_next(TreeIterator* treeI)
{
  TreeIteratorMode mode = treeI->mode;
  switch (mode)
  {
  case IN_DEPTH:
    if (!tree_is_leaf(treeI->current))
    {
      // easy case: just descend deeper in the tree
      treeI->current = treeI->current->firstChild;
      return;
    }
    // leaf: while no next sibling is available, move up
    while (treeI->current != NULL && treeI->current->next == NULL)
      treeI->current = treeI->current->parent;
    if (treeI->current != NULL)
      // run goes on from next sibling
      treeI->current = treeI->current->next;
    break;
  case IN_BREADTH:
    if (treeI->current->next != NULL)
    {
      // easy case : just move to the next sibling
      treeI->current = treeI->current->next;
      return;
    }
    // try to go to next "cousin" on same level
    if (treeI->current->parent != NULL && treeI->current->parent->next != NULL)
    {
      TreeNode* treeNodeParent = treeI->current->parent->next;
      while (treeNodeParent != NULL && tree_is_leaf(treeNodeParent))
        treeNodeParent = treeNodeParent->next;
      if (treeNodeParent != NULL)
      {
        treeI->current = treeNodeParent->firstChild;
        return;
      }
    }
    // try to go to next level
    while (treeI->current->prev != NULL)
      treeI->current = treeI->current->prev;
    while (treeI->current != NULL && tree_is_leaf(treeI->current))
      treeI->current = treeI->current->next;
    if (treeI->current != NULL)
      treeI->current = treeI->current->firstChild;
    break;
  }
}

void treeI_destroy(TreeIterator* treeI)
{
  safe_free(treeI);
}
