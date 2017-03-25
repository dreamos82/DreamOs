/// @file   double_linked_list.h

#pragma once

#include "stddef.h"

/// @brief Structure used to implement a double linked list.
typedef struct double_linked_list
{
    /// @brief The previous element.
    struct double_linked_list * prev;
    /// @brief The subsequent element.
    struct double_linked_list * next;
} double_linked_list;

/// @brief Initialize the given linked list.
void dll_init(double_linked_list * dlist);

/// @brief Insert element d2 after d1.
void dll_insert_after(double_linked_list * d1, double_linked_list * d2);

/// @brief Insert element d2 before d1.
void dll_insert_before(double_linked_list * d1, double_linked_list * d2);

/// @brief Remove d from the list.
void dll_remove(double_linked_list * d);

/// @brief Push d1 to the from of the d2_ptr list.
void dll_push(double_linked_list * d1, double_linked_list ** d2_ptr);

/// @brief Pop the front of the dll list.
double_linked_list * dll_pop(double_linked_list ** dll);

/// @brief Remove d2 from the list, advancing d1p if needed.
void dll_remove_from(double_linked_list ** d1p, double_linked_list * d2);
