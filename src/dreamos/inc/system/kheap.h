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

#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

#define HEAP_START 0xD0000000
#define HEAP_END   0xFFBFF000

typedef struct header_t
{
    struct header_t * prev;
    struct header_t * next;
    uint32_t allocated : 1;
    uint32_t length : 31;
} header_t;

void kernel_init_heap();

// Returns a pointer to a chunk of memory, minimum size 'l' bytes.
void * kmalloc(size_t size);

// Takes a chunk of memory allocated with kmalloc, and returns it to the pool of usable memory.
void free(void * p);

#endif
