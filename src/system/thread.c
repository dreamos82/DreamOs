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

uint32_t next_tid = 0;

void thread_exit ();

extern void _create_thread(int (*)(void*), void*, uint32_t*, thread_t*);

#define DEFAULT_STACK_SIZE 0x100000


thread_t *kernel_init_threading ()
{
  thread_t *thread = kmalloc (sizeof (thread_t));
  thread->id  = next_tid++;

  return thread;
}

thread_t *kernel_create_thread (int (*fn)(void*), void *arg, uint32_t *stack)
{
  // Most threads don't want to deal with stack size
  if (stack == 0) {
	stack = kmalloc (DEFAULT_STACK_SIZE)
		+ (DEFAULT_STACK_SIZE - sizeof(uint32_t)*3);
  }

  thread_t *thread = kmalloc (sizeof (thread_t));
  memset (thread, 0, sizeof (thread_t));
  thread->id = next_tid++;
  
  *--stack = (uint32_t)arg;
  *--stack = (uint32_t)&thread_exit;
  *--stack = (uint32_t)fn;

  thread->esp = (uint32_t)stack;
  thread->ebp = 0;
  thread->eflags = 0x200; // Interrupts enabled.
  thread->exit = 0;
  kernel_activate_thread(thread);

  return thread;
}

void thread_exit()
{
  uint32_t exit_value = 0;
  asm("movl %%eax, %0" : "=r" (exit_value));

  thread_t* current = kernel_get_current_thread();

  printf ("Thread %d exited with value %d\n", current->id, exit_value);

  current->exit = 1;

  for (;;) ;
}

#ifdef E_NEWSCHED
void switch_thread (struct thread_list *next)

{
      //asm ("mov %%eax, %0" : : "r" (current_thread));

      asm ("mov %%esp, %0" : "=r" (current_thread->esp));
      asm ("mov %%ebp, %0" : "=r" (current_thread->ebp));
      asm ("mov %%ebx, %0" : "=r" (current_thread->ebx));
      asm ("mov %%esi, %0" : "=r" (current_thread->esi));
      asm ("mov %%edi, %0" : "=r" (current_thread->edi));

      //asm ("pushf");
      //asm ("pop %ecx");

      //[eax+20], ecx
      //asm ("mov %%ecx, %0" : "=r" (current_thread->ecx));

      current_thread = next->thread;

      //asm ("mov %0, %%eax" : : "r" (current_thread));

      asm ("mov %0, %%esp" : : "r" (next->thread->esp));
      asm ("mov %0, %%ebp" : : "r" (next->thread->ebp));
      asm ("mov %0, %%ebx" : : "r" (next->thread->ebx));
      asm ("mov %0, %%edi" : : "r" (next->thread->edi));
      asm ("mov %0, %%esi" : : "r" (next->thread->esi));

      //mov eax, [eax+20]
      //asm ("mov %0, %%eax" : : "r" (next->thread->ecx));

      //asm ("push %eax");
      //asm ("popf");
}
#endif
