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
#include <stdarg.h>
#include <video.h>
#include <ctype.h>
#include <string.h>
#include <keyboard.h>
#include <kheap.h>
#include <debug.h>

#define LEFT 1
#define RIGHT 0

/*
 * Print a character
 */
void putchar (char ch)
{
   asm ("movl $0x0, %%eax\n"
	"int $80\n\t"
	:: "b" (ch));
}

/*
 * Convert a string to an integer
 */
int atoi(const char *nptr)
{
  int base=1;
  int res=0;
  int i;

  /* Make sure all chars are numbers */
  for (i=0; *(nptr+i); ++i) {
    if(!isdigit(*(nptr+i)))
      return -1;
  }

  i=0;
  while(nptr[++i])
    base *= 10;

  /* Actual conversion. It works like this: for example, 123 is obtained with
   1*100 + 2*10 + 3*1 */
  for (i=0; *(nptr+i); ++i ) {
    res += ((int)nptr[i] - 48) * base;
    base /= 10;
  }

  return res;
}

/*
 * No words...
 */
int printf (const char *format, ...)
{
    va_list ap;
    char current[255];
    char *cur_p = &current[0];
    int cursize = 0;

    short int direction=RIGHT;
    short int width=0;
    char field[5];
    int len=0;
    int i=0;

    /* Start variabile argument's list */
    va_start (ap, format);

    while (*format) {

        /* % marks a specific */
        if (*format == '%') {
            format++;

            /* The field width is to be written to left */
            if (*format == '-') {
                direction=LEFT;
                format++;
            }

            /* Take field width if present and convert it to an int */
            if (isdigit (*format) == 1) { // precisione di campo
                while (isdigit(*format))
                    field[i++] = *format++;
                field[i] = '\0';
                width = atoi (field);
            }

            /* Some data type aren't supported yet
             * In the future we will fix this */
            if (*format == 's') {
		cur_p = strcpy(cur_p, va_arg (ap, char *));
            }
            if (*format == 'd' || *format == 'i') {
                int varint = va_arg (ap, int);
                _kntos (cur_p, varint, 10);
            }
            if (*format == 'u') {
                unsigned int uint = va_arg (ap, int);
                _kntos (cur_p, uint, 10);
            }
            if (*format == 'x' || *format == 'X') {
                int varhex = va_arg (ap, int);
                _kntos (cur_p, varhex, 16);
            }
            if (*format == 'n')
                _kntos (cur_p, len, 10);
            if (*format == '%')
                putchar ('%');

            cursize = strlen(cur_p);

            /* Actually write the field width */
            if (direction==LEFT) {
                for(i=1; i<=width; i++)
                    _kbackspace();
            } else {
            for (i=1; i<=width; i++)
                putchar (' ');
            }

            /* Print the argument converted to a string */
            *(cur_p + cursize) = '\0';
            _kputs (cur_p);
        }
        else
            putchar (*format);

        /* Update values */
        format++;
        direction=RIGHT;
        width=0;
        len++;
        for(i=0; i<255; i++)
	  current[i] = 0;
    }
    va_end (ap); // end of arguments

    return len;
}

int getchar (void)
{
    int tmpchar;

    while ((tmpchar = _kgetch()) == -1);
    return tmpchar;
}

char *gets ()
{
    char *str=0;
    int c;
    int count=0;
    shell_mess_col = _kgetcolumn ();
    shell_mess_line = _kgetline ();

    do {
	c = getchar();
	if (c=='\n') // tasto invio
	    break;
	else if (c=='\b') //backspace
	{
	    if (count>0)
                count--;
	}
	else
	    str[count++] = c;
    } while (count<255);

    str[count] = '\0';
    return str;
}

int scanf (const char *format, ...)
{
    va_list scan;
    char *input;
    int count=0;
    char maxchars[5] = {0};
    int i=0, nmax=0;

    char *s_ptr;
    int *i_ptr;

    va_start (scan, format);

    for (; *format; format++) {

	if (*format == '%') {
	    input = gets();
            count += strlen (input);

            if (isdigit(*++format)) {
              while (isdigit(*format)) {
                maxchars[i++] = *format;
                format++;
              }
              maxchars[i] = '\0';
              nmax = atoi(maxchars);
            }

	    switch (*format) {
	    case 's':
		s_ptr = va_arg (scan, char *);

                if (nmax == 0 || strlen(input) <= nmax)
		  s_ptr = strncpy (s_ptr, input, strlen (input));
                else
                  s_ptr = strncpy (s_ptr, input, nmax);
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
