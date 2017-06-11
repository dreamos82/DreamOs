/// @file   generic_tree.h

#pragma once

#include "list.h"

/// @brief A node of the generic tree.
typedef struct gtreenode_t
{
    /// Children of the current node.
    list_t * children;
    /// Value pointed by the node.
    void * value;
} gtreenode_t;

/// @brief The structure of the tree.
typedef struct gtree_t
{
    /// The root of the tree.
    gtreenode_t * root;
} gtree_t;

/// @brief Create a tree with root = NULL.
gtree_t * tree_create();

/// @brief Create a tree node with specified value, and a list of 0 children.
gtreenode_t * treenode_create(void * value);

/// @brief Insert a node under subroot.
gtreenode_t * tree_insert(
    gtree_t * tree,
    gtreenode_t * subroot,
    void * value);

/// @brief Find the parent with the given index.
gtreenode_t * tree_find_parent(
    gtree_t * tree,
    gtreenode_t * remove_node,
    int * child_index);

/// @brief Remove the given node from the tree.
void tree_remove(
    gtree_t * tree,
    gtreenode_t * remove_node);

/// @brief Transform the tree to a list.
void tree2list(
    gtree_t * tree,
    list_t * list);

/// @brief Transform a tree into an array.
void tree2array(
    gtree_t * tree,
    void ** array,
    int * size);
