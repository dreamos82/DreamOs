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
//#inc <debug.h>

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
    /*__asm__ (
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

int atoi(const char * str)
{
    // Initialize sign as positive.
    int sign = 1;
    // Initialize the result.
    int result = 0;
    // Initialize the index.
    int i = 0;
    // If the number is negative, then update the sign.
    if (str[0] == '-') sign = -1;
    // Check that the rest of the numbers are digits.
    for (i = (sign == -1) ? 1 : 0; str[i] != '\0'; ++i)
    {
        if (!isdigit(str[i])) return -1;
    }
    // Iterate through all digits and update the result.
    for (i = (sign == -1) ? 1 : 0; str[i] != '\0'; ++i)
    {
        result = (result * 10) + str[i] - '0';
    }
    return sign * result;
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
    int count = 0;
    va_list scan;
    va_start (scan, format);
    for (; *format; format++)
    {
        if (*format == '%')
        {
            // Declare an input string.
            char input[255];
            // Get the input string.
            gets(input);
            // Evaluate the length of the string.
            size_t input_length = strlen(input);
            // Add the length of the input to the counter.
            count += input_length;
            // Evaluate the maximum number of input characters.
            size_t max_chars = 0;
            if (isdigit(*++format))
            {
                char max_char_num[16];
                int i = 0;
                while (isdigit(*format))
                {
                    max_char_num[i++] = *format;
                    format++;
                }
                max_char_num[i] = '\0';
                int number = atoi(max_char_num);
                if (number > 0)
                {
                    max_chars = (size_t) number;
                }
            }
            char * s_ptr;
            int * d_ptr;
            switch (*format)
            {
                case 's':
                    s_ptr = va_arg(scan, char *);
                    if (max_chars == 0 || input_length <= max_chars)
                    {
                        s_ptr = strncpy(s_ptr, input, input_length);
                    }
                    else
                    {
                        s_ptr = strncpy(s_ptr, input, max_chars);
                    }
                    break;
                case 'd':
                    d_ptr = va_arg(scan, int *);
                    if (max_chars != 0 && input_length > max_chars)
                    {
                        input[max_chars] = '\0';
                    }
                    (*d_ptr) = atoi(input);
                    break;
                default:
                    break;
            }
        }
    }
    va_end(scan);
    return count;
}
