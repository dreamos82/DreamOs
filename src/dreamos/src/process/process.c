/// @file   process.c
/// @brief  Process data structures and functions.
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


#include "process.h"
#include "kheap.h"
#include "scheduler.h"
#include "string.h"
#include "debug.h"

/// The default dimension of the stack of a process.
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
    // Set the state of the process to zombie.
    current->state = PROCESS_ZOMBIE;
    // Free the space occupied by the stack.
    kfree((uint32_t *) current->stack);
    // Free the space occupied by the structure of the process.
    kfree(current);
    // Process waits to be remove from the scheduler.
    while (TRUE);
}

process_t * kernel_create_process(int (* function)(void *),
                                  char * name,
                                  void * arg)
{
    // Checks if by activating a new process we exceed the number of maximum
    // active processes
    if (get_active_process() + 1 > MAX_PROCESSES)
    {
        return NULL;
    }
    // Create a new stack.
    uint32_t * stack = kmalloc(DEFAULT_STACK_SIZE);
    // Set the base address of the stack.
    uint32_t * ebp = (uint32_t *) ((char *) stack + DEFAULT_STACK_SIZE);
    // Create a pointer to keep track of the top of the stack.
    uint32_t * esp = ebp;
    // Store inside the stack the arguments of the function, the function
    // called when the process terminates and the function itself.
    *(--esp) = (uint32_t) arg;
    *(--esp) = (uint32_t) &process_exit;
    *(--esp) = (uint32_t) function;
    // Create the process.
    process_t * process = kmalloc(sizeof(process_t));
    memset(process, 0, sizeof(process_t));
    // Set the top address of the stack.
    process->regs.esp = (uint32_t) esp;
    // Set the base address of the stack.
    process->regs.ebp = (uint32_t) ebp;
    // Enable the interrupts.
    process->regs.eflags = EFLAG_IF;
    // Set the id of the process.
    process->id = process_get_id();
    // Store the beginning of the stack.
    process->stack = stack;
    // Set the state of the process as running.
    process->state = PROCESS_RUNNING;
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
