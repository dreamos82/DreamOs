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

#include <scheduler.h>
#include <string.h>
#include <kheap.h>
#include <debug.h>

process_list_t * ready_queue = NULL;
process_list_t * current_process = NULL;
list_t * process_list;

void kernel_initialize_scheduler()
{
    // Create the scheduler process.
    process_t * scheduler = kmalloc(sizeof(process_t));
    scheduler->id = 0;
    strcpy(scheduler->name, "Scheduler");
    // Create a new list item for the new process.
    current_process = (process_list_t *) kmalloc(sizeof(process_list_t));
    current_process->process = scheduler;
    current_process->next = NULL;
    // Initialize the list of processs.
    process_list = list_create();
    // Add the process to the list of processs.
//    scheduler->self = list_insert_front(process_list, scheduler);
}

void kernel_activate_process(process_t * process)
{
    // Create a new list item for the new process.
    process_list_t * item = (process_list_t *) kmalloc(sizeof(process_list_t));
    item->process = process;
    item->next = NULL;
    if (!ready_queue)
    {
        // Special case if the ready queue is empty.
        ready_queue = item;
    }
    else
    {
        // Iterate through the ready queue to the end.
        process_list_t * iterator = ready_queue;
        while (iterator->next)
        {
            iterator = iterator->next;
        }
        // Add the item.
        iterator->next = item;
    }
    // Add the process to the list of processs.
    process->self = list_insert_front(process_list, process);
}

void kernel_deactivate_process(process_t * process)
{
    // Remove the process from the list of processs.
    list_remove_node(process_list, process->self);

    // Attempt to find the process in the ready queue.
    process_list_t * iterator = ready_queue;

    // Special case if the process is first in the queue.
    if (iterator->process == process)
    {
        ready_queue = iterator->next;
        kfree(iterator);
        return;
    }

    while (iterator->next)
    {
        if (iterator->next->process == process)
        {
            process_list_t * tmp = iterator->next;
            iterator->next = tmp->next;
            kfree(tmp);
        }
        iterator = iterator->next;
    }
}

process_t * kernel_get_current_process()
{
    return current_process->process;
}

void schedule()
{
    // The ready queue is empty.
    if (!ready_queue)
    {
        //dbg_print("The ready queue is empty.\n");
        return;
    }
    // Search the first process which has completed.
    while (ready_queue->process->exit == 1)
    {
        dbg_print("Deactivating process '%s'.\n", ready_queue->process->name);
        // Store the next process pointed by the completed process.
        process_list_t * next = ready_queue->next;
        // Deactivate the completed process.
        kernel_deactivate_process(ready_queue->process);
        // Set the ready queue to the stored process.
        ready_queue = next;
    }
    // Iterate through the ready queue to the end.
    process_list_t * iterator = ready_queue;
    while (iterator->next)
    {
        iterator = iterator->next;
    }
    // Add the old process to the end of the queue.
    iterator->next = current_process;
    current_process->next = 0;
    // --------------------------------
    // Remove the moved process from the start.
    process_list_t * new_process = ready_queue;
    ready_queue = ready_queue->next;
    // --------------------------------
    // Switch to the new process.
    switch_process(&current_process, new_process);
}

size_t get_active_processs()
{
    return list_size(process_list);
}