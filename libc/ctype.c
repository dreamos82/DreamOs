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

/* Is c a digit? */
int isdigit(int c) {
  if (c >= 48 && c <= 57)
    return 1;
  return 0;
}

/* Is c a letter? */
int isalpha(int c)
{
  if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122 ))
    return 1;
  return 0;
}

/* Is c a letter or a digit? */
int isalnum(int c)
{
  return (isalpha(c) || isdigit(c));
}

/* Is c an hexadecimal digit? */
int isxdigit(int c)
{
  if (isdigit(c) || (c >= 65 && c <= 70 ))
    return 1;
  return 0;
}

/* Is c a lowercase letter? */
int islower(int c)
{
  if (c >= 97 && c <= 122)
    return 1;
  return 0;
}

/* Is c an uppercase letter? */
int isupper(int c)
{
  if (c >= 65 && c <= 90)
    return 1;
  return 0;
}

/* Convert c to a lowercase char */
int tolower(int c)
{
  if (isalpha(c) == 0 || islower(c))
    return c;
  else return c + OFFSET;
}

/* Convert c to an uppercase char */
int toupper(int c)
{
  if (isalpha(c) == 0 || isupper(c))
    return c;
  else return c - OFFSET;
}
