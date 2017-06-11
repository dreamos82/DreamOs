/// @file   stdlib.c

#include "stdlib.h"
#include "kheap.h"

/// Used to align the memory.
#define ALIGN(x) \
    (((x) + (sizeof(size_t) - 1)) & ~(sizeof(size_t) - 1))

void * calloc(size_t element_number, size_t element_size)
{
    register char * ptr;
    register size_t * q;
    size_t size = ALIGN(element_number * element_size);
    ptr = kmalloc(size);
    if (ptr == NULL)
    {
        return NULL;
    }
    q = (size_t *) (ptr + size);
    while ((char *) q > ptr) *--q = 0;

    rand();
    return ptr;
}

/// Seed used to generate random numbers.
int rseed = 0;

inline void srand(int x)
{
    rseed = x;
}

#ifndef MS_RAND

/// The maximum value returned by the rand function.
#define RAND_MAX ((1U << 31) - 1)

/// @brief Returns a pseudo-random integral number in the range
/// between 0 and RAND_MAX.
inline int rand()
{
    return rseed = (rseed * 1103515245 + 12345) & RAND_MAX;
}

#else /* MS rand */

/// The maximum 32bit value returned by the rand function.
#define RAND_MAX_32 ((1U << 31) - 1)
/// The maximum value returned by the rand function.
#define RAND_MAX ((1U << 15) - 1)

/// @brief Returns a pseudo-random integral number in the range
/// between 0 and RAND_MAX.
inline int rand()
{
    return (rseed = (rseed * 214013 + 2531011) & RAND_MAX_32) >> 16;
}

#endif /* MS_RAND */