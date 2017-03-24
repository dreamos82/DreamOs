//
// string.c
//
// String routines
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#include <string.h>
#include <ctype.h>
#include <kheap.h>

#define KERNEL 1

#ifndef KERNEL
#include <ctype.h>
#endif

char * strncpy(char * dest, const char * source, size_t n)
{
    char * start = dest;
    while (n && (*dest++ = *source++)) n--;
    if (n) while (--n) *dest++ = '\0';
    return start;
}

int strncmp(const char * s1, const char * s2, size_t n)
{
    if (!n) return 0;

    while (--n && *s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }

    return *(unsigned char *) s1 - *(unsigned char *) s2;
}

int stricmp(const char * s1, const char * s2)
{
    while (*s2 != 0 && toupper(*s1) == toupper(*s2)) s1++, s2++;
    return (toupper(*s1) - toupper(*s2));
}

int strnicmp(const char * s1, const char * s2, size_t n)
{
    int f, l;

    do
    {
        if (((f = (unsigned char) (*(s1++))) >= 'A') && (f <= 'Z'))
            f -= 'A' - 'a';
        if (((l = (unsigned char) (*(s2++))) >= 'A') && (l <= 'Z'))
            l -= 'A' - 'a';
    } while (--n && f && (f == l));

    return f - l;
}

int strcasecmp(const char * s1, const char * s2)
{
    return stricmp(s1, s2);
}

int strncasecmp(const char * s1, const char * s2, size_t n)
{
    return strnicmp(s1, s2, n);
}

char * strchr(const char * s, int ch)
{
    while (*s && *s != (char) ch) s++;
    if (*s == (char) ch) return (char *) s;
    return NULL;
}

char * strrchr(const char * s, int ch)
{
    char * start = (char *) s;

    while (*s++);
    while (--s != start && *s != (char) ch);
    if (*s == (char) ch) return (char *) s;

    return NULL;
}

char * strstr(const char * str1, const char * str2)
{
    char * cp = (char *) str1;
    char * s1, * s2;

    if (!*str2) return (char *) str1;

    while (*cp)
    {
        s1 = cp;
        s2 = (char *) str2;

        while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
        if (!*s2) return cp;
        cp++;
    }

    return NULL;
}

size_t strspn(const char * string, const char * control)
{
    const char * str = string;
    const char * ctrl = control;

    char map[32];
    size_t n;

    // Clear out bit map
    for (n = 0; n < 32; n++) map[n] = 0;

    // Set bits in control map
    while (*ctrl)
    {
        map[*ctrl >> 3] |= (char) (1 << (*ctrl & 7));
        ctrl++;
    }

    // 1st char NOT in control map stops search
    if (*str)
    {
        n = 0;
        while (map[*str >> 3] & (1 << (*str & 7)))
        {
            n++;
            str++;
        }

        return n;
    }

    return 0;
}

size_t strcspn(const char * string, const char * control)
{
    const char * str = string;
    const char * ctrl = control;

    char map[32];
    size_t n;

    // Clear out bit map
    for (n = 0; n < 32; n++) map[n] = 0;

    // Set bits in control map
    while (*ctrl)
    {
        map[*ctrl >> 3] |= (char) (1 << (*ctrl & 7));
        ctrl++;
    }

    // 1st char in control map stops search
    n = 0;
    map[0] |= 1;
    while (!(map[*str >> 3] & (1 << (*str & 7))))
    {
        n++;
        str++;
    }
    return n;
}

char * strpbrk(const char * string, const char * control)
{
    const char * str = string;
    const char * ctrl = control;

    char map[32];
    int n;

    // Clear out bit map
    for (n = 0; n < 32; n++) map[n] = 0;

    // Set bits in control map
    while (*ctrl)
    {
        map[*ctrl >> 3] |= (char) (1 << (*ctrl & 7));
        ctrl++;
    }

    // 1st char in control map stops search
    while (*str)
    {
        if (map[*str >> 3] & (1 << (*str & 7))) return (char *) str;
        str++;
    }

    return NULL;
}

void * memmove(void * dst, const void * src, size_t n)
{
    void * ret = dst;

    if (dst <= src || (char *) dst >= ((char *) src + n))
    {
        // Non-overlapping buffers; copy from lower addresses to higher addresses
        while (n--)
        {
            *(char *) dst = *(char *) src;
            dst = (char *) dst + 1;
            src = (char *) src + 1;
        }
    }
    else
    {
        // Overlapping buffers; copy from higher addresses to lower addresses
        dst = (char *) dst + n - 1;
        src = (char *) src + n - 1;

        while (n--)
        {
            *(char *) dst = *(char *) src;
            dst = (char *) dst - 1;
            src = (char *) src - 1;
        }
    }

    return ret;
}

void * memchr(const void * buf, int ch, size_t n)
{
    while (n && (*(unsigned char *) buf != (unsigned char) ch))
    {
        buf = (unsigned char *) buf + 1;
        n--;
    }

    return (n ? (void *) buf : NULL);
}

#ifndef KERNEL

char *strdup(const char *s)
{
  char *t;
  int len;

  if (!s) return NULL;
  len = strlen(s);
  t = (char *) malloc(len + 1);
  memcpy(t, s, len + 1);
  return t;
}

char *_lstrdup(const char *s)
{
  char *t;
  int len;

  if (!s) return NULL;
  len = strlen(s);
  t = (char *) _lmalloc(len + 1);
  memcpy(t, s, len + 1);
  return t;
}

char *strlwr(char *s)
{
  char *p = s;

  while (*p)
  {
    *p = (char) tolower(*p);
    p++;
  }

  return s;
}

char *strupr(char *s)
{
  char *p = s;

  while (*p)
  {
    *p = (char) toupper(*p);
    p++;
  }

  return s;
}

#endif

char * strncat(char * s1, const char * s2, size_t n)
{
    char * start = s1;

    while (*s1++);
    s1--;

    while (n--)
    {
        if (!(*s1++ = *s2++)) return start;
    }

    *s1 = '\0';
    return start;
}

char * strnset(char * s, int c, size_t n)
{
    while (n-- && *s) *s++ = (char) c;
    return s;
}

char * strrev(char * s)
{
    char * start = s;
    char * left = s;
    char ch;

    while (*s++);
    s -= 2;

    while (left < s)
    {
        ch = *left;
        *left++ = *s;
        *s-- = ch;
    }

    return start;
}

char * strtok_r(char * string, const char * control, char ** lasts)
{
    char * str;
    const char * ctrl = control;

    char map[32];
    int n;

    // Clear control map
    for (n = 0; n < 32; n++) map[n] = 0;

    // Set bits in delimiter table
    do
    { map[*ctrl >> 3] |= (char) (1 << (*ctrl & 7)); }
    while (*ctrl++);

    // Initialize str. If string is NULL, set str to the saved
    // pointer (i.e., continue breaking tokens out of the string
    // from the last strtok call)
    if (string)
        str = string;
    else
        str = *lasts;

    // Find beginning of token (skip over leading delimiters). Note that
    // there is no token iff this loop sets str to point to the terminal
    // null (*str == '\0')

    while ((map[*str >> 3] & (1 << (*str & 7))) && *str) str++;

    string = str;

    // Find the end of the token. If it is not the end of the string,
    // put a null there
    for (; *str; str++)
    {
        if (map[*str >> 3] & (1 << (*str & 7)))
        {
            *str++ = '\0';
            break;
        }
    }

    // Update nexttoken
    *lasts = str;

    // Determine if a token has been found
    if (string == (char *) str)
        return NULL;
    else
        return string;
}

#ifndef KERNEL

char *strtok(char *string, const char *control)
{
  return strtok_r(string, control, &gettib()->nexttoken);
}

#endif

/////////////////////////////////////////////////////////////////////
//
// intrinsic functions
//

/*#pragma function(memset)
#pragma function(memcmp)
#pragma function(memcpy)

#pragma function(strcpy)
#pragma function(strlen)
#pragma function(strcat)
#pragma function(strcmp)
#pragma function(strset)*/

void * memset(void * ptr, int value, size_t num)
{
    // Truncate c to 8 bits.
    value = (value & 0xFF);
    char * dst = (char *) ptr;
    // Initialize the rest of the size.
    while (num--) *dst++ = (char) value;
    return ptr;
}

int memcmp(const void * dst, const void * src, size_t n)
{
    if (!n) return 0;

    while (--n && *(char *) dst == *(char *) src)
    {
        dst = (char *) dst + 1;
        src = (char *) src + 1;
    }

    return *((unsigned char *) dst) - *((unsigned char *) src);
}

void * memcpy(void * _dst, const void * _src, size_t num)
{
    char * dst = _dst;
    const char * src = _src;
    while (num--) *dst++ = *src++;
    return _dst;
}

void * memccpy(void * dst, const void * src, int c, size_t n)
{
    while (n && (*((char *) (dst = (char *) dst + 1) - 1) =
                     *((char *) (src = (char *) src + 1) - 1)) != (char) c)
        n--;

    return n ? dst : NULL;
}

#ifndef KERNEL

int memicmp(const void *buf1, const void *buf2, size_t n)
{
  int f = 0, l = 0;
  const unsigned char *dst = buf1, *src = buf2;

  while (n-- && f == l)
  {
    f = tolower(*dst++);
    l = tolower(*src++);
  }

  return f - l;
}

#endif

char * strcpy(char * dst, const char * src)
{
    char * save = dst;
    while ((*dst++ = *src++) != '\0');
    return save;
}

size_t strlen(const char * s)
{
    const char * eos;
    for (eos = s; *eos != 0; ++eos);
    long len = eos - s;
    return (len < 0) ? 0 : (size_t) len;
}

size_t strnlen(const char * s, size_t count)
{
    const char * sc;
    for (sc = s; *sc != '\0' && count--; ++sc);
    long len = sc - s;
    return (len < 0) ? 0 : (size_t) len;
}

int strcmp(const char * s1, const char * s2)
{
    int ret = 0;
    while (!(ret = *(unsigned char *) s1 - *(unsigned char *) s2) && *s2)
        ++s1, ++s2;

    if (ret < 0)
        ret = -1;
    else if (ret > 0)
        ret = 1;

    return ret;
}

char * strcat(char * dst, const char * src)
{
    char * cp = dst;
    while (*cp) cp++;
    while ((*cp++ = *src++) != '\0');
    return dst;
}

char * strset(char * s, int c)
{
    char * start = s;
    while (*s) *s++ = (char) c;
    return start;
}

/* Written by shainer.
 * Separate a string in token according to the delimiter
 * If str is NULL, the scanning will continue for the previous string
 * It can be bettered
 */

char * strtok(char * s, const char * delim)
{
    const char * spanp;
    int c, sc;
    char * tok;
    static char * last;


    if (s == NULL && (s = last) == NULL)
        return (NULL);

    cont:
    c = *s++;
    for (spanp = delim; (sc = *spanp++) != 0;)
    {
        if (c == sc)
            goto cont;
    }

    if (c == 0)
    {
        last = NULL;
        return (NULL);
    }
    tok = s - 1;

    for (;;)
    {
        c = *s++;
        spanp = delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
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
 * Compare n characters of s2 and s1
 */
int _kstrncmp(const char * s1, const char * s2, size_t num)
{
    // If the number of characters that has to be checked is equal to zero,
    // just return 0.
    if (num == 0)
    {
        return 0;
    }
    size_t sn = 0;
    while ((*s1 == *s2) && (sn < (num - 1)))
    {
        ++s1;
        ++s2;
        ++sn;
    }
    if (*s1 > *s2) return 1;
    if (*s1 < *s2) return -1;
    return 0;
}

char * trim(char * str)
{
    size_t len = 0;
    char * frontp = str;
    char * endp = NULL;

    if (str == NULL)
    {
        return NULL;
    }
    if (str[0] == '\0')
    {
        return str;
    }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while (isspace((unsigned char) *frontp))
    {
        ++frontp;
    }
    if (endp != frontp)
    {
        while (isspace((unsigned char) *(--endp)) && endp != frontp);
    }
    if (str + len - 1 != endp)
    {
        *(endp + 1) = '\0';
    }
    else if (frontp != str && endp == frontp)
    {
        *str = '\0';
    }
    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if (frontp != str)
    {
        while (*frontp)
        {
            *endp++ = *frontp++;
        }
        *endp = '\0';
    }
    return str;
}


char * strdup(const char * src)
{
    int len = strlen(src) + 1;
    char * dst = kmalloc(len);
    memcpy(dst, src, len);
    return dst;
}

char * strsep(char ** stringp, const char * delim)
{
    char * s;
    const char * spanp;
    int c, sc;
    char * tok;
    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;)
    {
        c = *s++;
        spanp = delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

/*
   Split a string into list of strings
   */
list_t * str_split(const char * str,
                   const char * delim,
                   unsigned int * numtokens)
{
    list_t * ret_list = list_create();
    char * s = strdup(str);
    char * token, * rest = s;
    while ((token = strsep(&rest, delim)) != NULL)
    {
        if (!strcmp(token, ".")) continue;
        if (!strcmp(token, ".."))
        {
            if (list_size(ret_list) > 0) list_pop(ret_list);
            continue;
        }
        list_push(ret_list, strdup(token));
        if (numtokens) (*numtokens)++;
    }
    kfree(s);
    return ret_list;
}

/*
 * Reconstruct the string with tokens and delimiters
 * */
char * list2str(list_t * list, const char * delim)
{
    char * ret = kmalloc(256);
    memset(ret, 0, 256);
    size_t len = 0;
    size_t ret_len = 256;
    while (list_size(list) > 0)
    {
        char * temp = list_pop(list)->val;
        size_t len_temp = strlen(temp);
        if (len + len_temp + 1 + 1 > ret_len)
        {
            ret_len = ret_len * 2;
            kfree(ret);
            ret = kmalloc(ret_len);
            len = len + len_temp + 1;
        }
        strcat(ret, delim);
        strcat(ret, temp);
    }
    return ret;
}
