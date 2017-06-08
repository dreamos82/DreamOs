/// @file   scheduler.h
/// @brief  Scheduler structures and functions.
/// @author Dario Casalinuovo
/// @date   Oct 27 2003
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

#include "process.h"
#include "stdint.h"
#include "list.h"

/// Defines the maximum number of processes.
#define MAX_PROCESSES 9

/// @brief Linked list of processes.
typedef struct process_list_t
{
    /// Pointer to a process.
    process_t * process;
    /// @brief Pointer to the next element of the list.
    struct process_list_t * next;
} process_list_t;

/// Contains all the running processs.
extern list_t * process_list;

/// @brief Initialize the scheduler.
void kernel_initialize_scheduler();

/// @brief Activate the given process.
void kernel_activate_process(process_t * process);

/// @brief Removes the given process from the scheduler.
void kernel_deactivate_process(process_t * process);

/// @brief Returns the pointer to the current active process.
process_t * kernel_get_current_process();

/// @brief Main function of the scheduler.
void schedule();

/// @brief Switch the current process with the next process.
/// @param current  The current process.
/// @param next     The next process.
void switch_process(process_list_t ** current, process_list_t * next);

/// @brief Returns the number of active processes.
size_t get_active_process();
