/// @file   stdio.c
/// @brief  Standard I/0 functions.
/// @author shainer <shainer@debianclan.org>
/// @date   Mar 31 2007
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdio.h"
#include "video.h"
#include "ctype.h"
#include "string.h"
#include "keyboard.h"

#define LEFT    1
#define RIGHT   0

char * VIDEO_RAM = (char *) 0xb8000;
char * VIDEO_PTRZ = (char *) 0xb8000;
char VIDEO_CLRZ = 0x7;

int last_Xz = 0;
int last_Yz = 0;

void putchar(int character)
{
    video_putc(character);
}

int getchar(void)
{
    int tmpchar;
    while ((tmpchar = keyboard_getc()) == -1);
    return tmpchar;
}

int puts(char * str)
{
    while ((*str) != 0)
    {
        if (last_Xz && last_Yz)
        {
            video_scroll_down();
        }
        *(VIDEO_PTRZ++) = (*str);
        *(VIDEO_PTRZ++) = VIDEO_CLRZ;
        video_shift_one_line();
        video_set_cursor_auto();
        ++str;
    }
    video_new_line();
    return 1;
}

char * gets(char * str)
{
    int count = 0;
    //shell_mess_col = video_get_column();
    //shell_mess_line = video_get_line();
    char tmp[255];
    memset(tmp, '\0', 255);
    do
    {
        int c = getchar();
        // Return Key
        if (c == '\n')
        {
            break;
        }
        // Backspace key
        if (c == '\b')
        {
            if (count > 0)
            {
                count--;
            }
        }
        else
        {
            tmp[count++] = (char) c;
        }
    } while (count < 255);
    tmp[count] = '\0';
    // tmp cant simply be returned, it is allocated in this stack frame and
    // it will be lost!
    strcpy(str, tmp);
    return str;
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
        if (!isdigit(str[i]))
        {
            return -1;
        }
    }
    // Iterate through all digits and update the result.
    for (i = (sign == -1) ? 1 : 0; str[i] != '\0'; ++i)
    {
        result = (result * 10) + str[i] - '0';
    }
    return sign * result;
}

int printf(const char * format, ...)
{
    va_list ap;
    int len = 0;
    /* Start variabile argument's list */
    va_start (ap, format);
    char buffer[1024];
    len = vsprintf(buffer, format, ap);
    video_puts(buffer);
    va_end (ap); // end of arguments
    return len;
}

size_t scanf(const char * format, ...)
{
    size_t count = 0;
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
