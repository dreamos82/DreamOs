/// @file   paging.h
/// @brief  Functions used to manage paging.
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

#include "multiboot.h"
#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"

/// @brief Initial address of the stack.
#define PAGING_STACK_ADDR 0xFF000000

// Error code interpretation.
#define ERR_PRESENT     0x1     ///< Page present.
#define ERR_RW          0x2     ///< Page is read only.
#define ERR_USER        0x4     ///< Page is read only.
#define ERR_RESERVED    0x8     ///< Overwrote reserved bits.
#define ERR_INST        0x10    ///< Instruction fetch.

void kernel_init_paging(uint32_t start);

/// @brief Enable paging, turn off PSE bit first as it was turned on by the
/// assembly header when kernel was loading. Then enable PG Bit in cr0.
void kernel_enable_paging();

void kernel_activate_paging();

uint32_t kernel_alloc_page();

void kernel_free_page(uint32_t p);

void kernel_map_memory(multiboot_info_t * info);

void page_fault_handler(register_t * reg);
