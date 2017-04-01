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

#include "stdint.h"

#define CREDENTIALS_LENGTH  50
#define CMD_LEN             256
#define DESC_LEN            256
#define CURPATH_LEN         256
#define MAX_NUM_COM         50
#define DEF_COM             18
#define HST_LEN             10

/// Pointer to the function of a commmand.
typedef void (* CommandFunction)(int argc, char ** argv);

/// @brief Holds information about a command.
typedef struct command_t
{
    /// The name of the command.
    char cmdname[CMD_LEN];
    /// The function pointer to the command.
    CommandFunction function;
    /// The description of the command.
    char cmddesc[DESC_LEN];
} command_t;

/// @brief Holds information about the user.
typedef struct userenv_t
{
    /// The username.
    char username[CREDENTIALS_LENGTH];
    /// The current path.
    char cur_path[CURPATH_LEN];
    /// The user identifier.
    unsigned int uid;
    /// The group identifier.
    unsigned int gid;
} userenv_t;

/// Contains the list of all the commands.
extern command_t shell_cmd[MAX_NUM_COM];
/// Contains the information about the current user.
extern userenv_t current_user;
/// The input command.
extern char cmd[CMD_LEN];
/// The index of the cursor.
extern uint32_t cmd_cursor_index;

int shell(void *);

void move_cursor_left(void);

void move_cursor_right(void);
