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

#include "vm.h"
#include "paging.h"
#include "idt.h"
#include "handlers.h"
#include "panic.h"
#include <string.h>
#include <stdio.h>

uint32_t *page_directory = (uint32_t *)PAGE_DIR_VIRTUAL_ADDR;
uint32_t *page_tables = (uint32_t *)PAGE_TABLE_VIRTUAL_ADDR;

page_directory_t *current_directory;

extern char kernel_paging_active;

#define TABLE_SIZE 0x1000

uint32_t fault = 0;


void init_area(void* area)
{
	memset(area, 0, TABLE_SIZE);
}

void kernel_init_vm ()
{
  uint32_t cr0;

  // Register the page fault handler.
  kernel_add_interrupt_function_table(14, &page_fault_handler);
  // Create a page directory.
  page_directory_t *pd = (page_directory_t*)kernel_alloc_page ();

  // Initialise it.
  init_area(pd);

  // Identity map the first 4 MB.
  pd[0] = kernel_alloc_page () | PAGE_PRESENT | PAGE_WRITE;

  uint32_t *pt = (uint32_t*) (pd[0] & PAGE_MASK);
  for (int i = 0; i < 1024; i++)
    pt[i] = i*TABLE_SIZE | PAGE_PRESENT | PAGE_WRITE;

  // Assign the second-last table and zero it.
  pd[1022] = kernel_alloc_page () | PAGE_PRESENT | PAGE_WRITE;
  pt = (uint32_t*) (pd[1022] & PAGE_MASK);
  init_area(pt);

  // The last entry of the second-last table is the directory itself.
  pt[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;

  // The last table loops back on the directory itself.
  pd[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;

  // Set the current directory.
  kernel_switch_page_directory(pd);

  // Enable paging.
  asm volatile ("mov %%cr0, %0" : "=r" (cr0));
  cr0 |= 0x80000000;
  asm volatile ("mov %0, %%cr0" : : "r" (cr0));

  // We need to map the page table where the physical memory manager keeps its page stack
  // else it will panic on the first "kernel_free_page".
  uint32_t pt_idx = PAGE_DIR_IDX((PAGING_STACK_ADDR>>12));
  page_directory[pt_idx] = kernel_alloc_page () | PAGE_PRESENT | PAGE_WRITE;

  init_area(&page_tables[pt_idx*1024]);

  // Paging is now active. Tell the physical memory manager.
  kernel_paging_active = 1;

}

void kernel_switch_page_directory (page_directory_t *pd)
{
  current_directory = pd;
  asm volatile ("mov %0, %%cr3" : : "r" (pd));
}

void map (uint32_t va, uint32_t pa, uint32_t flags)
{
  uint32_t virtual_page = va / TABLE_SIZE;
  uint32_t pt_idx = PAGE_DIR_IDX(virtual_page);

  // Find the appropriate page table for 'va'.
  if (page_directory[pt_idx] == 0) {
    // The page table holding this page has not been created yet.
    page_directory[pt_idx] = kernel_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
    init_area(&page_tables[pt_idx*1024]);
  }

  // Now that the page table definately exists, we can update the PTE.
  page_tables[virtual_page] = (pa & PAGE_MASK) | flags;
}

void unmap (uint32_t va)
{
  uint32_t virtual_page = va / TABLE_SIZE;

  page_tables[virtual_page] = 0;
  // Inform the CPU that we have invalidated a page mapping.
  asm volatile ("invlpg (%0)" : : "a" (va));
}

char get_mapping (uint32_t va, uint32_t *pa)
{
  uint32_t virtual_page = va / TABLE_SIZE;
  uint32_t pt_idx = PAGE_DIR_IDX(virtual_page);

  // Find the appropriate page table for 'va'.
  if (page_directory[pt_idx] == 0)
    return 0;

  if (page_tables[virtual_page] != 0) {
    if (pa)
      *pa = page_tables[virtual_page] & PAGE_MASK;

    return 1;
  }
  return 0;
}

void page_fault_handler (int ecode)
{
  // TODO: The system launch this fault continuously and prevent
  // to read the debug info, this should be investigated.
  if (fault == 0)
	fault = 1;
  else
	return;

  uint32_t cr2;
  asm volatile ("mov %%cr2, %0" : "=r" (cr2));

  char message[128];

  // TODO: Support registers in handlers
  sprintf(message,
	"Detected page fault at address 0x%x", cr2);
 
  kernel_panic(message);
  for (;;) ; 
}
