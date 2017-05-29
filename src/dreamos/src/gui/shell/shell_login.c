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

#include "shell_login.h"
#include "stdio.h"
#include "vfs.h"
#include "fcntl.h"
#include "string.h"
#include "debug.h"
#include "video.h"
#include "keyboard.h"

/// @brief Contains the credentials retrieved from the file.
typedef struct credentials_t
{
    /// The username.
    char username[CREDENTIALS_LENGTH];
    /// The password.
    char password[CREDENTIALS_LENGTH];
} credentials_t;

/// @brief Initialize the given credentials.
void init_credentials(credentials_t * credentials)
{
    if (credentials != NULL)
    {
        memset(credentials->username, '\0', CREDENTIALS_LENGTH);
        memset(credentials->password, '\0', CREDENTIALS_LENGTH);
    }
}

/// @brief Get the
/// @param fd           The FD of the file which contains the credentials.
/// @param credentials  The structure which has to be filled with the
///                     credentials.
/// @return If the credentials has been retrieved.
bool_t user_get(int fd, struct credentials_t * credentials)
{
    // Create a support array of char.
    char support[CREDENTIALS_LENGTH];
    memset(support, '\0', CREDENTIALS_LENGTH);
    // Create a variable which will contain the error value for the read
    // functions.
    int error = 1;
    // --------------------------------
    // Get the username.
    int i = 0;
    while ((i < CREDENTIALS_LENGTH || support[i] != ':') && error != 0)
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
    replace_char(support, '\r', 0);
    // Save the username.
    strcpy(credentials->username, support);
    // Clear the support array.
    memset(support, '\0', CREDENTIALS_LENGTH);
    // --------------------------------
    // Get the password.
    i = 0;
    while ((i < CREDENTIALS_LENGTH || support[i] != '\n') && error != 0)
    {
        error = read(fd, &support[i], 1);
        if (support[i] == '\n')
        {
            support[i] = '\0';
            break;
        }
        ++i;
    }
    replace_char(support, '\r', 0);
    if (error == 0)
    {
        return false;
    }
    strcpy(credentials->password, support);
    return true;
}

/// @brief Checks if the given credentials are correct.
bool_t check_credentials(credentials_t * credentials)
{
    // Initialize a variable for the return value.
    bool_t status = false;
    // Initialize the structure which will contain the username and the
    // password.
    credentials_t existing;
    init_credentials(&existing);
    // Open the file which contains the credentials.
    // TODO: BUG: The first time the open is called, it fails.
    int fd = open("/passwd", O_RDONLY, 0);
    // Check the file descriptor.
    if (fd >= 0)
    {
        // Get the next row inside the file containing the credentials.
        while (user_get(fd, &existing) == true)
        {
            if (!strcmp(credentials->username, existing.username) &&
                !strcmp(credentials->password, existing.password))
            {
                status = true;
                break;
            }
        }
        // Close the file descriptor.
        close(fd);
    }
    else
    {
        dbg_print("Can't open passwd file\n");
    }
    return status;
}

void shell_login()
{
    do
    {
        // Initialize the credentials.
        credentials_t credentials;
        init_credentials(&credentials);
        // ----------------------------
        // Ask the username.
        printf(LNG_USER);
        // Update the lower-bounds for the video.
        lower_bound_x = video_get_column();
        lower_bound_y = video_get_line();
        // Get the username.
        scanf("%50s", credentials.username);
        // ----------------------------
        // Ask the password.
        printf(LNG_PWD);
        // Update the lower-bounds for the video.
        lower_bound_x = video_get_column();
        lower_bound_y = video_get_line();
        // Set the shadow option.
        keyboard_set_shadow(true);
        // Get the password.
        scanf("%50s", credentials.password);
        // Disable the shadow option.
        keyboard_set_shadow(false);
        // ----------------------------
        // Check if the data are correct.
        if (check_credentials(&credentials))
        {
            strcpy(current_user.username, credentials.username);
            break;
        }
        printf(LNG_WRONG_CRED);
    } while (true);
}

