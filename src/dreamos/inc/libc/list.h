/// @file   list.h
/// @brief An implementation for generic, doubly linked list, may be handy in
/// the future when we do vfs, process manmagement, etc..

#pragma once

#include "stdint.h"
#include "stddef.h"

/// @brief Represent the node of a list.
typedef struct listnode_t
{
    /// A pointer to the value.
    void * value;
    /// The previous node.
    struct listnode_t * prev;
    /// The next node.
    struct listnode_t * next;
} listnode_t;

/// @brief Represent the list.
typedef struct list_t
{
    /// The first element of the list.
    listnode_t * head;
    /// The last element of the list.
    listnode_t * tail;
    /// The size of the list.
    size_t size;
} list_t;

/// @brief Macro used to iterate through a list.
#define listnode_foreach(t, list) \
    for(listnode_t * t = list->head; t != NULL; t = t->next)

/// @brief Create a list and set head, tail to NULL, and size to 0.
list_t * list_create();

/// @brief Get list size.
size_t list_size(list_t * list);

/// @brief Insert a value at the front of list.
listnode_t * list_insert_front(list_t * list, void * value);

/// @brief Insert a value at the back of list.
listnode_t * list_insert_back(list_t * list, void * value);

/// @brief Given a listnode, remove it from lis.
void * list_remove_node(list_t * list, listnode_t * node);

/// @brief Remove a value at the front of list.
void * list_remove_front(list_t * list);

/// @brief Remove a value at the back of list.
void * list_remove_back(list_t * list);

/// @brief Insert after tail of list(same as insert back).
void list_push(list_t * list, void * value);

/// @brief Remove and return tail of list(user is responsible for freeing the
/// returned node and the value).
listnode_t * list_pop(list_t * list);

/// @brief Insert before head of list(same as insert front).
void list_enqueue(list_t * list, void * value);

/// @brief Remove and return tail of list(same as list_pop).
listnode_t * list_dequeue(list_t * list);

/// @brief Get the value of the first element but not remove it.
void * list_peek_front(list_t * list);

/// @brief Get the value of the last element but not remove it.
void * list_peek_back(list_t * list);

/// @brief Destory a list.
void list_destroy(list_t * list);

/// @brief Destroy a node of the list.
void listnode_destroy(listnode_t * node);

/// @brief Does the list contain a value (Return -1 if list element is not
/// found).
int list_contain(list_t * list, void * value);

/// @brief Returns the node at the given index.
listnode_t * list_get_node_by_index(list_t * list, size_t index);

/// @brief Removes a node from the list at the given index.
void * list_remove_by_index(list_t * list, size_t index);
