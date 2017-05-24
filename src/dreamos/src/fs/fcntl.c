/*
 * Dreamos
 * fcntl.c
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <vfs.h>
#include <fcntl.h>
#include <string.h>
#include <debug.h>
#include "kheap.h"

#ifdef LEGACY

#endif
#ifdef LATEST
#include <heap.h>
#endif

#include <shell.h>

/**
  * @author Ivan Gualandri
  * @param char* path percorso del file da aprire
  * @param int oflags parametri di apertura
  *
  * Dato un path viene aperto se presente, e si torna il numero di descrittore che lo contiene
  * @todo Inserire gestione flags
  */
int open(const char * path, int oflags, mode_t mode)
{
    // Allocate a variable for the path.
    char new_path[CURPATH_LEN];
    // Initialize the path variable.
    memset(new_path, '\0', CURPATH_LEN);
    // Copy the path to the working variable.
    strcpy(new_path, path);
    // If the first character is not the '/' then get the absolute path.
    if (new_path[0] != '/')
    {
        if (!get_absolute_path(new_path))
        {
            dbg_print("Cannot get the absolute path.\n");
            return -1;
        }
    }
    // Reset the current file descriptor variable.
    current_fd = 0;
    while ((fd_list[current_fd].mountpoint_id != -1) &&
           (current_fd < _SC_OPEN_MAX))
    {
        ++current_fd;
    }
    if (current_fd == _SC_OPEN_MAX)
    {
        dbg_print("No more file descriptors available\n");
        return -1;
    }
    int32_t mp_id = get_mountpoint_id(new_path);
    if (mp_id < 0)
    {
        dbg_print("That path doesn't exist\n");
        return -1;
    }
    fd_list[current_fd].mountpoint_id = mp_id;
    char * relative_path = get_relative_path((uint32_t) mp_id, new_path);
    strcpy(new_path, relative_path);
    kfree(relative_path);
    if (mountpoint_list[mp_id].operations.open == NULL)
    {
        dbg_print("No OPEN function found here.\n");
        return -1;
    }
    fd_list[current_fd].fs_spec_id =
            mountpoint_list[mp_id].operations.open(new_path, oflags, mode);
    if (fd_list[current_fd].fs_spec_id == -1)
    {
        dbg_print("No file's Found\n");
        return -1;
    }
    // Reset the offset.
    fd_list[current_fd].offset = 0;
    // Set the flags.
    fd_list[current_fd].flags_mask = oflags;
    // Save the file descriptor.
    int ret_fd = current_fd;
    // Increment the current file descriptor.
    current_fd++;
    // Return the file descriptor.
    return ret_fd;
}

