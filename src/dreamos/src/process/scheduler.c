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

//thread_list_t * ready_queue = NULL;
//thread_list_t * current_thread = NULL;

list_t * thread_list;

thread_t * cthread = NULL;

void kernel_initialize_scheduler()
{
    // Initialize the list of threads.
    thread_list = list_create();
    // Create the scheduler thread.
    cthread = kmalloc(sizeof(thread_t));
    cthread->id = 0;
    strcpy(cthread->name, "Scheduler");
//    // Create a new list item for the new thread.
//    current_thread = (thread_list_t *) kmalloc(sizeof(thread_list_t));
//    current_thread->thread = cthread;
//    current_thread->next = NULL;

    // Add the thread to the list of threads.
    cthread->self = list_insert_front(thread_list, cthread);
}

void kernel_activate_thread(thread_t * thread)
{
//    // Create a new list item for the new thread.
//    thread_list_t * item = (thread_list_t *) kmalloc(sizeof(thread_list_t));
//    item->thread = thread;
//    item->next = NULL;
//    if (!ready_queue)
//    {
//        // Special case if the ready queue is empty.
//        ready_queue = item;
//    }
//    else
//    {
//        // Iterate through the ready queue to the end.
//        thread_list_t * iterator = ready_queue;
//        while (iterator->next)
//        {
//            iterator = iterator->next;
//        }
//        // Add the item.
//        iterator->next = item;
//    }
    // Add the thread to the list of threads.
    thread->self = list_insert_front(thread_list, thread);
}

void kernel_deactivate_thread(thread_t * thread)
{
//    // Attempt to find the thread in the ready queue.
//    thread_list_t * iterator = ready_queue;
//
//    // Special case if the thread is first in the queue.
//    if (iterator->thread == thread)
//    {
//        ready_queue = iterator->next;
//        kfree(iterator);
//        return;
//    }
//
//    while (iterator->next)
//    {
//        if (iterator->next->thread == thread)
//        {
//            thread_list_t * tmp = iterator->next;
//            iterator->next = tmp->next;
//            kfree(tmp);
//        }
//        iterator = iterator->next;
//    }
    // Remove the thread from the list of threads.
    list_remove_node(thread_list, thread->self);
}

thread_t * kernel_get_current_thread()
{
//    return current_thread->thread;
    return cthread;
}

void schedule()
{
    dbg_print("Scheduler.\n");
    // The ready queue is empty.
    if (!list_size(thread_list))
    {
        dbg_print("1 - The ready queue is empty.\n");
        return;
    }
//    if (!ready_queue)
//    {
//        dbg_print("2 - The ready queue is empty.\n");
//        return;
//    }
    size_t nprocs = list_size(thread_list);
    for (size_t i = 0; i < nprocs; ++i)
    {
        listnode_t * node = list_get_node_by_index(thread_list, i);
        if (node)
        {
            thread_t * thread = node->val;
            if (thread)
            {
                if (thread->exit == 1)
                {
                    dbg_print("Deactivating thread '%s'.\n", thread->name);
//                    // Store the next thread pointed by the completed thread.
//                    thread_list_t * next = ready_queue->next;
//                    // Deactivate the completed thread.
//                    kernel_deactivate_thread(ready_queue->thread);
//                    // Set the ready queue to the stored thread.
//                    ready_queue = next;
                }
            }
        }
    }
    // Search the first thread which has completed.
//    while (ready_queue->thread->exit == 1)
//    {
//        dbg_print("Deactivating thread '%s'.\n", ready_queue->thread->name);
//        // Store the next thread pointed by the completed thread.
//        thread_list_t * next = ready_queue->next;
//        // Deactivate the completed thread.
//        kernel_deactivate_thread(ready_queue->thread);
//        // Set the ready queue to the stored thread.
//        ready_queue = next;
//    }
    // Iterate through the ready queue to the end.
//    thread_list_t * iterator = ready_queue;
//    while (iterator->next)
//    {
//        iterator = iterator->next;
//    }
//    dbg_print("1 - Current thread '%s'.\n", current_thread->thread->name);
    dbg_print("2 - Current thread '%s'.\n", cthread->name);
    // Add the old thread to the end of the queue.
//    iterator->next = current_thread;
//    current_thread->next = 0;
    list_remove_node(thread_list, cthread->self);
    cthread->self = list_insert_back(thread_list, cthread);
    // --------------------------------
    // Remove the moved thread from the start.
//    thread_list_t * new_thread = ready_queue;
//    ready_queue = ready_queue->next;
    // --------------------------------
    thread_t * nthread = list_peek_front(thread_list);
//    dbg_print("1 - Next thread '%s'.\n", new_thread->thread->name);
    dbg_print("2 - Next thread '%s'.\n", nthread->name);
    // --------------------------------
    // Switch to the new thread.
//    switch_thread(&current_thread, new_thread);
    dbg_print("Switch to the new thread.\n");
    switch_thread(&cthread, nthread);
    dbg_print("Done.\n");
}
