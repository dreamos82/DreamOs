/// @file   chunk.h

#pragma once

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"

typedef struct chunk_t
{
    uint32_t id;
    struct chunk_t * prev;
    struct chunk_t * next;
    uint32_t length : 31;
    bool_t used;
} chunk_t;

uint32_t chunk_counter;
/// Pointer to the first chunk.
chunk_t * first_chunk;
/// Pointer to the last chunk.
chunk_t * last_chunk;
/// @brief The top of the heap.
uint32_t heap_top;

/// @brief Allocate a new chunk of the given size starting from 'start'.
/// @param start The address from which the chunk should be placed.
/// @param size  The size of the chunk.
/// @return The allocated chunk.
chunk_t * alloc_chunk(const uint32_t start, const size_t size);

/// @brief Search a free chunk which can contain the given size.
/// @param size The required size.
/// @return A pointer to a chunk which can contain the given size.
chunk_t * find_free_chunk(const size_t size);

void free_chunk(chunk_t * chunk);

void split_chunk(chunk_t * chunk, const size_t size);

void glue_chunk(chunk_t * chunk);

/// @brief Insert chunk1 after chunk2.
void chunk_insert_after(chunk_t * chunk1, chunk_t * chunk2);