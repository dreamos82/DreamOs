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
#include <io.h>
#include <video.h>
#include <stdio.h>
#include <stddef.h>

#define SERIAL_COM1 (0x03f8)

/******************************
 *		Debug Func
 ******************************/

/**
 *  Print msg in the bochs prompt 
 */
void dbg_bochs_print(const char * msg)
{
#if defined(BOCHS_DEBUG)
    register unsigned int i;
    for (i = 0; msg[i] != '\0'; i++)
        outportb(0xE9, msg[i]);
#else
    printf(msg);
#endif
    return;
}

/**
 *  Print an integer
 */
void dbg_bochs_print_digit(unsigned int number)
{
    char buffer[10];
    _kntos(buffer, number, 10);
    dbg_bochs_print(buffer);
    dbg_bochs_print("\n");
}

/**
 *  Send cmd to bochs port
 */
void dbg_bochs_send_cmd(const int port, const int cmd)
{
#if defined(BOCHS_DEBUG)
    outportb(port, cmd);
#endif
    return;
}

void dbg_qemu_print(const char * msg, ...)
{
    va_list ap;
    // Start variabile argument's list.
    va_start (ap, msg);
    // Define a buffer for the arguments.
    char buffer[1024];
    // Format the message.
    vsprintf(buffer, msg, ap);
    // Print on COM1 the message.
    register int it;
    for (it = 0; buffer[it] != 0; ++it)
    {
        outportb(SERIAL_COM1, buffer[it]);
    }
    // End the list of arguments.
    va_end (ap);
}
