/***************************************************************************
 *            io.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Byte I/O on a port
 * ***************************************************************************/

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

#include <io.h>

inline uint8_t inportb(uint16_t port)
{
    unsigned char data = 0;
    __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

inline void outportb(const uint16_t port, const uint32_t data)
{
    __asm__ __volatile__ ("outb %%al, %%dx"::"a" (data), "d" (port));
}
