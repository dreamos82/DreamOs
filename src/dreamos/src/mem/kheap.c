/// @file   kheap.c
/// @brief  Functions used to manage the heap.
/// @author Dario Casalinuovo
/// @date   Oct 27 2003
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "kheap.h"
#include "debug.h"
#include "string.h"
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
    dbg_print("[%-4s][%-6s][%-12s][%-6s][%-6s][%-12s][%-12s]\n",
              "ID", "STATUS", "SIZE", "PREV", "NEXT", "START", "END");
    while (current)
    {
        dbg_print(" %4d  %6s  %12d  %6d  %6d  %12d  %12d\n",
                  current->id,
                  ((current->used ? "USED" : "FREE")),
                  current->size,
                  ((current->prev) ? current->prev->id : 0),
                  ((current->next) ? current->next->id : 0),
                  ((uint32_t) current) - HEAP_START,
                  ((uint32_t) current) + current->size - HEAP_START);
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
    chunk_t * last_chunk = first_chunk;
    // Iterate through the headers.
    while (last_chunk)
    {
        // Check if there is a next header.
        if (!last_chunk->next) break;
        // Move to the next element.
        last_chunk = last_chunk->next;
    }
    // If I've not found a suitable header, create a new one.
    uint32_t chunk_start = HEAP_START;
    if (last_chunk)
    {
        chunk_start = (uint32_t) last_chunk + last_chunk->size;
    }
    // Allocate the memory for a new chunk.
    chunk_t * new_chunk = create_chunk(chunk_start, size);
    // Insert the chunk after the previous one.
    insert_chunk_after(new_chunk, last_chunk);
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
    glue_chunk(header);
}
