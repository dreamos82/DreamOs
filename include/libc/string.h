/***************************************************************************
 *            string.h
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  String library header
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

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int _kstrncmp (const char *, const char *, int);
int strcmp (const char *, const char *);
void *memset(void *, register const int, register size_t);
extern void * memmove(void *,const void *,size_t);
void *memcpy(void *, const void *, size_t );
size_t strlen (const char *);
char *strncpy (char *, register const char *, size_t);
char *strchr (register const char *, register int);
char *strstr (const char *, const char *);
char *strtok (char *, const char *);
char *strncat (char *, const char *, size_t);
char *strcpy (char *, register const char *);

#endif
