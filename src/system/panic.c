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

#include <panic.h>

#include <elf.h>
#include <stdio.h>

#include "support_defs.h"

static void print_stack_trace ();

extern elf_t kernel_elf;

void kernel_panic(const char* msg)
{
  printf ("\nPANIC: %s\n\nWelcome to Kernel Debugging Land...\n\n", msg);
  print_stack_trace ();
  printf ("\n");
  for (;;);
}

void print_stack_trace()
{
  uint32_t *ebp, *eip;
  uint32_t count = 0;

  asm volatile ("mov %%ebp, %0" : "=r" (ebp));

  while (ebp) {
    if (count > 15)
		return;

    eip = ebp+1;
    printf (" %d 0x%x %s\n", count, *eip,
		elf_lookup_symbol (*eip, &kernel_elf));
    ebp = (uint32_t*) *ebp;
    count++;
  }
}
