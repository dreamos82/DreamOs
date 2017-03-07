// ordered_array.h -- Interface for creating, inserting and deleting
//                    from ordered arrays.
//                    Written for JamesM's kernel development tutorials.

#pragma once

#include <stdint.h>

/// @brief This array is insertion sorted - it always remains in a sorted state (between calls).
/// It can store anything that can be cast to a void* -- so a uint32_t, or any pointer.
typedef void * array_type_t;

/// @brief A predicate should return nonzero if the first argument is less
/// than the second. Else it should return zero.
typedef int8_t (* lessthan_predicate_t)(array_type_t, array_type_t);

typedef struct
{
    array_type_t * array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

/// @brief A standard less than predicate.
int8_t standard_lessthan_predicate(array_type_t a, array_type_t b);

/// @brief Create an ordered array.
ordered_array_t create_ordered_array(uint32_t max_size,
                                     lessthan_predicate_t less_than);

ordered_array_t place_ordered_array(void * addr,
                                    uint32_t max_size,
                                    lessthan_predicate_t less_than);

/// @brief Destroy an ordered array.
void destroy_ordered_array(ordered_array_t * array);

/// @brief Add an item into the array.
void insert_ordered_array(array_type_t item, ordered_array_t * array);

/// @brief Lookup the item at index i.
array_type_t lookup_ordered_array(uint32_t i, ordered_array_t * array);

/// @brief Deletes the item at location i from the array.
void remove_ordered_array(uint32_t i, ordered_array_t * array);
