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

#define sti() __asm__ __volatile__ ("sti");
#define cli() __asm__ __volatile__ ("cli");
 
inline unsigned char inportb (int);
inline void outportb (int, int);
 
#endif
