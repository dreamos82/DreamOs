/// @file   unistd.h
/// @brief  Functions used to manage files.
/// @author Ivan Gualandri
/// @date   Oct 27 2003
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

#include "stddef.h"

/// Maximum number of opened file.
#define _SC_OPEN_MAX 4

/// @brief Read a line from the file pointed by the file descriptor.
/// @param fildes   The file descriptor.
/// @param buf      The buffer.
/// @param nbyte    The number of bytes to read.
/// @return The number of read characters.
ssize_t read(int fildes, void * buf, size_t nbyte);

/// @brief Write the the buffer inside the file.
/// @param fildes   The file descriptor.
/// @param buf      The buffer to write.
/// @param nbyte    The number of bytes to write.
/// @return The number of written bytes.
ssize_t write(int fildes, const void * buf, size_t nbyte);

/// @brief Close the file pointed by the file descriptor.
/// @param fildes The file descriptor.
/// @return The result of the operation.
int close(int fildes);
