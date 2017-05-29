/*
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


/***************************************************************************
 *            io.h
 *
 *  Sat Mar 31 07:57:13 2007
 *  Copyright  2007  shainer & ^Inuyasha^
 *  Byte I/O on ports prototypes
 ****************************************************************************/

#pragma once

#include "stdint.h"

/// @brief Used for reading from the I/O ports.
/// @param port The input port.
/// @return The read value.
uint8_t inportb(const uint16_t port);

/// @brief Used for reading 2 bytes from the I/O ports.
/// @param port The input port.
/// @return The read value.
uint16_t inports(const uint16_t _port);

/// @brief Used for reading 4 bytes from the I/O ports.
/// @param port The input port.
/// @return The read value.
uint32_t inportl(const uint16_t _port);

/// @brief Use this to write to I/O ports to send bytes to devices.
/// @param port The output port.
/// @param data The data to write.
void outportb(const uint16_t port, const uint8_t data);

/// @brief Use this to write to I/O ports to send 2 bytes to devices.
/// @param port The output port.
/// @param data The data to write.
void outports(const uint16_t _port, const uint16_t _data);

/// @brief Use this to write to I/O ports to send 4 bytes to devices.
/// @param port The output port.
/// @param data The data to write.
void outportl(const uint16_t _port, const uint32_t _data);
