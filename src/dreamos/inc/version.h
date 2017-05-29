/*
 * Version.h
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * Author: Osiris
 * osiris@katamail.com
 */

#pragma once

/// The name of the operating system
#define OS_NAME "DreamOS"

/// The site of the operating system
#define OS_SITEURL "www.dreamos.org"

/// Major version of the operating system
#define OS_MAJOR_VERSION 0

/// Minor version of the operating system
#define OS_MINOR_VERSION 3

/// Micro version of the operating system
#define OS_MICRO_VERSION 0

#define OS_STR_HELPER(x) #x
#define OS_STR(x) OS_STR_HELPER(x)
/// Complete version of the operating system
#define OS_VERSION \
    OS_STR(OS_MAJOR_VERSION) "." \
    OS_STR(OS_MINOR_VERSION) "." \
    OS_STR(OS_MICRO_VERSION)
