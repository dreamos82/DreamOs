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

#include <port_io.h>

inline uint8_t inportb(const uint16_t port)
{
    unsigned char data = 0;
    __asm__ __volatile__("inb %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

inline uint16_t inports(const uint16_t _port)
{
    uint16_t rv;
    __asm__ __volatile__("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

inline uint32_t inportl(const uint16_t _port)
{
    uint32_t rv;
    __asm__ __volatile__("inl %%dx, %%eax" : "=a" (rv) : "dN" (_port));
    return rv;
}

inline void outportb(const uint16_t port, const uint8_t data)
{
    __asm__ __volatile__("outb %%al, %%dx"::"a" (data), "d" (port));
}

inline void outports(const uint16_t _port, const uint16_t _data)
{
    __asm__ __volatile__("outw %1, %0" : : "dN" (_port), "a" (_data));
}

inline void outportl(const uint16_t _port, const uint32_t _data)
{
    __asm__ __volatile__("outl %%eax, %%dx" : : "dN" (_port), "a" (_data));
}
