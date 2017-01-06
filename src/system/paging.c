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

#include <paging.h>

#include <vm.h>

#include <stddef.h>
#include "panic.h"

uint32_t stack_loc = PAGING_STACK_ADDR;
uint32_t stack_max = PAGING_STACK_ADDR;
uint32_t location;
char kernel_paging_active = 0;

void kernel_init_paging (uint32_t start)
{
  // Ensure the initial page allocation location is page-aligned.
  location = (start + 0x1000) & PAGE_MASK;
}

uint32_t kernel_alloc_page ()
{
	if (!kernel_paging_active)
		return location += 0x1000;

    // Quick sanity check.
    if (stack_loc == PAGING_STACK_ADDR)
      kernel_panic("Error:out of memory.");

    // Pop off the stack.
    stack_loc -= sizeof (uint32_t);
    uint32_t *stack = (uint32_t*)stack_loc;
  
    return *stack;
}

void kernel_free_page (uint32_t p)
{
  // Ignore any page under "location", as it may contain important data initialised
  // at boot (like paging structures!)
  if (p < location) return;

  // If we've run out of space on the stack...
  if (stack_max <= stack_loc) {
    // Map the page we're currently freeing at the top of the free page stack.
    map (stack_max, p, PAGE_PRESENT | PAGE_WRITE);
    // Increase the free page stack's size by one page.
    stack_max += 4096;
  } else {
    // Else we have space on the stack, so push.
    uint32_t *stack = (uint32_t*)stack_loc;
    *stack = p;
    stack_loc += sizeof (uint32_t);
  }
}

void kernel_map_memory(struct multiboot_info* info)
{
  // Find all the usable areas of memory and inform the physical memory manager about them.
  uint32_t i = info->mmap_addr;
  while (i < info->mmap_addr + info->mmap_length) {
    memory_map_t *me = (memory_map_t*) i;

    // Does this entry specify usable RAM?
    if (me->type == 1) {
      uint32_t j;
      // For every page in this entry, add to the free page stack.
      for (j = me->base_addr_low; j < me->base_addr_low+me->length_low; j += 0x1000) {
        kernel_free_page (j);
      }
    }

    // The multiboot specification is strange in this respect
    // - the size member does not include "size" itself in its calculations,
    // so we must add sizeof (uint32_t).
    i += me->size + sizeof (uint32_t);
  }
}
