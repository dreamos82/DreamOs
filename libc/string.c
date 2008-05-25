/***************************************************************************
 *            string.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  String library
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

#include <string.h>
#include <stddef.h>

/*
 * Copy n characters from src to dest, returning dest
 */
char *strncpy (char *dest, const char *src, size_t n)
{
    int i;

    for (i=0; i<n; i++)
        dest[i] = src[i];

    return dest;
}

/*
 * Return the number of a string's characters
 */
int strlen (const char *s)
{
    int len = 0;
    while (*s != 0) { s++; len++; }

    return len;
}

/*
 * Compare n characters of s2 and s1
 */
int _kstrncmp(const char *s1, const char *s2, int n)
{
  int sn=0;

  while (*s1 == *s2 && sn < n-1)
  {
    s1++;
    s2++;
    sn++;
  }

  if (*s1 > *s2) return 1;
  if (*s1 < *s2) return -1;
  return 0;
}

/* Fill memory location dest with c for n times */
void *memset(void *dest, const int c, int n)
{
  int i;
  for (i = 0; i < n; i++)
    *((int *) dest + i) = c;
  return dest;
}

/*
 * Duplicate a string and return the new one
 */
char *strdup (const char *s)
{
    char *ret;
    int i=0;

    for (i=0; s[i]; i++)
	ret[i] = s[i];

    ret[i] = '\0';
    return ret;
}

/*
 * Look for the first occurence of c in s
 * Return the substring starting with c
 */
char *strchr (const char *s, int c)
{
    while (*s++ != c);

    return --s;
}

/*
 * Author: jmc
 * Look for the first occurrence of needle in haystack
 * Return the substring starting with needle
 */
char *strstr(const char *haystack, const char *needle)
{
  size_t length = strlen(needle);
  size_t last = strlen(haystack) - length;
  size_t x;
 
  for (x = 0; x <= last; x++)
    if (_kstrncmp(&haystack[x], needle, length) == 0)
      return (char *) &haystack[x];
 
  return NULL;
}

/*
 * Separate a string in token according to the delimitator
 * If str is NULL, the scanning will continue for the previous string
 */
char *strtok (char *str, const char *delim)
{
    static char *t;
    char *ret, *sptr;

    if (str)
	t = str;

    if ((ret = strstr(t, delim))) {
	sptr = t;
	while (t++ != ret);
	*t = '\0';
	t += strlen(delim);
	return sptr;
    }

    return NULL;
}

/*
 * Concatenate n characters of src to dest
 */
char *strncat (char *dest, const char *src, size_t n)
{
    size_t dlen = strlen (dest);
    int i;

    for (i=0; i<n && src[i]; i++)
	dest[dlen+i] = src[i];
    dest[dlen+i] = '\0';

    return dest;
}
