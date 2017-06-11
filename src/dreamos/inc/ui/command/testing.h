/// @file   testing.h
/// @brief  Commands used to test OS functionalities.
/// @author Lisa
/// @date   Feb 2006
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

/// @brief Tries the string tokenizer.
void try_strtok();

/// @brief Tries the malloc.
void try_kmalloc();

/// @brief Prints the memory.
void try_printmem();

/// @brief Tries the fault handling.
void do_fault();

/// @brief Shows the list of files.
void try_module();

/// @brief Try to open a file.
void try_open();

/// @brief Try to create a file.
void try_ocreat();

/// @brief Try the system calls.
void try_syscall();

/// @brief Shows the filesystem.
void show_fd();

/// @brief Tries the stat function.
void test_stat();

/// @brief Tries the shadow option of the keyboard.
void try_shadow();

/// @brief Tries to map addresses.
void try_mapaddress();

/// @brief Tries the lock features.
void try_process();

/// @brief Creates some background processes.
void try_process_sleep();

/// @brief Tries the queue.
void try_queue();

/// @brief Stresses the heap.
void try_stress_heap();
