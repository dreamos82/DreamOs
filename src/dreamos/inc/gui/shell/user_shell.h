/*
 * Dreamos
 * user_shell.h
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include "stdbool.h"

#define CREDENTIALS_LENGTH 50

/// @brief Contains the credentials retrieved from the file.
typedef struct credentials_t
{
    /// The username.
    char username[CREDENTIALS_LENGTH];
    /// The password.
    char password[CREDENTIALS_LENGTH];
} credentials_t;

void init_credentials(credentials_t * credentials);

bool_t check_credentials(credentials_t * credentials);
