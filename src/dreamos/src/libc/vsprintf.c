/// @file   vsprintf.c
/// @brief  Print formatting routines.
/// @author Michael Ringgaard
/// @date   2002
/// @copyright
/// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions
/// are met:
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the project nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
/// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
/// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
/// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
/// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
/// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
/// SUCH DAMAGE.

#include "stdarg.h"
#include "string.h"
#include "ctype.h"

#define KERNEL 1

#ifdef KERNEL
#define NOFLOAT
#endif

#define ZEROPAD 1               // Pad with zero
#define SIGN    2               // Unsigned/signed long
#define PLUS    4               // Show plus
#define SPACE   8               // Space if plus
#define LEFT    16              // Left justified
#define SPECIAL 32              // 0x
#define LARGE   64              // Use 'ABCDEF' instead of 'abcdef'

static char * digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static char * upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static int skip_atoi(const char ** s)
{
    int i = 0;
    do
    {
        i = i * 10 + *((*s)++) - '0';
    } while (isdigit(**s));
    return i;
}

static char * number(char * str,
                     long num,
                     int base,
                     int size,
                     int32_t precision,
                     int type)
{
    char c, tmp[66];
    char * dig = digits;

    if (type & LARGE) dig = upper_digits;
    if (type & LEFT) type &= ~ZEROPAD;
    if (base < 2 || base > 36) return 0;

    c = (type & ZEROPAD) ? '0' : ' ';
    // --------------------------------
    // Set the sign.
    char sign = 0;
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
            size--;
        }
        else if (type & PLUS)
        {
            sign = '+';
            size--;
        }
        else if (type & SPACE)
        {
            sign = ' ';
            size--;
        }
    }
    // Sice I've removed the sign (if negative), i can transform it to unsigned.
    uint32_t uns_num = (uint32_t) num;

    if (type & SPECIAL)
    {
        if (base == 16)
        {
            size -= 2;
        }
        else if (base == 8)
        {
            size--;
        }
    }

    int32_t i = 0;
    if (uns_num == 0)
    {
        tmp[i++] = '0';
    }
    else
    {
        while (uns_num != 0)
        {
            tmp[i++] = dig[((unsigned long) uns_num) % (unsigned) base];
            uns_num = ((unsigned long) uns_num) / (unsigned) base;
        }
    }

    if (i > precision)
    {
        precision = i;
    }
    size -= precision;
    if (!(type & (ZEROPAD | LEFT)))
    {
        while (size-- > 0) *str++ = ' ';
    }
    if (sign)
    {
        *str++ = sign;
    }

    if (type & SPECIAL)
    {
        if (base == 8)
            *str++ = '0';
        else if (base == 16)
        {
            *str++ = '0';
            *str++ = digits[33];
        }
    }

    if (!(type & LEFT))
    {
        while (size-- > 0) *str++ = c;
    }
    while (i < precision--)
    {
        *str++ = '0';
    }
    while (i-- > 0)
    {
        *str++ = tmp[i];
    }
    while (size-- > 0)
    {
        *str++ = ' ';
    }
    return str;
}

static char * eaddr(char * str,
                    unsigned char * addr,
                    int size,
                    int precision,
                    int type)
{
    (void) precision;
    char tmp[24];
    char * dig = digits;
    int i, len;

    if (type & LARGE) dig = upper_digits;
    len = 0;
    for (i = 0; i < 6; i++)
    {
        if (i != 0) tmp[len++] = ':';
        tmp[len++] = dig[addr[i] >> 4];
        tmp[len++] = dig[addr[i] & 0x0F];
    }

    if (!(type & LEFT)) while (len < size--) *str++ = ' ';
    for (i = 0; i < len; ++i) *str++ = tmp[i];
    while (len < size--) *str++ = ' ';

    return str;
}

static char * iaddr(char * str,
                    unsigned char * addr,
                    int size,
                    int precision,
                    int type)
{
    (void) precision;
    char tmp[24];
    int i, n, len;

    len = 0;
    for (i = 0; i < 4; i++)
    {
        if (i != 0) tmp[len++] = '.';
        n = addr[i];

        if (n == 0)
            tmp[len++] = digits[0];
        else
        {
            if (n >= 100)
            {
                tmp[len++] = digits[n / 100];
                n = n % 100;
                tmp[len++] = digits[n / 10];
                n = n % 10;
            }
            else if (n >= 10)
            {
                tmp[len++] = digits[n / 10];
                n = n % 10;
            }

            tmp[len++] = digits[n];
        }
    }

    if (!(type & LEFT)) while (len < size--) *str++ = ' ';
    for (i = 0; i < len; ++i) *str++ = tmp[i];
    while (len < size--) *str++ = ' ';

    return str;
}

#ifndef NOFLOAT

char *ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
char *fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);

static void cfltcvt(double value, char *buffer, char fmt, int precision)
{
  int decpt, sign, exp, pos;
  char *digits = NULL;
  char cvtbuf[80];
  int capexp = 0;
  int magnitude;

  if (fmt == 'G' || fmt == 'E')
  {
    capexp = 1;
    fmt += 'a' - 'A';
  }

  if (fmt == 'g')
  {
    digits = ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
    magnitude = decpt - 1;
    if (magnitude < -4  ||  magnitude > precision - 1)
    {
      fmt = 'e';
      precision -= 1;
    }
    else
    {
      fmt = 'f';
      precision -= decpt;
    }
  }

  if (fmt == 'e')
  {
    digits = ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);

    if (sign) *buffer++ = '-';
    *buffer++ = *digits;
    if (precision > 0) *buffer++ = '.';
    memcpy(buffer, digits + 1, precision);
    buffer += precision;
    *buffer++ = capexp ? 'E' : 'e';

    if (decpt == 0)
    {
      if (value == 0.0)
        exp = 0;
      else
        exp = -1;
    }
    else
      exp = decpt - 1;

    if (exp < 0)
    {
      *buffer++ = '-';
      exp = -exp;
    }
    else
      *buffer++ = '+';

    buffer[2] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[1] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[0] = (exp % 10) + '0';
    buffer += 3;
  }
  else if (fmt == 'f')
  {
    digits = fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
    if (sign) *buffer++ = '-';
    if (*digits)
    {
      if (decpt <= 0)
      {
        *buffer++ = '0';
        *buffer++ = '.';
        for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
        while (*digits) *buffer++ = *digits++;
      }
      else
      {
        pos = 0;
        while (*digits)
        {
          if (pos++ == decpt) *buffer++ = '.';
          *buffer++ = *digits++;
        }
      }
    }
    else
    {
      *buffer++ = '0';
      if (precision > 0)
      {
        *buffer++ = '.';
        for (pos = 0; pos < precision; pos++) *buffer++ = '0';
      }
    }
  }

  *buffer = '\0';
}

static void forcdecpt(char *buffer)
{
  while (*buffer)
  {
    if (*buffer == '.') return;
    if (*buffer == 'e' || *buffer == 'E') break;
    buffer++;
  }

  if (*buffer)
  {
    int n = strlen(buffer);
    while (n > 0) 
    {
      buffer[n + 1] = buffer[n];
      n--;
    }

    *buffer = '.';
  }
  else
  {
    *buffer++ = '.';
    *buffer = '\0';
  }
}

static void cropzeros(char *buffer)
{
  char *stop;

  while (*buffer && *buffer != '.') buffer++;
  if (*buffer++)
  {
    while (*buffer && *buffer != 'e' && *buffer != 'E') buffer++;
    stop = buffer--;
    while (*buffer == '0') buffer--;
    if (*buffer == '.') buffer--;
    while ((*++buffer = *stop++));
  }
}

static char *flt(char *str, double num, int size, int precision, char fmt, int flags)
{
  char tmp[80];
  char c, sign;
  int n, i;

  // Left align means no zero padding
  if (flags & LEFT) flags &= ~ZEROPAD;

  // Determine padding and sign char
  c = (flags & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (flags & SIGN)
  {
    if (num < 0.0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if (flags & PLUS)
    {
      sign = '+';
      size--;
    }
    else if (flags & SPACE)
    {
      sign = ' ';
      size--;
    }
  }

  // Compute the precision value
  if (precision < 0)
    precision = 6; // Default precision: 6
  else if (precision == 0 && fmt == 'g')
    precision = 1; // ANSI specified

  // Convert floating point number to text
  cfltcvt(num, tmp, fmt, precision);

  // '#' and precision == 0 means force a decimal point
  if ((flags & SPECIAL) && precision == 0) forcdecpt(tmp);

  // 'g' format means crop zero unless '#' given
  if (fmt == 'g' && !(flags & SPECIAL)) cropzeros(tmp);

  n = strlen(tmp);

  // Output number with alignment and padding
  size -= n;
  if (!(flags & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
  if (sign) *str++ = sign;
  if (!(flags & LEFT)) while (size-- > 0) *str++ = c;
  for (i = 0; i < n; i++) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';

  return str;
}

#endif

int vsprintf(char * str, const char * fmt, va_list args)
{
    int base;
    char * tmp;
    char * s;

    int flags;            // Flags to number()

    int qualifier;        // 'h', 'l', or 'L' for integer fields

    for (tmp = str; *fmt; fmt++)
    {
        if (*fmt != '%')
        {
            *tmp++ = *fmt;
            continue;
        }

        // Process flags
        flags = 0;
        repeat:
        fmt++; // This also skips first '%'
        switch (*fmt)
        {
            case '-':
                flags |= LEFT;
                goto repeat;
            case '+':
                flags |= PLUS;
                goto repeat;
            case ' ':
                flags |= SPACE;
                goto repeat;
            case '#':
                flags |= SPECIAL;
                goto repeat;
            case '0':
                flags |= ZEROPAD;
                goto repeat;
        }

        // --------------------------------------------------------------------
        // Get the width of the output field.
        int32_t field_width;
        field_width = -1;
        if (isdigit(*fmt))
        {
            field_width = skip_atoi(&fmt);
        }
        else if (*fmt == '*')
        {
            fmt++;
            field_width = va_arg(args, int32_t);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        // --------------------------------------------------------------------
        // Get the precision, thus the minimum number of digits for
        // integers; max number of chars for from string.
        int32_t precision = -1;
        if (*fmt == '.')
        {
            ++fmt;
            if (isdigit(*fmt))
            {
                precision = skip_atoi(&fmt);
            }
            else if (*fmt == '*')
            {
                ++fmt;
                precision = va_arg(args, int);
            }
            if (precision < 0)
            {
                precision = 0;
            }
        }

        // Get the conversion qualifier
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
        {
            qualifier = *fmt;
            fmt++;
        }

        // Default base
        base = 10;

        switch (*fmt)
        {
            case 'c':
                if (!(flags & LEFT)) while (--field_width > 0) *tmp++ = ' ';
                *tmp++ = va_arg(args, char);
                while (--field_width > 0) *tmp++ = ' ';
                continue;

            case 's':
                s = va_arg(args, char *);
                if (!s)
                {
                    s = "<NULL>";
                }
                int32_t len = (int32_t) strnlen(s, (uint32_t) precision);
                if (!(flags & LEFT))
                {
                    while (len < field_width--)
                    {
                        *tmp++ = ' ';
                    }
                }
                int32_t it;
                for (it = 0; it < len; ++it)
                {
                    *tmp++ = *s++;
                }
                while (len < field_width--)
                {
                    *tmp++ = ' ';
                }
                continue;

            case 'p':
                if (field_width == -1)
                {
                    field_width = 2 * sizeof(void *);
                    flags |= ZEROPAD;
                }
                tmp = number(tmp,
                             (unsigned long) va_arg(args, void *),
                             16,
                             field_width,
                             precision,
                             flags);
                continue;

            case 'n':
                if (qualifier == 'l')
                {
                    long * ip = va_arg(args, long *);
                    *ip = (tmp - str);
                }
                else
                {
                    int * ip = va_arg(args, int *);
                    *ip = (tmp - str);
                }
                continue;

            case 'A':
                flags |= LARGE;

            case 'a':
                if (qualifier == 'l')
                    tmp = eaddr(tmp, va_arg(args, unsigned char *), field_width,
                                precision, flags);
                else
                    tmp = iaddr(tmp, va_arg(args, unsigned char *), field_width,
                                precision, flags);
                continue;

                // Integer number formats - set up the flags and "break"
            case 'o':
                base = 8;
                break;

            case 'X':
                flags |= LARGE;

            case 'x':
                base = 16;
                break;

            case 'd':
            case 'i':
                flags |= SIGN;

            case 'u':
                break;

#ifndef NOFLOAT

            case 'E':
            case 'G':
            case 'e':
            case 'f':
            case 'g':
              tmp = flt(tmp, va_arg(args, double), field_width, precision, *fmt, flags | SIGN);
              continue;

#endif

            default:
                if (*fmt != '%') *tmp++ = '%';
                if (*fmt)
                    *tmp++ = *fmt;
                else
                    --fmt;
                continue;
        }

        if (flags & SIGN)
        {
            long num;
            if (qualifier == 'l')
            {
                num = va_arg(args, long);
            }
            else if (qualifier == 'h')
            {
                num = va_arg(args, short);
            }
            else
            {
                num = va_arg(args, int);
            }
            tmp = number(tmp,
                         num,
                         base,
                         field_width,
                         precision,
                         flags);
        }
        else
        {
            unsigned long num;
            if (qualifier == 'l')
            {
                num = va_arg(args, unsigned long);
            }
            else if (qualifier == 'h')
            {
                num = va_arg(args, unsigned short);
            }
            else
            {
                num = va_arg(args, unsigned int);
            }
            tmp = number(tmp,
                         num,
                         base,
                         field_width,
                         precision,
                         flags);
        }
    }

    *tmp = '\0';
    return tmp - str;
}

int sprintf(char * str, const char * fmt, ...)
{
    va_list args;
    int n;

    va_start(args, fmt);
    n = vsprintf(str, fmt, args);
    va_end(args);

    return n;
}
