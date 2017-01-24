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

#ifndef THREAD_H
#define THREAD_H

#include "support_defs.h"

typedef struct
{
  uint32_t esp, ebp, ebx, esi, edi, eflags;
  uint32_t id;                  // Thread ID.
  char *name;
} thread_t;

typedef struct thread_list
{
  thread_t *thread;
  struct thread_list *next;
} thread_list_t;

thread_t* kernel_init_threading();

// If stack is 0 a default one is created
thread_t* kernel_create_thread(int (*fn)(void*), void* arg, uint32_t* stack);

void switch_thread(thread_list_t* next);

#endif
