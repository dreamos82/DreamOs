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
#include "assert.h"

void kernel_init_heap()
{
    heap_max = HEAP_START;
    first_chunk = NULL;
}

void print_heap()
{
    uint32_t count = 0;
    chunk_t * current = first_chunk;
    dbg_print("# -------------------------------------------\n");
    while (current)
    {
        dbg_print("[%4d][%s] Chunk: %d\n",
                  count,
                  (current->used ? "USED" : "FREE"),
                  current->size);
        // Move to the next chunk.
        current = current->next;
        // Increment the counter.
        ++count;
    }
    dbg_print("# -------------------------------------------\n");
}

void * kmalloc(size_t size)
{
    // Add to the size the header.
    size += sizeof(chunk_t);
    // Initialize a pointer which will point to the current header to the
    // begin of the heap.
    chunk_t * cur_header = first_chunk;
    chunk_t * prev_header = NULL;
    // Iterate through the headers.
    while (cur_header)
    {
        // Check if the current element of the heap has not been used, and its
        // length is greater than the required size.
        /*
        if ((cur_header->used == 0) && (cur_header->size >= size))
        {
            split_chunk(cur_header, size);
            cur_header->used = true;
            return (void *) ((uint32_t) cur_header + sizeof(chunk_t));
        }
         */
        // Set the previous element.
        prev_header = cur_header;
        // Move to the next element.
        cur_header = cur_header->next;
    }
    // If I've not found a suitable header, create a new one.
    uint32_t chunk_start = HEAP_START;
    if (prev_header)
    {
        chunk_start = (uint32_t) prev_header + prev_header->size;
    }
    // Allocate the memory for a new chunk.
    chunk_t * new_chunk = create_chunk(chunk_start, size);
    // Insert the chunk after the previous one.
    insert_chunk_after(new_chunk, prev_header);
    // If there is no first chunk set, use the new one.
    if (first_chunk == NULL)
    {
        first_chunk = new_chunk;
    }
    // Return a pointer to the allocated memory after the header.
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
    chunk_t * header = (chunk_t *) ((uint32_t) ptr - sizeof(chunk_t));
    assert(header && "Cannot find the header of the chunk.");
    header->used = false;
    //glue_chunk(header);
    free_chunk(header);
}
