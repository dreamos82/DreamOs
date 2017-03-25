/// @file   double_linked_list.c

#include "double_linked_list.h"

void dll_init(double_linked_list_t * dlist)
{
    dlist->next = dlist->prev = dlist;
}

void dll_insert_after(double_linked_list_t * d1, double_linked_list_t * d2)
{
    double_linked_list_t * d1n = d1->next;
    double_linked_list_t * d2p = d2->prev;
    d1->next = d2;
    d2->prev = d1;
    d2p->next = d1n;
    d1n->prev = d2p;
}

void dll_insert_before(double_linked_list_t * d1, double_linked_list_t * d2)
{
    double_linked_list_t * d1p = d1->prev;
    double_linked_list_t * d2p = d2->prev;
    d1p->next = d2;
    d2->prev = d1p;
    d2p->next = d1;
    d1->prev = d2p;
}

void dll_remove(double_linked_list_t * d)
{
    d->prev->next = d->next;
    d->next->prev = d->prev;
    d->next = d;
    d->prev = d;
}

void dll_push(double_linked_list_t ** d1p, double_linked_list_t * d2)
{
    //printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p != NULL)
    {
        dll_insert_before(*d1p, d2);
    }
    *d1p = d2;
}

double_linked_list_t * dll_pop(double_linked_list_t ** dll)
{
    double_linked_list_t * dll_ptr = (*dll);
    double_linked_list_t * dll_next = dll_ptr->next;
    dll_remove(dll_ptr);
    if (dll_ptr == dll_next)
    {
        (*dll) = NULL;
    }
    else
    {
        (*dll) = dll_next;
    }
    return dll_ptr;
}

void dll_remove_from(double_linked_list_t ** d1p, double_linked_list_t * d2)
{
    if (*d1p == d2)
    {
        dll_pop(d1p);
    }
    else
    {
        dll_remove(d2);
    }
}