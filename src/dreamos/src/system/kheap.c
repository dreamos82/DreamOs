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
#include <stdio.h>
#include <paging.h>
#include <vm.h>
#include <debug.h>

static void alloc_chunk(uint32_t start, uint32_t len);

static void free_chunk(header_t * chunk);

static void split_chunk(header_t * chunk, uint32_t len);

static void glue_chunk(header_t * chunk);

uint32_t heap_max = HEAP_START;
header_t * heap_first = 0;

void kernel_init_heap()
{
}

void * kmalloc(size_t size)
{
    // Add to the size the header.
    size += sizeof(header_t);
    // Initialize a pointer which will point to the current header to the
    // begin of the heap.
    header_t * cur_header = heap_first;
    header_t * prev_header = 0;
    // Iterate through the headers.
    while (cur_header)
    {
        // Check if the current element of the heap has not been used, and its
        // length is greater than the required size.
        if ((cur_header->allocated == 0) && (cur_header->length >= size))
        {
            dbg_print("I've found a suitable header(%d).\n", size);
            split_chunk(cur_header, size);
            cur_header->allocated = 1;
            return (void *) ((uint32_t) cur_header + sizeof(header_t));
        }
        // Set the previous element.
        prev_header = cur_header;
        // Move to the next element.
        cur_header = cur_header->next;
    }
    dbg_print("Create a new header(%d).\n", size);
    // If I've not found a suitable header, create a new one.
    uint32_t chunk_start;
    if (prev_header)
    {
        chunk_start = (uint32_t) prev_header + prev_header->length;
    }
    else
    {
        chunk_start = HEAP_START;
        heap_first = (header_t *) chunk_start;
    }
    // Allocate the memory for a new chunk.
    alloc_chunk(chunk_start, size);
    cur_header = (header_t *) chunk_start;
    cur_header->prev = prev_header;
    cur_header->next = 0;
    cur_header->allocated = 1;
    cur_header->length = size;
    if (prev_header)
    {
        prev_header->next = cur_header;
    }
    return (void *) (chunk_start + sizeof(header_t));
}

void kfree(void * p)
{
    header_t * header = (header_t *) ((uint32_t) p - sizeof(header_t));
    dbg_print("Freeing memory at  : %p [%ld]\n", header, header);
    header->allocated = 0;
    glue_chunk(header);
}

void alloc_chunk(uint32_t start, uint32_t len)
{
    while (start + len > heap_max)
    {
        uint32_t page = kernel_alloc_page();
        map(heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += 0x1000;
    }
}

void free_chunk(header_t * chunk)
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

void split_chunk(header_t * chunk, uint32_t len)
{
    // In order to split a chunk, once we split we need to know that there will be enough
    // space in the new chunk to store the chunk header, otherwise it just isn't worthwhile.
    if (chunk->length - len > sizeof(header_t))
    {
        header_t * newchunk = (header_t *) ((uint32_t) chunk + chunk->length);
        newchunk->prev = chunk;
        newchunk->next = 0;
        newchunk->allocated = 0;
        newchunk->length = chunk->length - len;

        chunk->next = newchunk;
        chunk->length = len;
    }
}

void glue_chunk(header_t * chunk)
{
    dbg_print("Starting glue...\n");
    if (chunk->next)
    {
        if (chunk->next->allocated == 0)
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
        if (chunk->prev->allocated == 0)
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
