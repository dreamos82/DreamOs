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

#include <thread.h>
#include <kheap.h>
#include <scheduler.h>
#include <string.h>
#include <debug.h>

#define DEFAULT_STACK_SIZE 0x100000

/// @brief Returns a non-decreasing unique thread id.
/// @return A unique thread id.
uint32_t thread_get_id(void)
{
    static unsigned long int tid = 0;
    return ++tid;
}

void thread_exit();

extern void _create_thread(int (*)(void *), void *, uint32_t *, thread_t *);

thread_t * kernel_create_thread(int (* fn)(void *),
                                char * name,
                                void * arg,
                                uint32_t * stack)
{
    if (get_active_threads() + 1 > MAX_THREADS)
    {
        return NULL;
    }
    // Most threads don't want to deal with stack size.
    if (stack == 0)
    {
        stack = (uint32_t *) (((char *) kmalloc(DEFAULT_STACK_SIZE))
                              + DEFAULT_STACK_SIZE
                              - sizeof(uint32_t) * 3);
    }
    // Create the thread.
    thread_t * thread = kmalloc(sizeof(thread_t));
    memset(thread, 0, sizeof(thread_t));

    *(--stack) = (uint32_t) arg;
    *(--stack) = (uint32_t) &thread_exit;
    *(--stack) = (uint32_t) fn;

    // Set the top address of the stack.
    thread->regs.esp = (uint32_t) stack;
    // Set the base address of the stack.
    thread->regs.ebp = 0;
    // Enable the interrupts.
    thread->regs.eflags = EFLAG_IF;
    // Set the exit status to 0.
    thread->exit = 0;
    // Set the id of the thread.
    thread->id = thread_get_id();
    // Set the name of the thread.
    memset(thread->name, '\0', 50);
    strcpy(thread->name, name);
    dbg_print("\nRunning thread %s with id %d and flags %d\n",
              thread->name,
              thread->id,
              thread->regs.eflags);
    // Activate the thread.
    kernel_activate_thread(thread);
    return thread;
}

void thread_exit()
{
    // Get the exit value of the thread.
    uint32_t exit_value = 0;
    __asm__("movl %%eax, %0" : "=r" (exit_value));
    thread_t * current = kernel_get_current_thread();
    dbg_print("\nThread %d exited with value %d, flags %d\n",
              current->id,
              exit_value,
              current->regs.eflags);
    current->exit = 1;
    while (TRUE);
}
