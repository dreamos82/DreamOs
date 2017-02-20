/***************************************************************************
 *            stdio.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Stdio library
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

#include <stdio.h>
#include <video.h>
#include <ctype.h>
#include <string.h>
#include <keyboard.h>
//#include <debug.h>

#define LEFT 1
#define RIGHT 0

char * VIDEO_RAM = (char *) 0xb8000, * VIDEO_PTRZ = (char *) 0xb8000, VIDEO_CLRZ = 0x7;
int last_Xz = 0, last_Yz = 0;

/* 
 *  puts() function
 *  print *s
 *  do newline
 */
int puts(char * s)
{
    while (*s != 0)
    {
        if (last_Xz && last_Yz) _kscrolldown();
        *VIDEO_PTRZ++ = *s;
        *VIDEO_PTRZ++ = VIDEO_CLRZ;
        _kshiftAll();
        _ksetcursauto();
        s++;
    }
    _knewline();
    return 1;
}

/*
 * Print a character
 */

void putchar(char ch)
{
    /*asm (
        "movl $0x0, %%eax\n"
        "movl %0, %%ecx\n"
        "int $80\n\t"
        ::"g" (ch) );*/
    /*char s[2];
    s[0] = ch;
    s[1] = '\0';
    */
    //_kputs(s);
    _kputc(ch);

}


/*
 * Convert a string to an integer
 */
int atoi(const char * nptr)
{
    int base = 1;
    int res = 0;
    int i;

    /* Make sure all chars are numbers */
    for (i = 0; *(nptr + i); ++i)
    {
        if (!isdigit(*(nptr + i)))
            return -1;
    }

    i = 0;
    while (nptr[++i])
        base *= 10;

    /* Actual conversion. It works like this: for example, 123 is obtained with
     1*100 + 2*10 + 3*1 */
    for (i = 0; *(nptr + i); ++i)
    {
        res += ((int) nptr[i] - 48) * base;
        base /= 10;
    }

    return res;
}

/*
 * No words...
 */
int printf(const char * format, ...)
{
    va_list ap;
    int len = 0;

    /* Start variabile argument's list */
    va_start (ap, format);
    char buffer[1024];

    len = vsprintf(buffer, format, ap);
    _kputs(buffer);
    va_end (ap); // end of arguments

    return len;
}

int getchar(void)
{
    int tmpchar;

    while ((tmpchar = _kgetch()) == -1);
    return tmpchar;
}

char * gets(char * s)
{
    char str[255];
    int c;
    int count = 0;
    shell_mess_col = _kgetcolumn();
    shell_mess_line = _kgetline();
    memset(str, '\0', 255);
    do
    {
        c = getchar();
        if (c == '\n') // tasto invio
            break;
        else if (c == '\b') //backspace
        {
            if (count > 0)
                count--;
        }
        else
            str[count++] = c;
    } while (count < 255);

    str[count] = '\0';
    //str cant simply be returned, it is allocated in this stack frame and it will be lost!
    strcpy(s, str);
    return s;
}

int scanf(const char * format, ...)
{
    va_list scan;
    char input[255];
    int count = 0;
    char maxchars[5] = {0};
    int i = 0, nmax = 0;

    char * s_ptr;
    int * i_ptr;

    va_start (scan, format);

    for (; *format; format++)
    {

        if (*format == '%')
        {
            gets(input);

            count += strlen(input);

            if (isdigit(*++format))
            {
                while (isdigit(*format))
                {
                    maxchars[i++] = *format;
                    format++;
                }
                maxchars[i] = '\0';
                nmax = atoi(maxchars);
            }

            switch (*format)
            {
                case 's':
                    s_ptr = va_arg (scan, char *);
                    if (nmax == 0 || strlen(input) <= nmax)
                        s_ptr = strncpy(s_ptr, input, strlen(input));
                    else
                        s_ptr = strncpy(s_ptr, input, nmax);
                    break;

                case 'd':
                    i_ptr = va_arg (scan, int *);

                    if (nmax != 0 && strlen(input) > nmax)
                        input[nmax] = '\0';
                    *i_ptr = atoi(input);
                    break;
            }
        }
    }
    va_end (scan);
    return count;
}
