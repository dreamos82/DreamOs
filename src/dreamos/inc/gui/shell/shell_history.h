/// @file   shell_history.h
/// @brief  Functions used to move through the history of the shell.
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

/// @brief Initialize the init.
void history_init(void);

/// @brief Push the command inside the history.
void history_push(char * command);

/// @brief Give the key allows to navigate through the history.
void history_start(const int key);

/// @brief Prints the history.
void history_print(int argc, char ** argv);
