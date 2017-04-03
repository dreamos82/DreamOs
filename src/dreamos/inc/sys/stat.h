/*
 * Dreamos
 * stat.h
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

#ifndef _STAT_H
#define _STAT_H

#include <stddef.h>
#include "clock.h"

/// @brief Data structure which contains information about a file.
typedef struct stat_t
{
    /// ID of device containing file.
    dev_t st_dev;
    /// Fle serial number.
    ino_t st_ino;
    /// Mode of file.
    mode_t st_mode;
    /// User id del file.
    uid_t st_uid;
    /// Group id del file.
    gid_t st_gid;
    /// Dimensione del file.
    off_t st_size;
    /// Time of last access.
    time_t st_atime;
    /// Time of last data modification.
    time_t st_mtime;
    /// Time of last status change.
    time_t st_ctime;
} stat_t;

/// @brief Retrieves information about the file at the given location.
/// @param path The file descriptor of the file that is being inquired.
/// @param buf  A structure where data about the file will be stored.
/// @return Returns a negative value on failure.
int stat(const char * path, stat_t * buf);

#endif
