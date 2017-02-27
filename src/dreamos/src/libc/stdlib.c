/// @file   stdlib.c

#include <stdlib.h>
#include <kheap.h>

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
    return ptr;
}
