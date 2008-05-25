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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int _kstrncmp (const char *, const char *, int);
void *memset(void *, const int, int);
int strlen (const char *);
char *strncpy (char *, const char *, size_t);
char *strchr (const char *, int);
char *strstr (const char *, const char *);
char *strtok (char *, const char *);
char *strncat (char *, const char *, size_t);

#endif
