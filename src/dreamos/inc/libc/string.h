//
// string.h
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

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <list.h>

char * strncpy(char * dest, const char * source, size_t n);

int strncmp(const char * s1, const char * s2, size_t n);

int stricmp(const char * s1, const char * s2);

int strnicmp(const char * s1, const char * s2, size_t n);

char * strchr(const char * s, int ch);

char * strrchr(const char * s, int ch);

char * strstr(const char * s1, const char * s2);

size_t strspn(const char * string, const char * control);

size_t strcspn(const char * string, const char * control);

char * strpbrk(const char * string, const char * control);

int strcasecmp(const char * s1, const char * s2);

int strncasecmp(const char * s1, const char * s2, size_t n);

int strcoll(const char * s1, const char * s2);

char * strdup(const char * s);

char * strlwr(char * s);

char * strupr(char * s);

char * strncat(char * s1, const char * s2, size_t n);

char * strnset(char * s, int c, size_t n);

char * strset(char * s, int c);

char * strrev(char * s);

char * strtok(char * string, const char * control);

char * strtok_r(char * string, const char * control, char ** lasts);

char * strerror(int errnum);

char * strsignal(int signum);

void * memmove(void * dst, const void * src, size_t n);

void * memchr(const void * buf, int ch, size_t n);

void * memccpy(void * dst, const void * src, int c, size_t n);

int memicmp(const void * buf1, const void * buf2, size_t n);

// Intrinsic functions

/// @brief Copy a block of memory, handling overlap.
/// @param _dst Pointer to the destination.
/// @param _src Pointer to the source.
/// @param num Number of bytes to be copied.
/// @return Pointer to the destination.
void * memcpy(void * _dst, const void * _src, size_t num);

int memcmp(const void *, const void *, size_t);

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

list_t * str_split(const char * str, const char * delim, unsigned int * numtokens);

char * list2str(list_t * list, const char * delim);

/// @brief Move the number "num" into a string.
/// @author Lisa
/// @version 1.0
/// @param buffer   The string containing the number.
/// @param num      The number to convert.
/// @param base     The base used to convert.
void int_to_str(char * buffer, unsigned int num, unsigned int base);

void _knntos(char *, int, int);

#endif
