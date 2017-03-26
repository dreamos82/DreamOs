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
    heap_max = HEAP_START;
    dbg_print("# -------------------------------------------\n");
    dbg_print("# Heap Start : %12p\n", HEAP_START);
    dbg_print("# Heap End   : %12p\n", HEAP_END);
    dbg_print("# Pages      : %12d\n", ((HEAP_END - HEAP_START) / PAGE_SIZE));
    dbg_print("# -------------------------------------------\n");
}

void print_heap()
{
    uint32_t count = 0;
    chunk_t * current = heap_first;
    dbg_print("# -------------------------------------------\n");
    while (current)
    {
        dbg_print("[%4d][%s] Chunk: %d\n",
                  count,
                  (current->used ? "USED" : "FREE"),
                  current->length);
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
    chunk_t * cur_header = heap_first;
    chunk_t * prev_header = NULL;
    // Iterate through the headers.
    while (cur_header)
    {
        // Check if the current element of the heap has not been used, and its
        // length is greater than the required size.
        if ((!cur_header->used) && (cur_header->length >= size))
        {
            split_chunk(cur_header, size);
            cur_header->used = true;
            return (void *) ((uint32_t) cur_header + sizeof(chunk_t));
        }
        // Set the previous element.
        prev_header = cur_header;
        // Move to the next element.
        cur_header = cur_header->next;
    }
    // If I've not found a suitable header, create a new one.
    uint32_t chunk_start;
    if (prev_header)
    {
        chunk_start = (uint32_t) prev_header + prev_header->length;
    }
    else
    {
        chunk_start = HEAP_START;
        heap_first = (chunk_t *) HEAP_START;
    }

    // Allocate the memory for a new chunk.
    cur_header = alloc_chunk(chunk_start, size);
    cur_header->prev = prev_header;
    cur_header->next = 0;
    cur_header->used = true;
    cur_header->length = size;
    if (prev_header)
    {
        prev_header->next = cur_header;
    }
    return (void *) (chunk_start + sizeof(chunk_t));
}

void * kcalloc(uint32_t num, uint32_t size)
{
    void * ptr = kmalloc(num * size);
    memset(ptr, 0, num * size);
    return ptr;
}

void kfree(void * p)
{
    chunk_t * header = (chunk_t *) ((uint32_t) p - sizeof(chunk_t));
    header->used = false;
    glue_chunk(header);
}
