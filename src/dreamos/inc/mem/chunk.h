/// @file   chunk.h

#pragma once

#include "stdint.h"

typedef struct chunk_t
{
    struct chunk_t * prev;
    struct chunk_t * next;
    uint32_t allocated : 1;
    uint32_t length : 31;
} chunk_t;

void alloc_chunk(uint32_t start, uint32_t len);

void free_chunk(chunk_t * chunk);

void split_chunk(chunk_t * chunk, uint32_t len);

void glue_chunk(chunk_t * chunk);