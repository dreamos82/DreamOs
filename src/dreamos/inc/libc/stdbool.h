/// @file   stdbool.h

#pragma once

/// @brief Define boolean value.
typedef enum bool_t
{
    false,  ///< [0] False.
    true    ///< [1] True.
} __attribute__ ((__packed__)) bool_t;
