/// @file   commands.h
/// @brief  Prototypes of functions for the Shell.
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

#include "shell.h"

#define MAX_TEST 15

void print_logo();

void aalogo(int argc, char ** argv);

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

/// @brief Define a fucntion used to test functionalities.
typedef struct development_function_t
{
    /// The name of the test.
    char cmd_testname[CMD_LEN];
    /// A description of the test.
    char cmd_description[DESC_LEN];
    /// A pointer to the function.
    void (* func)(void);
} development_function_t;
