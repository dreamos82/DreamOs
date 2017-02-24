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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread.h>
#include <stdint.h>

typedef struct thread_list
{
    thread_t * thread;
    struct thread_list * next;
} thread_list_t;

extern thread_list_t * ready_queue;

void kernel_init_scheduler(thread_t* initial_thread);

void kernel_activate_thread(thread_t* thread);

void kernel_deactivate_thread(thread_t* thread);

thread_t* kernel_get_current_thread();

void schedule();

#endif
