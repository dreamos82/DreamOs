/// @file   bitops.h
/// @brief  Bitmasks functions.
/// @author Ivan Gualandri
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

#include "stdint.h"
#include "stdbool.h"

int find_first_bit(unsigned short int);

/// @brief Check if the passed value has the given flag set.
/// @param flags The value to check.
/// @param flag  The flag to search.
/// @return <b>True</b> if the value contain the flag, <br>
///         <b>False</b> otherwise.
bool_t has_flag(const uint32_t flags, const uint32_t flag);

/// @brief Set the passed flag to the value.
/// @param flags The destination value.
/// @param flag  The flag to set.
void set_flag(uint32_t * flags, const uint32_t flag);

/// @brief Clear the passed flag to the value.
/// @param flags The destination value.
/// @param flag  The flag to clear.
void clear_flag(uint32_t * flags, const uint32_t flag);
