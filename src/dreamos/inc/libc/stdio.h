/// @file   stdio.h
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

#pragma once

#include "stdarg.h"
#include "stddef.h"

/// The maximum number of digits of an integer.
#define MAX_DIGITS_IN_INTEGER 11

#ifndef EOF
/// Define the End-Of-File.
#define EOF (-1)
#endif

/// @brief Writes the given character to the standard output (stdout).
void putchar(int character);

/// @brief Writes the string pointed by str to the standard output (stdout)
/// and appends a newline character ('\n').
void puts(char * str);

/// @brief Returns the next character from the standard input (stdin).
int getchar(void);

/// @brief Reads characters from the standard input (stdin) and stores them
/// as a C string into str until a newline character or the end-of-file is
/// reached.
char * gets(char * str);

/// @brief Convert the given string to an integer.
int atoi(const char * str);

/// @brief Write formatted output to stdout.
int printf(const char *, ...);

/// @brief Read formatted input from stdin.
size_t scanf(const char *, ...);

/// @brief Write formatted output to the string str from argument list ARG.
int vsprintf(char * str, const char * fmt, va_list args);

/// @brief Write formatted output to the string str.
int sprintf(char * str, const char * fmt, ...);
