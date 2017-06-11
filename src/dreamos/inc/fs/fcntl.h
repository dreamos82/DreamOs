/// @file   fcntl.h
/// @brief  Headers of functions fcntl() and open().
/// @details Based on JamesM's kernel developement tutorials.
/// @author Ivan Gualandri
/// @date   Mar 31 2007
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

/// Open for reading only.
#define O_RDONLY    0x00
/// Open for writing only.
#define O_WRONLY    0x01
/// Open for reading and writing.
#define O_RDWR      0x02
/// If the file exists, this flag has no effect. Otherwise, the file is created.
#define O_CREAT     0x40
/// The file offset will be set to the end of the file prior to each write.
#define O_APPEND    0x400

/// @brief Given a pathname for a file, open() returns a file descriptor, a
/// small, nonnegative integer for use in subsequent system calls.
/// @param pathname A pathname for a file.
/// @param oflags   Used to set the file status flags and file access modes
///                  of the open file description.
/// @param mode     Specifies the file mode bits be applied when a new file
///                  is created.
/// @return Returns a file descriptor, a small, nonnegative integer for use
/// in subsequent system calls.
int open(const char * pathname, int oflags, mode_t mode);
