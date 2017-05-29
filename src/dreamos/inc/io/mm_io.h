/// @file   mmio.h

#pragma once

#include "stdint.h"

uint8_t in_memb(uint32_t addr);

uint16_t in_mems(uint32_t addr);

uint32_t in_meml(uint32_t addr);

void out_memb(uint32_t addr, uint8_t value);

void out_mems(uint32_t addr, uint16_t value);

void out_meml(uint32_t addr, uint32_t value);