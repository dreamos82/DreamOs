/// @file   list.c

#include "list.h"
#include "kheap.h"

list_t * list_create()
{
    list_t * list = kcalloc(sizeof(list_t), 1);
    return list;
}

size_t list_size(list_t * list)
{
    if (!list) return 0;
    return list->size;
}

listnode_t * list_insert_front(list_t * list, void * value)
{
    listnode_t * t = kcalloc(sizeof(listnode_t), 1);
    list->head->prev = t;
    t->next = list->head;
    t->value = value;

    // If it's the first element, then it's both head and tail
    if (!list->head)
        list->tail = t;

    list->head = t;
    list->size++;
    return t;
}

listnode_t * list_insert_back(list_t * list, void * value)
{
    listnode_t * t = kcalloc(sizeof(listnode_t), 1);
    t->prev = list->tail;
    if (list->tail)
        list->tail->next = t;
    t->value = value;

    if (!list->head)
        list->head = t;

    list->tail = t;
    list->size++;
    return t;
}

void * list_remove_node(list_t * list, listnode_t * node)
{
    void * value = node->value;
    if (list->head == node)
        return list_remove_front(list);
    else if (list->tail == node)
        return list_remove_back(list);
    else
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        list->size--;
        kfree(node);
    }
    return value;
}

void * list_remove_front(list_t * list)
{
    if (!list->head) return NULL;
    listnode_t * t = list->head;
    void * value = t->value;
    list->head = t->next;
    if (list->head)
        list->head->prev = NULL;
    kfree(t);
    list->size--;
    return value;
}

void * list_remove_back(list_t * list)
{
    if (!list->head) return NULL;
    listnode_t * t = list->tail;
    void * value = t->value;
    list->tail = t->prev;
    if (list->tail)
        list->tail->next = NULL;
    kfree(t);
    list->size--;
    return value;
}

void list_push(list_t * list, void * value)
{
    list_insert_back(list, value);
}

listnode_t * list_pop(list_t * list)
{
    if (!list->head) return NULL;
    listnode_t * t = list->tail;
    list->tail = t->prev;
    if (list->tail)
        list->tail->next = NULL;
    list->size--;
    return t;
}

void list_enqueue(list_t * list, void * value)
{
    list_insert_front(list, value);
}

listnode_t * list_dequeue(list_t * list)
{
    return list_pop(list);
}

void * list_peek_front(list_t * list)
{
    if (!list->head) return NULL;
    return list->head->value;
}

void * list_peek_back(list_t * list)
{
    if (!list->tail) return NULL;
    return list->tail->value;
}

int list_contain(list_t * list, void * value)
{
    int idx = 0;
    listnode_foreach(listnode, list)
    {
        if (listnode->value == value)
            return idx;
        idx++;
    }
    return -1;
}

listnode_t * list_get_node_by_index(list_t * list, size_t index)
{
    if (index >= list_size(list))
    {
        return NULL;
    }
    size_t curr = 0;
    listnode_foreach(listnode, list)
    {
        if (index == curr) return listnode;
        curr++;
    }
    return NULL;
}

void * list_remove_by_index(list_t * list, size_t index)
{
    listnode_t * node = list_get_node_by_index(list, index);
    return list_remove_node(list, node);
}

void list_destroy(list_t * list)
{
    // Free each node's value and the node itself
    listnode_t * node = list->head;
    while (node != NULL)
    {
        listnode_t * save = node;
        node = node->next;
        kfree(save);
    }
    // Free the list
    kfree(list);
}

void listnode_destroy(listnode_t * node)
{
    kfree(node);
}
