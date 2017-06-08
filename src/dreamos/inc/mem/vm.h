/// @file   vm.h
/// @brief  Functions used to manage the virtual memory.
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

#include "stdint.h"

#define PAGE_SIZE 0x1000

#define PAGE_DIR_VIRTUAL_ADDR   0xFFBFF000
#define PAGE_TABLE_VIRTUAL_ADDR 0xFFC00000
#define PAGE_DIR_IDX(x) ((uint32_t)x/1024)
#define PAGE_TABLE_IDX(x) ((uint32_t)x%1024)

#define PAGE_PRESENT   0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4
#define PAGE_MASK      0xFFFFF000

// Paging register manipulation macro
#define SET_PGBIT(cr0)      (cr0 = cr0 | 0x80000000)
#define CLEAR_PSEBIT(cr4)   (cr4 = cr4 & 0xffffffef)

typedef uint32_t page_directory_t;

/// @brief Sets up the environment, page directories etc and enables paging.
void kernel_init_vm();

/// @brief Changes address space.
void kernel_switch_page_directory(page_directory_t * pd);

/// @brief Maps the physical address pa into the virtual space at address va,
/// using page protection flags "flags".
/// @param va       The virtual address.
/// @param pa       The physical address.
/// @param flags    The flags.
void map(uint32_t va, uint32_t pa, uint32_t flags);

/// @brief Removes one page of V->P mappings at virtual address "va".
void unmap(uint32_t virtual_address);

/// @brief If "*pa" is non-NULL, the physical address of the mapping is
/// placed in *pa.
/// @param virtual_address  The virtual address.
/// @param physical_address The physical address.
/// @return 1 if the given virtual address is mapped in the address space.
char get_mapping(uint32_t virtual_address, uint32_t * physical_address);
