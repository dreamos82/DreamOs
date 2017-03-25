/// @file   chunk.h

#pragma once

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"

typedef struct chunk_t
{
    struct chunk_t * prev;
    struct chunk_t * next;
    uint32_t length : 31;
    bool_t used;
} chunk_t;

chunk_t * alloc_chunk(const uint32_t start, const size_t size);

void free_chunk(chunk_t * chunk);

void split_chunk(chunk_t * chunk, uint32_t len);

void glue_chunk(chunk_t * chunk);