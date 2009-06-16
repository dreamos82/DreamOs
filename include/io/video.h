/***************************************************************************
 *            video.h
 *
 *  Sat Mar 31 07:57:13 2007
 *  Copyright  2007  shainer & ^Inuyasha^
 *  Video functions and costants
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
 
#ifndef _VIDEO_H
#define _VIDEO_H

#define _SCR_W 80
#define _SCR_H 25

#define OK_POS 70

void _kntos(char*,unsigned int, int);
void _kputc(char);
void _kcolor(char);
void _kputs(char *);
void _kbackspace();
void _kesetcursor(unsigned int, unsigned int);
void _kgoto(int, int);
void _ksetcursauto();
void _kshiftAll();
void _ktab();
void _kclear();
void _knewline();
void _kminline();
void _kprintOK();
int _kgetline();
int _kgetcolumn();
void _krotate_buffer();
void _kscrollup();
void _kscrolldown();
void _kntohex (char *, unsigned int);

unsigned short shell_mess_col, shell_mess_line;


#endif /* _VIDEO_H */
