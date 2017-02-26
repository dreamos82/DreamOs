/***************************************************************************
 *            debug.c
 *
 *  Sat Mar 31 07:47:17 2007
 *  Copyright  2007  Theking0 - finarfin
 *  Email
 ****************************************************************************/

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


#include <debug.h>
#include <stdio.h>

#define SERIAL_BOCHS    (0x00E9)
#define SERIAL_COM1     (0x03F8)

void dbg_print(const char * msg, ...)
{
    // Define a buffer for the formatted string.
    char formatted[1024];
    // --------------------------------
    // Stage 1: FORMAT
    // Start variabile argument's list.
    va_list ap;
    va_start (ap, msg);
    // Format the message.
    vsprintf(formatted, msg, ap);
    // End the list of arguments.
    va_end (ap);
    // --------------------------------
    // Stage 2: SEND
    register int it;
    for (it = 0; formatted[it] != 0; ++it)
    {
#if     defined(BOCHS_DEBUG)
        outportb(SERIAL_BOCHS, formatted[it]);
#elif   defined(QEMU_DEBUG)
        outportb(SERIAL_COM1, formatted[it]);
#endif
    }
    // --------------------------------
}