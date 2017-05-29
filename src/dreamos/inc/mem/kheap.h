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

#pragma once

#include "stdint.h"
#include "stddef.h"
#include "chunk.h"

#define HEAP_START 0xD0000000
#define HEAP_END   0xFFBFF000

uint32_t heap_max;
chunk_t * first_chunk;

/// @brief Initialize the heap.
void kernel_init_heap();

void print_heap();

/// @brief Returns a pointer to a chunk of memory.
void * kmalloc(size_t size);

/// @brief Returns a pointer to a number of chunks of memory.
void * kcalloc(uint32_t num, uint32_t size);

/// @brief Takes a chunk of memory allocated with kmalloc, and returns it to
/// the pool of usable memory.
void kfree(void * ptr);
