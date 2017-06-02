/// @file   paging.c
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

#include "paging.h"
#include "vm.h"
#include "panic.h"
#include "debug.h"
#include "video.h"
#include "irqflags.h"

// The top address of the stack.
uint32_t stack_max = PAGING_STACK_ADDR;
// The current position on the stack.
uint32_t stack_loc = PAGING_STACK_ADDR;
// The location where data is stored when paging is not enabled.
uint32_t location;
// Flag used to determine if paging is enabled.
bool_t paging_enabled = false;

void kernel_init_paging(uint32_t start)
{
    // Ensure the initial page allocation location is page-aligned.
    location = (start + PAGE_SIZE) & PAGE_MASK;
}

uint32_t kernel_alloc_page()
{
    if (!paging_enabled)
    {
        return location += PAGE_SIZE;
    }
    // Quick sanity check.
    if (stack_loc == PAGING_STACK_ADDR)
    {
        kernel_panic("Error:out of memory.");
    }
    // Now we are going to pop off the stack. First we have to move the entry
    // point on the stack.
    stack_loc -= sizeof(uint32_t);
    // Then we get the 32bit pointer to that stack location.
    uint32_t * stack_ptr = (uint32_t *) stack_loc;
    return (*stack_ptr);
}

void kernel_free_page(uint32_t p)
{
    // Ignore any page under "location", as it may contain important data initialised
    // at boot (like paging structures!)
    if (p < location) return;

    // If we've run out of space on the stack...
    if (stack_max <= stack_loc)
    {
        // Map the page we're currently freeing at the top of the free page stack.
        map(stack_max, p, PAGE_PRESENT | PAGE_WRITE);
        // Increase the free page stack's size by one page.
        stack_max += 4096;
    }
    else
    {
        // Else we have space on the stack, so push.
        uint32_t * stack_ptr = (uint32_t *) stack_loc;
        // Save on that position of the stack the given pointer.
        (*stack_ptr) = p;
        // Increase the stack location.
        stack_loc += sizeof(uint32_t);
    }
}

void kernel_map_memory(multiboot_info_t * info)
{
    // Find all the usable areas of memory and inform the physical memory manager about them.
    uint32_t i = info->mmap_addr;
    while (i < info->mmap_addr + info->mmap_length)
    {
        memory_map_t * me = (memory_map_t *) i;

        // Does this entry specify usable RAM?
        if (me->type == 1)
        {
            uint32_t j;
            // For every page in this entry, add to the free page stack.
            for (j = me->base_addr_low;
                 j < me->base_addr_low + me->length_low; j += PAGE_SIZE)
            {
                kernel_free_page(j);
            }
        }

        // The multiboot specification is strange in this respect
        // - the size member does not inc "size" itself in its calculations,
        // so we must add sizeof (uint32_t).
        i += me->size + sizeof(uint32_t);
    }
}

void page_fault_handler(register_t * reg)
{
    static bool_t throw_once = false;
    if (!throw_once)
    {
        throw_once = true;
    }
    else
    {
        return;
    }

    // Disable the IRQs.
    irq_disable();
    video_set_color(BRIGHT_RED);
    dbg_print("Page fault:\n");

    // Gather fault info and print to screen
    uint32_t faulting_addr;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_addr));
    uint32_t present = reg->err_code & ERR_PRESENT;
    uint32_t rw = reg->err_code & ERR_RW;
    uint32_t user = reg->err_code & ERR_USER;
    uint32_t reserved = reg->err_code & ERR_RESERVED;
    uint32_t inst_fetch = reg->err_code & ERR_INST;

    dbg_print("Possible causes: [ ");
    if (!present) dbg_print("Page not present ");
    if (rw) dbg_print("Page is read only ");
    if (user) dbg_print("Page is read only ");
    if (reserved) dbg_print("Overwrote reserved bits ");
    if (inst_fetch) dbg_print("Instruction fetch ");
    dbg_print("]\n");

    print_reg(reg);
    while (true);
}
