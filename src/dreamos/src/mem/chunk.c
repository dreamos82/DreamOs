/// @file   chunk.c

#include "chunk.h"
#include "debug.h"
#include "kheap.h"
#include "paging.h"
#include "vm.h"


void alloc_chunk(uint32_t start, uint32_t len)
{
    while (start + len > heap_max)
    {
        uint32_t page = kernel_alloc_page();
        map(heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += 0x1000;
    }
}

void free_chunk(chunk_t * chunk)
{
    dbg_print("Freeing chunk at : %p\n", chunk);
    chunk->prev->next = 0;

    if (chunk->prev == 0)
        heap_first = 0;

    // While the heap max can contract by a page and still be greater than the chunk address...
    while ((heap_max - 0x1000) >= (uint32_t) chunk)
    {
        heap_max -= 0x1000;
        uint32_t page;
        get_mapping(heap_max, &page);
        kernel_free_page(page);
        unmap(heap_max);
    }
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
        newchunk->used = 0;
        newchunk->length = chunk->length - len;

        chunk->next = newchunk;
        chunk->length = len;
    }
}

void glue_chunk(chunk_t * chunk)
{
    dbg_print("Starting glue...\n");
    if (chunk->next)
    {
        if (chunk->next->used == 0)
        {
            dbg_print("Glue next\n");
            dbg_print("Update length\n");
            chunk->length = chunk->length + chunk->next->length;
            dbg_print("Update chunk->next->next->prev\n");
            chunk->next->next->prev = chunk;
            dbg_print("Update chunk->next\n");
            chunk->next = chunk->next->next;
            dbg_print("Done next...\n");
        }
    }
    if (chunk->prev)
    {
        if (chunk->prev->used == 0)
        {
            dbg_print("Glue prev\n");
            chunk->prev->length = chunk->prev->length + chunk->length;
            chunk->prev->next = chunk->next;
            chunk->next->prev = chunk->prev;
            chunk = chunk->prev;
            dbg_print("Done prev...\n");
        }
    }
    if (chunk->next == NULL)
    {
        free_chunk(chunk);
    }
}