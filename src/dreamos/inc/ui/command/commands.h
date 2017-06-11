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

/// The maximum number of tests.
#define MAX_TEST 15

/// @brief Prints the logo.
void cmd_logo(int argc, char ** argv);

/// @brief Returns the list of commands.
void cmd_help(int argc, char ** argv);

/// @brief Echos the given message.
void cmd_echo(int argc, char ** argv);

/// @brief Power off the machine.
void cmd_poweroff(int argc, char ** argv);

/// @brief Call the uname function.
void cmd_uname(int argc, char ** argv);

/// @brief Prints the credits.
void cmd_credits(int argc, char ** argv);

/// @brief Sleeps the thread of the shell for a given period of time.
void cmd_sleep(int argc, char ** argv);

/// @brief Shows the output of the cpuid function.
void cmd_cpuid(int argc, char ** argv);

/// @brief Lists the directory.
void cmd_ls(int argc, char ** argv);

/// @brief Move to another directory.
void cmd_cd(int argc, char ** argv);

/// @brief Show the current user name.
void cmd_whoami(int argc, char ** argv);

/// @brief Allows to test some functionalities of the kernel.
void cmd_tester(int argc, char ** argv);

/// @brief Print current working directory.
void cmd_pwd(int argc, char ** argv);

/// @brief Read content of a file.
void cmd_more(int argc, char ** argv);

/// @brief Create a new file.
void cmd_newfile(int argc, char ** argv);

/// @brief Show task list.
void cmd_ps(int argc, char ** argv);

/// @brief Show date and time.
void cmd_date(int argc, char ** argv);

/// @brief Clears the screen.
void cmd_clear(int argc, char ** argv);

/// @brief Shows the PID of the shell.
void cmd_showpid(int argc, char ** argv);

/// @brief Shows the heap status.
void cmd_heapdump(int argc, char ** argv);

/// @brief Prints the history.
void cmd_show_history(int argc, char ** argv);

/// @brief Loads the drivers.
void drv_load(int argc, char ** argv);

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
