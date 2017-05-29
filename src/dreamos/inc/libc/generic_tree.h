/// @file   generic_tree.h

#pragma once

#include "list.h"

typedef struct gtreenode
{
    list_t * children;
    void * value;
} gtreenode_t;

typedef struct gtree
{
    gtreenode_t * root;
} gtree_t;

/// @brief Create a tree with root = NULL.
gtree_t * tree_create();

/// @brief Create a tree node with specified value, and a list of 0 children.
gtreenode_t * treenode_create(void * value);

/// @brief Insert a node under subroot.
gtreenode_t * tree_insert(gtree_t * tree, gtreenode_t * subroot, void * value);

gtreenode_t * tree_find_parent(gtree_t * tree,
                               gtreenode_t * remove_node,
                               int * child_index);

gtreenode_t * tree_find_parent_recur(gtree_t * tree,
                                     gtreenode_t * remove_node,
                                     gtreenode_t * subroot,
                                     int * child_index);

void tree_remove(gtree_t * tree, gtreenode_t * remove_node);

void tree2list_recur(gtreenode_t * subroot, list_t * list);

void tree2list(gtree_t * tree, list_t * list);

void tree2array(gtree_t * tree, void ** array, int * size);

void tree2array_recur(gtreenode_t * subroot, void ** array, int * size);
