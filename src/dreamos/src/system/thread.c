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

#include "thread.h"
#include "kheap.h"
#include "scheduler.h"
#include <stdio.h>
#include <string.h>

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

thread_t * kernel_init_threading()
{
    thread_t * thread = kmalloc(sizeof(thread_t));
    thread->id = thread_get_id();
    return thread;
}

thread_t * kernel_create_thread(int (* fn)(void *),
                                void * arg,
                                uint32_t * stack)
{
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
    thread->id = thread_get_id();

    printf("Running thread with id %d\n", thread->id);

    *(--stack) = (uint32_t) arg;
    *(--stack) = (uint32_t) &thread_exit;
    *(--stack) = (uint32_t) fn;

    // Set the top address of the stack.
    thread->esp = (uint32_t) stack;
    // Set the base address of the stack.
    thread->ebp = 0;
    // Enable the interrupts.
    thread->eflags = EFLAG_IF;
    // Set the exit status to 0.
    thread->exit = 0;
    thread->exitFunction = &thread_exit;
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

    printf("\nThread %d exited with value %d\n", current->id, exit_value);

    current->exit = 1;

    while (TRUE);
}

void switch_thread_v2(struct thread_list * next)
{
    thread_t * current_thread = kernel_get_current_thread();

    __asm__("mov %0, %%esp" : "=r" (current_thread->esp));
    __asm__("mov %0, %%ebp" : "=r" (current_thread->ebp));
    __asm__("mov %0, %%ebx" : "=r" (current_thread->ebx));
    __asm__("mov %0, %%esi" : "=r" (current_thread->esi));
    __asm__("mov %0, %%edi" : "=r" (current_thread->edi));
    __asm__("pushf");
    __asm__("pop %0" : "=r" (current_thread->eflags));

    // Handle the next function?
    __asm__("mov %%eax, %0" : "=r" (next));
    __asm__("mov %%esp, %0" : "=r" (next->thread->esp));
    __asm__("mov %%ebp, %0" : "=r" (next->thread->ebp));
    __asm__("mov %%ebx, %0" : "=r" (next->thread->ebx));
    __asm__("mov %%esi, %0" : "=r" (next->thread->esi));
    __asm__("mov %%edi, %0" : "=r" (next->thread->edi));
    __asm__("push %0" : "=r" (next->thread->eflags));
    __asm__("popf");

    __asm__("ret");
}
