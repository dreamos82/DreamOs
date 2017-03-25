/// @file   chunk.c

#include "chunk.h"
#include "debug.h"
#include "kheap.h"
#include "paging.h"
#include "vm.h"

void alloc_chunk(const uint32_t start, const size_t size)
{
    dbg_print("Allocating chunk at %p of dimension %d.\n", start, size);
    while ((start + size) > heap_max)
    {
        uint32_t page = kernel_alloc_page();
        map(heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += PAGE_SIZE;
    }
}

void free_chunk(chunk_t * chunk)
{
    chunk->prev->next = 0;
    if (chunk->prev == 0)
    {
        heap_first = 0;
    }
    // While the heap max can contract by a page and still be greater than
    // the chunk address...
    while ((heap_max - PAGE_SIZE) >= (uint32_t) chunk)
    {
        heap_max -= PAGE_SIZE;
        uint32_t page;
        get_mapping(heap_max, &page);
        kernel_free_page(page);
        unmap(heap_max);
    }
    dbg_print("Freeing chunk at    %p.\n", chunk);
}

void split_chunk(chunk_t * chunk, uint32_t len)
{
    // In order to split a chunk, once we split we need to know that there will be enough
    // space in the new chunk to store the chunk header, otherwise it just isn't worthwhile.
    if (chunk->length - len > sizeof(chunk_t))
    {
        chunk_t * newchunk = (chunk_t *) ((uint32_t) chunk + chunk->length);
        newchunk->prev = chunk;
        newchunk->next = 0;
        newchunk->used = false;
        newchunk->length = chunk->length - len;

        chunk->next = newchunk;
        chunk->length = len;
    }
}

void glue_chunk(chunk_t * chunk)
{
    if (chunk->next)
    {
        if (!chunk->next->used)
        {
            chunk->length = chunk->length + chunk->next->length;
            chunk->next->next->prev = chunk;
            chunk->next = chunk->next->next;
        }
    }
    if (chunk->prev)
    {
        if (!chunk->prev->used)
        {
            chunk->prev->length = chunk->prev->length + chunk->length;
            chunk->prev->next = chunk->next;
            chunk->next->prev = chunk->prev;
            chunk = chunk->prev;
        }
    }
    if (chunk->next == NULL)
    {
        free_chunk(chunk);
    }
}