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

/// @brief Represents a chunk of memory.
typedef struct chunk_t
{
    /// The id of the chunk.
    uint32_t id;
    /// A pointer to the previous chunk.
    struct chunk_t * prev;
    /// A pointer to the next chunk.
    struct chunk_t * next;
    /// The size of the chunk.
    uint32_t size;
    /// Flag used to check if the chunk is currently used.
    bool_t used;
} chunk_t;

/// @brief Allows to create a new chunk.
/// @param start The starting address of the new chunk.
/// @param size  The size of the new chunk.
/// @return A pointer to the newly created chunk.
chunk_t * create_chunk(const uint32_t start, const size_t size);

/// @brief Allocate a new chunk of the given size starting from 'start'.
/// @param start The address from which the chunk should be placed.
/// @param size  The size of the chunk.
/// @return The allocated chunk.
void alloc_chunk(const uint32_t start, const size_t size);

/// @brief Free the memory occupied by the chunk.
void free_chunk(chunk_t * chunk);

/// @brief Splits a chunk.
void split_chunk(chunk_t * chunk, const size_t size);

/// @brief Glue togheter unused close chunks.
void glue_chunk(chunk_t * chunk);

/// @brief Insert chunk1 after chunk2.
void insert_chunk_after(chunk_t * chunk1, chunk_t * chunk2);
