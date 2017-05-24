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

#pragma once

#include "shell.h"

#define MAX_TEST 15

void aalogo(int argc, char ** argv);

void logo(int argc, char ** argv);

void help(int argc, char ** argv);

void echo(int argc, char ** argv);

void poweroff(int argc, char ** argv);

void kmalloc_try(int argc, char ** argv);

void uname_cmd(int argc, char ** argv);

void printmem(int argc, char ** argv);

void credits(int argc, char ** argv);

void sleep_cmd(int argc, char ** argv);

void cpuid(int argc, char ** argv);

void drv_load(int argc, char ** argv);

void ls(int argc, char ** argv);

void cd(int argc, char ** argv);

void whoami(int argc, char ** argv);

void tester(int argc, char ** argv);

void pwd(int argc, char ** argv);

void more(int argc, char ** argv);

void newfile(int argc, char ** argv);

void ps(int argc, char ** argv);

void date(int argc, char ** argv);

void clear(int argc, char ** argv);

void showpid(int argc, char ** argv);

void heapdump(int argc, char ** argv);

struct devel
{
    char cmd_testname[CMD_LEN];
    char cmd_description[DESC_LEN];
    void (* func)(void);
};
