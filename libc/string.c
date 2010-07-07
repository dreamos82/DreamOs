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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string.h>
#include <stddef.h>

/*
 * Copy n characters from src to dest, returning dest
 */
char *strncpy (char *dest, register const char *src, register size_t n)
{
  if (n != 0) {
	char *d = dest;
	const char *s = src;
	do {
		if ((*d++ = *s++) == 0) {
			while (--n != 0)
				*d++ = '\0';
				break;
			}
		} while (--n != 0);
	}
	return dest;
 }

char *strcpy (char *dest, const char *src)
{
	char *save = dest;
	for (; (*dest = *src); ++src, ++dest);
	return save;
}


/*
 * Return the number of a string's characters
 */
size_t strlen(const char *s) 
{
	register unsigned int len = 0;
	while (s[len] != '\0') 
		len++;

	return(len);	
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

int strncmp(const char *s1, const char *s2, int n){
	return _kstrncmp(s1,s2,n);
}

/*
 * Compare s1 and s2
 */
int strcmp (const char *s1, const char *s2)
{
  int s1n = strlen(s1);
  int s2n = strlen(s2);

  if (s1n > s2n)
    return 1;
  else if (s1n < s2n)
    return -1;

  while (*s1 == *s2 && s1n>=0)
  {
    s1++;
    s2++;
    s1n--;
  }

  if (s1n>0) {
    if (*s1 > *s2) return 1;
    else return -1;
  }
  return 0;
}

/* Fill memory location dest with c for n times */
void *memset(void *dest, register const int c, register size_t n)
{ 
	register char *s = dest;
	if ( n > 0 ) {
		n++;
		while (--n > 0)
		    *s++ = c;
	}
	return dest;
}

void *memmove(void * dest, const void *src, size_t count)
{
	register char *tmp;
	register const char *s;

	if ( count > 0 ) 
	{
		if (dest <= src) 
		{
			tmp = (char *) dest;
			s = (char *) src;
			
			while (count--)
				*tmp++ = *s++;
		}
		else 
		{
			tmp = (char *) dest + count;
			s = (char *) src + count;
			
			while (count--)
				*--tmp = *--s;
		}
	}
	return dest;
}

/*
 * Duplicate a string and return the new one
 */
char *strdup (const char *s)
{
    char *ret = NULL;
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
char *strchr(register const char *s, register int c)
{
	c = (char) c;
	while (c != *s)
		if (*s++ == '\0') 
			return NULL;  /* No Found */

	return (char *)s;
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
 * It can be bettered
 */

char *strtok(char *s, const char *delim)
{
  const char *spanp;
  int c, sc;
  char *tok;
  static char *last;


  if (s == NULL && (s = last) == NULL)
    return (NULL);

 cont:
  c = *s++;
  for (spanp = delim; (sc = *spanp++) != 0;) {
    if (c == sc)
      goto cont;
  }

  if (c == 0) {             
    last = NULL;
    return (NULL);
  }
  tok = s - 1;

  for (;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
          s = NULL;
        else
          s[-1] = 0;
        last = s;
        return (tok);
      }
    } while (sc != 0);
  }

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

void *memcpy(void *dest, const void *src, size_t n) { 
  // Copy a memory from src to dest with n length
  size_t i;
  for (i = 0; i < n; i++)
    *((char *) dest + i) = *((char *) src + i);
  return dest;
}
