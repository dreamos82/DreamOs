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

#include "scheduler.h"
#include "kheap.h"

thread_list_t *ready_queue = 0;
thread_list_t *current_thread = 0;

void kernel_init_scheduler (thread_t *initial_thread)
{
  current_thread = (thread_list_t*) kmalloc (sizeof (thread_list_t));
  current_thread->thread = initial_thread;
  current_thread->next = 0;
  ready_queue = 0;
}

void kernel_activate_thread(thread_t *t)
{
  // Create a new list item for the new thread.
  thread_list_t *item = (thread_list_t*) kmalloc (sizeof (thread_list_t));
  item->thread = t;
  item->next = 0;

  if (!ready_queue) {
    // Special case if the ready queue is empty.
    ready_queue = item;
  } else {
    // Iterate through the ready queue to the end.
    thread_list_t *iterator = ready_queue;
    while (iterator->next)
      iterator = iterator->next;

    // Add the item.
    iterator->next = item;
  }
}

void kernel_deactivate_thread(thread_t *t)
{
  // Attempt to find the thread in the ready queue.
  thread_list_t *iterator = ready_queue;

  // Special case if the thread is first in the queue.
  if (iterator->thread == t) {
    ready_queue = iterator->next;
    free (iterator);
    return;
  }

  while (iterator->next) {
    if (iterator->next->thread == t) {
      thread_list_t *tmp = iterator->next;
      iterator->next = tmp->next;
      free (tmp);
    }
    iterator = iterator->next;
  }
}

thread_t*
kernel_get_current_thread()
{
	return current_thread->thread;
}

void schedule ()
{
  if (!ready_queue)
	return;

  while (ready_queue->thread->exit == 1) {
	 thread_list_t* next = ready_queue->next;
     kernel_deactivate_thread(ready_queue->thread);
     ready_queue = next;
  }

  // Iterate through the ready queue to the end.
  thread_list_t *iterator = ready_queue;
  while (iterator->next)
    iterator = iterator->next;

  // Add the old thread to the end of the queue, and remove it from the start.
  iterator->next = current_thread;
  current_thread->next = 0;
  thread_list_t *new_thread = ready_queue;

  ready_queue = ready_queue->next;

  // Switch to the new thread.
  switch_thread (new_thread);
}
