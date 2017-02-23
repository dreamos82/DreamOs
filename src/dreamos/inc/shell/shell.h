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

#ifndef _SHELL_H
#define _SHELL_H

#define USER_LEN 22
#define CMD_LEN 256
#define DESC_LEN 256
#define CURPATH_LEN 256
#define MAX_NUM_COM 50
#define DEF_COM 18
#define HST_LEN 10

/// Pointer to the function of a commmand.
typedef void (* CommandFunction)(void);

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
    char username[USER_LEN];
    /// The current path.
    char cur_path[CURPATH_LEN];
    /// The user identifier.
    unsigned int uid;
    /// The group identifier.
    unsigned int gid;
} userenv_t;

extern command_t shell_cmd[MAX_NUM_COM];
extern userenv_t current_user;

int shell(void *);

/// @brief Initialize the init.
void history_init(void);

/// @brief Push the command inside the history.
void history_push(char * command);

/// @brief Give the key allows to navigate through the history.
void history_start(const int key);

/// @brief Prints the history.
void history_print(void);

#endif
