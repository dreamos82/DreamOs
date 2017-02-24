/// @file   stdbool.h

#pragma once

/// Define boolean value.
typedef enum __attribute__ ((__packed__)) bool_t
{
    false,  ///< [0] False.
    true    ///< [1] True.
} bool_t;
