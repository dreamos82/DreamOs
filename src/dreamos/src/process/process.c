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

#include "process.h"
#include "kheap.h"
#include "scheduler.h"
#include "string.h"
#include "debug.h"

#define DEFAULT_STACK_SIZE 0x100000

/// @brief Returns a non-decreasing unique process id.
/// @return A unique process id.
uint32_t process_get_id(void)
{
    static unsigned long int tid = 0;
    return ++tid;
}

/// @brief Executed when a process terminates.
void process_exit()
{
    // Get the exit value of the process.
    uint32_t exit_value = 0;
    __asm__("movl %%eax, %0" : "=r" (exit_value));
    process_t * current = kernel_get_current_process();
    dbg_print("\nProcess %d exited with value %d, flags %d\n",
              current->id,
              exit_value,
              current->regs.eflags);
    current->exit = 1;
    // Free the space occupied by the stack.
    kfree((uint32_t *) current->regs.ebp);
    // Free the space occupied by the structure of the process.
    kfree(current);
    while (TRUE);
}

process_t * kernel_create_process(int (* function)(void *),
                                  char * name,
                                  void * arg,
                                  uint32_t * stack)
{
    // Checks if by activating a new process we exceed the number of maximum
    // active processes
    if (get_active_process() + 1 > MAX_PROCESSES)
    {
        return NULL;
    }
    // If a pre-existing stack has not been provided, create a new one.
    uint32_t * stack_base_address = stack;
    if (stack == NULL)
    {
        stack_base_address = kmalloc(DEFAULT_STACK_SIZE);
        stack = (uint32_t *) ((char *) stack_base_address
                              + DEFAULT_STACK_SIZE);
    }
    // Store inside the stack the arguments of the function, the function
    // called when the process terminates and the function itself.
    *(--stack) = (uint32_t) arg;
    *(--stack) = (uint32_t) &process_exit;
    *(--stack) = (uint32_t) function;
    // Create the process.
    process_t * process = kmalloc(sizeof(process_t));
    memset(process, 0, sizeof(process_t));
    // Set the top address of the stack.
    process->regs.esp = (uint32_t) stack;
    // Set the base address of the stack.
    process->regs.ebp = (uint32_t) stack_base_address;
    // Enable the interrupts.
    process->regs.eflags = EFLAG_IF;
    // Set the exit status to 0.
    process->exit = 0;
    // Set the id of the process.
    process->id = process_get_id();
    // Set the name of the process.
    memset(process->name, '\0', 50);
    strcpy(process->name, name);
    dbg_print("\nRunning process %s with id %d and flags %d\n",
              process->name,
              process->id,
              process->regs.eflags);
    // Activate the process.
    kernel_activate_process(process);
    return process;
}
