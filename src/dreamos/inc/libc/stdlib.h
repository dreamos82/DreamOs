/// @file   stdlib.h

#pragma once

#include "stddef.h"

/// @brief Malloc based on the number of elements.
void * calloc(size_t element_number, size_t element_size);

/// @brief Allows to set the seed of the random value generator.
void srand(int x);

#ifndef MS_RAND

/// The maximum value of the random.
#define RAND_MAX ((1U << 31) - 1)

/// @brief Generates a random value.
int rand();

#else /* MS rand */

#define RAND_MAX_32 ((1U << 31) - 1)
#define RAND_MAX ((1U << 15) - 1)

int rand();

#endif /* MS_RAND */
