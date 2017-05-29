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

#pragma once

#include "process.h"
#include "stdint.h"
#include "list.h"

#define MAX_THREADS 9

#define LLIST 0

typedef struct process_list
{
    process_t * process;
    struct process_list * next;
} process_list_t;

/// Contains all the running processs.
extern list_t * process_list;

void kernel_initialize_scheduler();

void kernel_activate_process(process_t * process);

void kernel_deactivate_process(process_t * process);

process_t * kernel_get_current_process();

void schedule();

///// @brief Switch the current process with the next process.
///// @details Is defined inside "process.s".
///// @param current  The current process.
///// @param next     The next process.
void switch_process(struct process_list ** current,
                    struct process_list * next);

size_t get_active_process();
