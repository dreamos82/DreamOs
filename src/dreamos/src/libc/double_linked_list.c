/// @file   double_linked_list.c

#include "double_linked_list.h"

void dll_init(double_linked_list * dlist)
{
    dlist->next = dlist->prev = dlist;
}

void dll_insert_after(double_linked_list * d1, double_linked_list * d2)
{
    double_linked_list * d1n = d1->next;
    double_linked_list * d2p = d2->prev;
    d1->next = d2;
    d2->prev = d1;
    d2p->next = d1n;
    d1n->prev = d2p;
}

void dll_insert_before(double_linked_list * d1, double_linked_list * d2)
{
    double_linked_list * d1p = d1->prev;
    double_linked_list * d2p = d2->prev;
    d1p->next = d2;
    d2->prev = d1p;
    d2p->next = d1;
    d1->prev = d2p;
}

void dll_remove(double_linked_list * d)
{
    d->prev->next = d->next;
    d->next->prev = d->prev;
    d->next = d;
    d->prev = d;
}

void dll_push(double_linked_list * d1, double_linked_list ** d2_ptr)
{
    if ((*d2_ptr) != NULL)
    {
        dll_insert_before((*d2_ptr), d1);
    }
    (*d2_ptr) = d1;
}

double_linked_list * dll_pop(double_linked_list ** dll)
{
    double_linked_list * dll_ptr = (*dll);
    double_linked_list * dll_next = dll_ptr->next;
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

void dll_remove_from(double_linked_list ** d1p, double_linked_list * d2)
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