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

#ifndef _IO_H
#define _IO_H

/// @brief Used for reading from the I/O ports.
/// @param port The input port.
/// @return The read value.
unsigned char inportb(const unsigned short port);

/// @brief Use this to write to I/O ports to send bytes to devices.
/// @param port The output port.
/// @param data The data to write.
void outportb(const unsigned short port,
              const unsigned char data);

#endif
