/***************************************************************************
 *            stdio.h
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Standard I/0 header
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

#ifndef _STDIO_H
#define _STDIO_H

#define MAX_DIGITS_IN_INTEGER 11

#ifndef EOF
#define EOF (-1)
#endif

void putchar (char ch);
int atoi (const char *);
int printf (const char *, ...);
int puts (char *s);
int scanf (const char *, ...);
char *gets (char *);
int getchar (void);

#endif
