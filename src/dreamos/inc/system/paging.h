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

#ifndef PAGING_H
#define PAGING_H

#include <multiboot.h>

#include "support_defs.h"

#define PAGING_STACK_ADDR 0xFF000000

#define SUPERVISOR 0

void kernel_init_paging(uint32_t start);

uint32_t kernel_alloc_page();

void kernel_free_page(uint32_t p);

void kernel_map_memory(struct multiboot_info * info);

#endif
