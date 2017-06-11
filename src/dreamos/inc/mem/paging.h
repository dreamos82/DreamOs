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

/// @brief Initialize paging.
/// @param start Used to determine the initial page allocation location.
void kernel_init_paging(uint32_t start);

/// @brief Initialize the memory with free pages.
/// @param info Information retrieve during boot.
void kernel_map_memory(multiboot_info_t * info);

/// @brief Enable paging, turn off PSE bit first as it was turned on by the
/// assembly header when kernel was loading. Then enable PG Bit in cr0.
void kernel_enable_paging();

/// @brief Allows to activate paging.
void kernel_activate_paging();

/// @brief Allows to allocate a page.
/// @return A 32bit address to the allocated stack location.
uint32_t kernel_alloc_page();

/// @brief Frees the given page.
/// @param page The address of the page that has to be freed.
void kernel_free_page(uint32_t page);

/// @brief Fault handling function, called when a fault occurs.
/// @param reg Pointer to the registers.
void page_fault_handler(register_t * reg);
