/*
 * Shell.h
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

void try_strtok();

void try_kmalloc();

void try_printmem(void);

void do_fault();

void try_module();

void try_open();

void try_ocreat();

void try_syscall();

void show_fd();

void test_stat();

void try_shadow();

//void try_newheap();
void try_mapaddress();

void try_process();

void try_process_sleep();

void try_queue();
