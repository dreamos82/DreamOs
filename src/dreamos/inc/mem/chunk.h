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