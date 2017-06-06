/// @file   string.h
/// @brief  String routines.
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

#pragma once

#include "stddef.h"
#include "list.h"

/// @brief Copies the first num characters of source to destination.
char * strncpy(char * destination, const char * source, size_t num);

/// @brief Compares up to n characters of s1 to those of s2.
int strncmp(const char * s1, const char * s2, size_t n);

/// @brief Case insensitive string compare.
int stricmp(const char * s1, const char * s2);

/// @brief Case-insensitively compare up to n characters of s1 to those of s2.
int strnicmp(const char * s1, const char * s2, size_t n);

/// @brief Returns a pointer to the first occurrence of ch in str.
char * strchr(const char * s, int ch);

/// @brief Returns a pointer to the last occurrence of ch in str.
char * strrchr(const char * s, int ch);

/// @brief Returns a pointer to the first occurrence of s2 in s1, or NULL if
/// s2 is not part of s1.
char * strstr(const char * s1, const char * s2);

/// @brief Returns the length of the initial portion of string which consists
/// only of characters that are part of control.
size_t strspn(const char * string, const char * control);

/// @brief Calculates the length of the initial segment of string which
/// consists entirely of characters not in control.
size_t strcspn(const char * string, const char * control);

/// @brief Finds the first character in the string string that matches any
/// character specified in control.
char * strpbrk(const char * string, const char * control);

/// @brief Make a copy of the given string.
char * strdup(const char * s);

/// @brief Appends the string pointed to, by s2 to the end of the string
/// pointed to, by s1 up to n characters long.
char * strncat(char * s1, const char * s2, size_t n);

char * strnset(char * s, int c, size_t n);

char * strset(char * s, int c);

char * strrev(char * s);

/// @brief Breaks string str into a series of tokens separated by delim.
char * strtok(char * str, const char * delim);

char * strtok_r(char * string, const char * control, char ** lasts);

/// @brief Another function to copy n characters from str2 to str1.
void * memmove(void * dst, const void * src, size_t n);

/// @brief Searches for the first occurrence of the character c (an unsigned
/// char) in the first n bytes of the string pointed to, by the argument str.
void * memchr(const void * str, int c, size_t n);

void * memccpy(void * dst, const void * src, int c, size_t n);

// Intrinsic functions

/// @brief Copy a block of memory, handling overlap.
/// @param _dst Pointer to the destination.
/// @param _src Pointer to the source.
/// @param num Number of bytes to be copied.
/// @return Pointer to the destination.
void * memcpy(void * _dst, const void * _src, size_t num);

/// @brief Compares the first n bytes of str1 and str2.
int memcmp(const void * str1, const void * str2, size_t n);

/// @brief Sets the first num bytes of the block of memory pointed by ptr
///         to the specified value.
/// @param ptr   Pointer to the block of memory to set.
/// @param value Value to be set.
/// @param num   Number of bytes to be set to the given value.
/// @return The same ptr.
void * memset(void * ptr, int value, size_t num);

char * strcpy(char *, const char *);

char * strcat(char *, const char *);

int strcmp(const char *, const char *);

size_t strlen(const char *);

size_t strnlen(const char * s, size_t count);

// Written by shainer

char * strtok(char *, const char *);

int _kstrncmp(const char * s1, const char * s2, size_t num);

char * trim(char * str);

char * strdup(const char * src);

char * strsep(char ** stringp, const char * delim);

list_t *
str_split(const char * str, const char * delim, unsigned int * numtokens);

char * list2str(list_t * list, const char * delim);

/// @brief Move the number "num" into a string.
/// @author Lisa
/// @version 1.0
/// @param buffer   The string containing the number.
/// @param num      The number to convert.
/// @param base     The base used to convert.
void int_to_str(char * buffer, unsigned int num, unsigned int base);

void _knntos(char *, int, int);

char * replace_char(char * str, char find, char replace);
