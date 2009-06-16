/*
 * DreamOS
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *

 * Autrice: Lisa
 * Febbraio 2006
 * Prototipi di funzione e variabili globali per la shell
 */

#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <kheap.h>

int argc;
char **argv;
extern heap_t *kheap;

void aalogo();
void logo();
void help();
void echo();
void poweroff();
void kmalloc_try();
void do_fault();

void uname_cmd();
void uname_help();
void uname_info();
void printmem();
void credits();
void sleep_cmd();
void cpuid();
void answer();
void modprobe();
#endif
