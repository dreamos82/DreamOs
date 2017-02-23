/// @file   limits.h

#ifndef _LIBC_LIMITS_H_
#define _LIBC_LIMITS_H_

/// Number of bits in a `char'.
#define CHAR_BIT	8

/// Minimum and maximum values a `signed char' can hold.
#define SCHAR_MIN	(-128)
#define SCHAR_MAX	127

/// Maximum value a `char' can hold.  (Minimum is 0.)
#define CHAR_MAX	255

/// Minimum and maximum values a `signed short int' can hold.
#define SHRT_MIN	(-32768)
#define SHRT_MAX	32767

/// Minimum and maximum values a `signed int' can hold.
#define INT_MIN	(-INT_MAX - 1)
#define INT_MAX	2147483647

/// Maximum value an `unsigned int' can hold.  (Minimum is 0.)
#define UINT_MAX	4294967295U

/// Minimum and maximum values a `signed long int' can hold.
#define LONG_MAX    2147483647L
#define LONG_MIN    (-LONG_MAX - 1L)

#endif
