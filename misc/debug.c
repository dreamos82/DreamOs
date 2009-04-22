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
/******************************
 *		Debug Func
 ******************************/

/**
 *  Print msg in the bochs prompt 
 */ 
void dbg_bochs_print( const unsigned char *msg )
{
    register unsigned int i;
    for ( i = 0; msg[i] != '\0'; i++ )
        outportb(msg[i],0xE9);
}

/**
 *  Send cmd to bochs port
 */
inline void dbg_bochs_send_cmd( const int port, const int cmd )
{
    outportb(port, cmd);
}
