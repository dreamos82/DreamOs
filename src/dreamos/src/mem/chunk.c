/// @file   chunk.c

#include "chunk.h"
#include "debug.h"
#include "kheap.h"
#include "paging.h"
#include "vm.h"
#include "assert.h"

uint32_t chunk_id = 0;

chunk_t * create_chunk(const uint32_t start, const size_t size)
{
    // First, allocate the memory for the chunk.
    alloc_chunk(start, size);
    // Create a pointer to the memory area.
    chunk_t * new_chunk = (chunk_t *) start;
    // Initialize the header.
    new_chunk->id = chunk_id++;
    new_chunk->prev = NULL;
    new_chunk->next = NULL;
    new_chunk->size = size;
    new_chunk->used = true;
    return new_chunk;
}

void alloc_chunk(const uint32_t start, const size_t size)
{
    while (heap_max < (start + size))
    {
        uint32_t page = kernel_alloc_page();
        map(heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += PAGE_SIZE;
    }
}

void free_chunk(chunk_t * chunk)
{
    assert(chunk && "Received null chunk.");
    if (chunk->prev == NULL)
    {
        first_chunk = NULL;
    }
    else
    {
        chunk->prev->next = NULL;
    }
    uint32_t size = chunk->size;
    for (uint32_t it = (uint32_t) chunk; it <= size; it += PAGE_SIZE)
    {
        uint32_t page;
        get_mapping(it, &page);
        kernel_free_page(page);
        unmap(it);
    }
}

void split_chunk(chunk_t * chunk, uint32_t len)
{
    // In order to split a chunk, once we split we need to know that there will be enough
    // space in the new chunk to store the chunk header, otherwise it just isn't worthwhile.
    if (chunk->size - len > sizeof(chunk_t))
    {
        chunk_t * newchunk = (chunk_t *) ((uint32_t) chunk + chunk->size);
        newchunk->prev = chunk;
        newchunk->next = 0;
        newchunk->size = chunk->size - len;
        newchunk->used = false;

        chunk->next = newchunk;
        chunk->size = len;
    }
}

void glue_chunk(chunk_t * chunk)
{
    if (chunk->next)
    {
        if (!chunk->next->used)
        {
            dbg_print("Glue next\n");
            dbg_print("Update length\n");
            chunk->size = chunk->size + chunk->next->size;
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
            chunk->prev->size = chunk->prev->size + chunk->size;
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

void insert_chunk_after(chunk_t * chunk1, chunk_t * chunk2)
{
    if (chunk1)
    {
        chunk1->prev = chunk2;
    }
    if (chunk2)
    {
        chunk2->next = chunk1;
    }
}
