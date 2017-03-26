/*
 * Copyright (c), Dario Casalinuovo
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

//
// Based on JamesM's kernel developement tutorials.
//
#include <kheap.h>
#include <paging.h>
#include <vm.h>
#include <debug.h>
#include <string.h>

void kernel_init_heap()
{
    chunk_counter = 0;
    heap_top = HEAP_START;
    dbg_print("# -------------------------------------------\n");
    dbg_print("# Heap Start : %12p\n", HEAP_START);
    dbg_print("# Heap End   : %12p\n", HEAP_END);
    dbg_print("# Pages      : %12d\n", ((HEAP_END - HEAP_START) / PAGE_SIZE));
    dbg_print("# -------------------------------------------\n");
}

void print_heap()
{
    uint32_t count = 0;
    chunk_t * current_chunk = first_chunk;
    dbg_print("# -------------------------------------------\n");
    while (current_chunk)
    {
        dbg_print("[%4d][%s] Chunk: %d\n",
                  count,
                  (current_chunk->used ? "USED" : "FREE"),
                  current_chunk->length);
        // Move to the next chunk.
        current_chunk = current_chunk->next;
        // Increment the counter.
        ++count;
    }
    dbg_print("# -------------------------------------------\n");
}

void * kmalloc(size_t size)
{
    // Add to the size the header.
    size += sizeof(chunk_t);
    // -------------------------------------------------------------------------
    // Check if there is a free chunk.
    chunk_t * free_chunk = find_free_chunk(size);
    if (free_chunk)
    {
        split_chunk(free_chunk, size);
        free_chunk->used = true;
        return (void *) ((uint32_t) free_chunk + sizeof(chunk_t));
    }
    // -------------------------------------------------------------------------
    // Check if the pointer to the first chunk is set.
    if (!first_chunk)
    {
        first_chunk = (chunk_t *) HEAP_START;
    }
    // -------------------------------------------------------------------------
    // Get the starting address of the new chunk.
    // By default set the initial address of the chunk at the starting point
    // of the heap.
    uint32_t chunk_start = HEAP_START;
    // Check if there is a last chunk.
    if (last_chunk)
    {
        chunk_start = (uint32_t) last_chunk + last_chunk->length;
    }
    // Allocate the memory for a new chunk.
    chunk_t * new_chunk = alloc_chunk(chunk_start, size);
    // -------------------------------------------------------------------------
    if (last_chunk)
    {
        last_chunk->next = new_chunk;
        new_chunk->prev = last_chunk;
    }
    // Set the new chunk as last chunk.
    last_chunk = new_chunk;
    return (void *) (chunk_start + sizeof(chunk_t));
}

void * kcalloc(uint32_t num, uint32_t size)
{
    void * ptr = kmalloc(num * size);
    memset(ptr, 0, num * size);
    return ptr;
}

void kfree(void * ptr)
{
    // --------------------------------
    // | HEADER | DATA                |
    // --------------------------------
    // In order to take the real chunk, we need to move backward from the
    // pointer to the header of the chunk.
    chunk_t * header = (chunk_t *) ((uint32_t) ptr - sizeof(chunk_t));
    // Check if it is a valid header.
    if (header)
    {
        header->used = false;
        free_chunk(header);
//        glue_chunk(header);
    }
}
