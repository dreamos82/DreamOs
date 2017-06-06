/// @file   assert.h

#pragma once

#include "stdarg.h"
#include "stdio.h"
#include "panic.h"

/// @brief Function used to log the information of a failed assertion.
/// @param assertion    The failed assertion.
/// @param file         The file where the assertion is located.
/// @param line         The line inside the file.
/// @param function     The function where the assertion is.
static void __assert_fail(const char * assertion,
                          const char * file,
                          unsigned int line,
                          const char * function)
{
    char message[1024];
    sprintf(message,
            "%s:%d: %s: Assertion `%s' failed.\n",
            file,
            line,
            (function ? function : "NO_FUN"),
            assertion);
    kernel_panic(message);
}

/// @brief Assert function.
#define assert(expression) \
  ((expression) \
    ? (void) 0 : __assert_fail (#expression, __FILE__, __LINE__, __func__))
