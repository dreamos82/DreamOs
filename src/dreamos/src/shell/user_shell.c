/*
 * Dreamos
 * user_shell.c
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

#include <user_shell.h>
#include <stdio.h>
#include <vfs.h>
#include <fcntl.h>
#include <string.h>
#include <debug.h>
#include <language.h>

/// @brief Contains the credentials retrieved from the file.
typedef struct credentials_t
{
    /// The username.
    char username[50];
    /// The password.
    char password[50];
} credentials_t;

/// @brief Get the
/// @param fd           The FD of the file which contains the credentials.
/// @param credentials  The structure which has to be filled with the
///                     credentials.
/// @return If the credentials has been retrieved.
bool_t user_get(int fd, struct credentials_t * credentials);

bool_t user_chk(char * username, char * password)
{
    // Initialize a variable for the return value.
    bool_t status = false;
    // Initialize the structure which will contain the username and the
    // password.
    credentials_t credentials;
    memset(credentials.username, '\0', 50);
    memset(credentials.password, '\0', 50);
    // Open the file which contains the credentials.
    int fd = open("/passwd", O_RDONLY, 0);
    // Check the file descriptor.
    if (fd > 0)
    {
        // Get the next row inside the file containing the credentials.
        while (user_get(fd, &credentials) == true)
        {
            if (!strcmp(username, credentials.username) &&
                !strcmp(password, credentials.password))
            {
                status = true;
                break;
            }
        }
        // Close the file descriptor.
        close(fd);
    }
    return status;
}

bool_t user_get(int fd, struct credentials_t * credentials)
{
    // Create a support array of char.
    char support[51];
    memset(support, '\0', 50);
    // Create a variable which will contain the error value for the read
    // functions.
    int error = 1;

    // --------------------------------
    // Get the username.
    int i = 0;
    while ((i < 50 || support[i] != ':') && error != 0)
    {
        error = read(fd, &support[i], 1);
        if (support[i] == ':')
        {
            support[i] = '\0';
            break;
        }
        ++i;
    }
    if (error == 0)
    {
        return false;
    }
    // Save the username.
    strcpy(credentials->username, support);
    // Clear the support array.
    memset(support, '\0', 50);
    // --------------------------------
    // Get the password.
    i = 0;
    while ((i < 50 || support[i] != '\n') && error != 0)
    {
        error = read(fd, &support[i], 1);
        if (support[i] == '\n')
        {
            support[i] = '\0';
            break;
        }
        ++i;
    }
    if (error == 0)
    {
        return false;
    }
    strcpy(credentials->password, support);
    return true;
}
