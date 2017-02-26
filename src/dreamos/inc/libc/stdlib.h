/// @file   stdlib.h

#pragma once

#include <stddef.h>

/// Determines the minimum value between the two.
#define min(x, y)  (((y) < (x)) ? (y) : (x))

/// Determines the maximum value between the two.
#define max(x, y)  (((y) > (x)) ? (y) : (x))

void * calloc(size_t element_number, size_t element_size);
