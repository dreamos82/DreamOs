/// @file   chunk.h
/// @brief  Functions used to manage memory chunks.
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

#pragma once

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"

typedef struct chunk_t
{
    uint32_t id;
    struct chunk_t * prev;
    struct chunk_t * next;
    uint32_t size;
    bool_t used;
} chunk_t;

chunk_t * create_chunk(const uint32_t start, const size_t size);

/// @brief Allocate a new chunk of the given size starting from 'start'.
/// @param start The address from which the chunk should be placed.
/// @param size  The size of the chunk.
/// @return The allocated chunk.
void alloc_chunk(const uint32_t start, const size_t size);

void free_chunk(chunk_t * chunk);

void split_chunk(chunk_t * chunk, const size_t size);

void glue_chunk(chunk_t * chunk);

void insert_chunk_after(chunk_t * chunk1, chunk_t * chunk2);