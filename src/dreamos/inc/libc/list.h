/// @file   list.h
/// @brief An implementation for generic, doubly linked list, may be handy in
/// the future when we do vfs, process manmagement, etc..

#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct listnode
{
    void * val;
    struct listnode * prev;
    struct listnode * next;
} listnode_t;

typedef struct list
{
    listnode_t * head;
    listnode_t * tail;
    size_t size;
} list_t;

#define foreach(t, list) for(listnode_t * t = list->head; t != NULL; t = t->next)

/// @brief Create a list and set head, tail to NULL, and size to 0.
list_t * list_create();

/// @brief Get list size.
size_t list_size(list_t * list);

/// @brief Insert a value at the front of list.
listnode_t * list_insert_front(list_t * list, void * val);

/// @brief Insert a value at the back of list.
listnode_t * list_insert_back(list_t * list, void * val);

/// @brief Given a listnode, remove it from lis.
void * list_remove_node(list_t * list, listnode_t * node);

/// @brief Remove a value at the front of list.
void * list_remove_front(list_t * list);

/// @brief Remove a value at the back of list.
void * list_remove_back(list_t * list);

/// @brief Insert after tail of list(same as insert back).
void list_push(list_t * list, void * val);

/// @brief Remove and return tail of list(user is responsible for freeing the
/// returned node and the value).
listnode_t * list_pop(list_t * list);

/// @brief Insert before head of list(same as insert front).
void list_enqueue(list_t * list, void * val);

/// @brief Remove and return tail of list(same as list_pop).
listnode_t * list_dequeue(list_t * list);

/// @brief Get the value of the first element but not remove it.
void * list_peek_front(list_t * list);

/// @brief Get the value of the last element but not remove it.
void * list_peek_back(list_t * list);

void list_destroy(list_t * list);

void listnode_destroy(listnode_t * node);

/// @brief Does the list contain a value (Return -1 if list element is not
/// found).
int list_contain(list_t * list, void * val);

listnode_t * list_get_node_by_index(list_t * list, size_t index);

void * list_remove_by_index(list_t * list, size_t index);