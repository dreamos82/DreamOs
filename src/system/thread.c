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

thread_t *current_thread;
uint32_t next_tid = 0;

void thread_exit ();

extern void _create_thread(int (*)(void*), void*, uint32_t*, thread_t*);

#define DEFAULT_STACK_SIZE 0x100000


thread_t *kernel_init_threading ()
{
  thread_t *thread = kmalloc (sizeof (thread_t));
  thread->id  = next_tid++;
  
  current_thread = thread;

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
  kernel_activate_thread(thread);

  return thread;
}

void thread_exit()
{
  register uint32_t val asm ("eax");

  printf ("Thread exited with value %d\n", val);

  for (;;) ;
}

//void switch_thread (struct thread_list *next)
//{
//	// TODO implement
//}
