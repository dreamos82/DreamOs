/// @file   utsname.h
/// @brief  Functions used to provide information about the machine & OS.
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

/// Maximum length of the string used by utsname.
#define SYS_LEN 257

/// @brief Holds information concerning the machine and the os.
typedef struct utsname_t
{
    /// The name of the system.
    char sysname[SYS_LEN];
    /// The name of the node.
    char nodename[SYS_LEN];
    /// The version of the OS.
    char version[SYS_LEN];
    /// The name of the machine.
    char machine[SYS_LEN];
} utsname_t;

/// @brief Sets the values of os_infos.
int uname(utsname_t * os_infos);
