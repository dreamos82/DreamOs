/// @file   stdlib.h

#pragma once

#include "stddef.h"

/// Determines the minimum value between the two.
#define min(x, y)  (((y) < (x)) ? (y) : (x))

/// Determines the maximum value between the two.
#define max(x, y)  (((y) > (x)) ? (y) : (x))

void * calloc(size_t element_number, size_t element_size);

void srand(int x);

#ifndef MS_RAND

#define RAND_MAX ((1U << 31) - 1)

int rand();

#else /* MS rand */

#define RAND_MAX_32 ((1U << 31) - 1)
#define RAND_MAX ((1U << 15) - 1)

int rand();

#endif /* MS_RAND */
