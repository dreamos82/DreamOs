/*
 * Copyright (c), Dario Casalinuovo
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

//
// Based on JamesM's kernel developement tutorials.
//

#ifndef VM_H
#define VM_H

#include <stdint.h>

#define PAGE_DIR_VIRTUAL_ADDR   0xFFBFF000
#define PAGE_TABLE_VIRTUAL_ADDR 0xFFC00000
#define PAGE_DIR_IDX(x) ((uint32_t)x/1024)
#define PAGE_TABLE_IDX(x) ((uint32_t)x%1024)

#define PAGE_PRESENT   0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4
#define PAGE_MASK      0xFFFFF000

typedef uint32_t page_directory_t;

// Sets up the environment, page directories etc and enables paging.
void kernel_init_vm();

// Changes address space.
void kernel_switch_page_directory(page_directory_t * pd);

// Maps the physical page "pa" into the virtual space at address "va", using 
// page protection flags "flags".
void map(uint32_t va, uint32_t pa, uint32_t flags);

// Removes one page of V->P mappings at virtual address "va".
void unmap(uint32_t va);

// Returns 1 if the given virtual address is mapped in the address space.
// If "*pa" is non-NULL, the physical address of the mapping is placed in *pa.
char get_mapping(uint32_t va, uint32_t * pa);

void page_fault_handler(int);

#endif
