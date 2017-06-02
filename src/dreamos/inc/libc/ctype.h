/// @file   ctype.h
/// @brief  Functions related to character handling.
/// @author shainer <shainer@debianclan.org>
/// @date   Jan 26 2006
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

/// @brief Check if the given value is a digit.
int isdigit(int c);

/// @brief Check if the given value is a letter.
int isalpha(int c);

/// @brief Check if the given value is either a letter or a digit.
int isalnum(int c);

/// @brief Check if the given value is an hexadecimal digit.
int isxdigit(int c);

/// @brief Check if the given value is a lower case letter.
int islower(int c);

/// @brief Check if the given value is an upper case letter.
int isupper(int c);

/// @brief Transforms the given value into a lower case letter.
int tolower(int c);

/// @brief Transforms the given value into an upper case letter.
int toupper(int c);

/// @brief Check if the given value is a whitespace.
int isspace(int c);
