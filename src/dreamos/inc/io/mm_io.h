/// @file   mm_io.h
/// @brief  Memory Mapped IO functions.
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

#include "stdint.h"

/// @brief Reads a 8-bit value from the given address.
uint8_t in_memb(uint32_t addr);

/// @brief Reads a 16-bit value from the given address.
uint16_t in_mems(uint32_t addr);

/// @brief Reads a 32-bit value from the given address.
uint32_t in_meml(uint32_t addr);

/// @brief Writes a 8-bit value at the given address.
void out_memb(uint32_t addr, uint8_t value);

/// @brief Writes a 16-bit value at the given address.
void out_mems(uint32_t addr, uint16_t value);

/// @brief Writes a 32-bit value at the given address.
void out_meml(uint32_t addr, uint32_t value);