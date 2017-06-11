/// @file   stdarg.h

#ifndef LIBCSTDARGH
#define    LIBCSTDARGH

/// @brief The va_list type is an array containing a single element of one
/// structure containing the necessary information to implement the va_arg
/// macro.
typedef char * va_list;

/// @brief The type of the item on the stack.
#define    STACKITEM    int

/// @brief The size of a variadic list.
#define    VA_SIZE(TYPE)                    \
    ((sizeof(TYPE) + sizeof(STACKITEM) - 1)    \
        & ~(sizeof(STACKITEM) - 1))

/// @brief The start of a variadic list.
#define    va_start(AP, LASTARG)    \
    (AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

/// @brief The end of a variadic list.
#define va_end(AP)

/// @brief The argument of a variadic list.
#define va_arg(AP, TYPE)    \
    (AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

#endif
