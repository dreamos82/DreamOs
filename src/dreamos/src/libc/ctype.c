/***************************************************************************
 *            ctype.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Characters library
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

#include<ctype.h>

/// Distance from a uppercase character to the correspondent lowercase in ASCII.
#define OFFSET 32

int isdigit(int c)
{
    return (c >= 48 && c <= 57);
}

int isalpha(int c)
{
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int isalnum(int c)
{
    return (isalpha(c) || isdigit(c));
}

int isxdigit(int c)
{
    return (isdigit(c) || (c >= 65 && c <= 70));
}

int islower(int c)
{
    return (c >= 97 && c <= 122);
}

int isupper(int c)
{
    return (c >= 65 && c <= 90);
}

int tolower(int c)
{
    if (isalpha(c) == 0 || islower(c)) return c;
    return c + OFFSET;
}

int toupper(int c)
{
    if (isalpha(c) == 0 || isupper(c)) return c;
    return c - OFFSET;
}

int isspace(int c)
{
    return (c == ' ');
}
