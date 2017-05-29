/// @file   mutex.h

#pragma once

#include "stdint.h"

typedef struct mutex_t
{
    uint8_t state;
    uint32_t owner;
} mutex_t;

void mutex_lock(mutex_t * mutex, uint32_t owner);

void mutex_unlock(mutex_t * mutex);