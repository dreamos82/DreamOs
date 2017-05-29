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

#pragma once

#include "multiboot.h"
#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"

#define PAGING_STACK_ADDR 0xFF000000

// Error code interpretation.
#define ERR_PRESENT     0x1
#define ERR_RW          0x2
#define ERR_USER        0x4
#define ERR_RESERVED    0x8
#define ERR_INST        0x10

#define SUPERVISOR 0

extern bool_t paging_enabled;

void kernel_init_paging(uint32_t start);

uint32_t kernel_alloc_page();

void kernel_free_page(uint32_t p);

void kernel_map_memory(struct multiboot_info * info);

void page_fault_handler(register_t * reg);
