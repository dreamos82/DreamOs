/// @file   chunk.c

#include "chunk.h"
#include "debug.h"
#include "kheap.h"
#include "paging.h"
#include "vm.h"

chunk_t * create_chunk(const uint32_t start, const size_t size)
{
    chunk_t * new_chunk = (chunk_t *) ((uint32_t) start + size);
    new_chunk->id = chunk_counter++;
    new_chunk->prev = NULL;
    new_chunk->next = NULL;
    new_chunk->used = false;
    new_chunk->length = size;
    return new_chunk;
}

chunk_t * alloc_chunk(const uint32_t start, const size_t size)
{
    //uint32_t starting_heap = heap_top;
    uint32_t count = 0;
    while (heap_top < (start + size))
    {
        uint32_t page = kernel_alloc_page();
        map(heap_top, page, PAGE_PRESENT | PAGE_WRITE);
        heap_top += PAGE_SIZE;
        ++count;
    }
    chunk_t * new_chunk = create_chunk(start, size);
    /*
    dbg_print("# ALL  |%d3| OLD:%12p NEW:%12p ALL:%12p PAG:%12d REM:%12d SIZ:%12d BAL:%12d\n",
              new_chunk->id,
              starting_heap,
              heap_top,
              (heap_top - starting_heap),
              count,
              ((HEAP_END - heap_top) / PAGE_SIZE),
              size,
              (heap_top - starting_heap)-size);
    */
    return new_chunk;
}

chunk_t * find_free_chunk(const size_t size)
{
    chunk_t * current_chunk = first_chunk;
    while (current_chunk)
    {
        // Check if the chunk is currently used. If not, check if the element
        // fits the current chunk.
        if ((!current_chunk->used) && (current_chunk->length >= size))
        {
            return current_chunk;
        }
        // Move to the next element.
        current_chunk = current_chunk->next;
    }
    return NULL;
}

void free_chunk(chunk_t * chunk)
{
    // Check if the given chunk is the first chunk.
    if (chunk == first_chunk)
    {
        first_chunk = NULL;
    }
    // If the given chunk has a predecessor, unlink the current chunk.
    if (chunk->prev != NULL)
    {
        chunk->prev->next = NULL;
    }
    //uint32_t starting_heap = heap_top;
    uint32_t count = 0;
    // While the heap max can contract by a page and still be greater than
    // the chunk address...
    while ((heap_top - PAGE_SIZE) >= (uint32_t) chunk)
    {
        heap_top -= PAGE_SIZE;
        uint32_t page;
        get_mapping(heap_top, &page);
        kernel_free_page(page);
        unmap(heap_top);
        ++count;
    }
    /*
    dbg_print(
            "# FREE |%d3| OLD:%12p NEW:%12p ALL:%12p PAG:%12d REM:%12d SIZ:%12d\n",
            chunk->id,
            starting_heap,
            heap_top,
            (heap_top - starting_heap),
            count,
            ((HEAP_END - heap_top) / PAGE_SIZE),
            chunk->length);
            */
}

void split_chunk(chunk_t * chunk, const size_t size)
{
    if (chunk->length > size)
    {
        chunk_t * new_chunk = create_chunk((uint32_t) chunk + chunk->length,
                                           chunk->length - size);
        // Insert the new chunk after the previous.
        chunk_insert_after(new_chunk, chunk);
        // Set new length of the previous chunk to the required size.
        chunk->length = size;
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

void chunk_insert_after(chunk_t * chunk1, chunk_t * chunk2)
{
    chunk1->prev = chunk2;
    chunk2->next = chunk1;
}