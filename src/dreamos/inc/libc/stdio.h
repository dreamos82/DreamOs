/***************************************************************************
 *            stdio.h
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Standard I/0 header
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

#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>

#define MAX_DIGITS_IN_INTEGER 11

#ifndef EOF
#define EOF (-1)
#endif

/// @brief Writes the given character to the standard output (stdout).
void putchar(char character);

/// @brief Returns the next character from the standard input (stdin).
int getchar(void);

/// @brief Writes the string pointed by str to the standard output (stdout)
/// and appends a newline character ('\n').
int puts(char * str);

/// @brief Reads characters from the standard input (stdin) and stores them
/// as a C string into str until a newline character or the end-of-file is
/// reached.
char * gets(char * str);

/// @brief Convert the given string to an integer.
int atoi(const char * str);

/// @brief Write formatted output to stdout.
int printf(const char *, ...);

/// @brief Read formatted input from stdin.
int scanf(const char *, ...);

/// @brief Write formatted output to the string str from argument list ARG.
int vsprintf(char * str, const char * fmt, va_list args);

/// @brief Write formatted output to the string str.
int sprintf(char * str, const char * fmt, ...);

#endif
