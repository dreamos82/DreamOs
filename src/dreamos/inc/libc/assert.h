/// @file   assert.h

#pragma once

#include "stdarg.h"
#include "stdio.h"
#include "panic.h"

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

# define assert(expression) \
  ((expression) \
    ? (void) 0 : __assert_fail (#expression, __FILE__, __LINE__, __func__))
