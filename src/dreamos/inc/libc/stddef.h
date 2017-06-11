/// @file   stddef.h
/// @brief  Define basic data types.
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

#ifndef NULL
/// Define NULL.
#define NULL ((void*)0)
#endif

#ifndef EOF
/// Define End-Of-File.
#define EOF (-1)
#endif

/// Define the size of a buffer.
#define BUFSIZ 512

/// Define the size of the kernel.
#define KERNEL_SIZE 0x200

#ifndef TRUE
/// Define the value of true.
#define TRUE 1
#endif

#ifndef FALSE
/// Define the value of false.
#define FALSE 0
#endif

/// Define the byte type.
typedef unsigned char byte_t;
/// Define the generic size type.
typedef unsigned int size_t;
/// Define the generic signed size type.
typedef int ssize_t;
/// Define the type of an inode.
typedef unsigned int ino_t;
/// TODO: Check meaning!
typedef unsigned int dev_t;
/// The type of user-id.
typedef unsigned int uid_t;
/// The type of group-id.
typedef unsigned int gid_t;
/// The type of offset.
typedef unsigned int off_t;
/// The type of mode.
typedef unsigned int mode_t;
/// The type of process id.
typedef int pid_t;
