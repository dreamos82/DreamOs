/// @file   stdatomic.h

#pragma once

#include "compiler.h"
#include "stdint.h"
#include "stdbool.h"

/// @brief Standard structure for atomic operations.
typedef struct atomic_t
{
    /// @brief The counter associated to the atomic.
    volatile int32_t counter;
} atomic_t;

/// @brief At declaration, initialize an atomic_t to i.
#define ATOMIC_INIT(i)  { (i) }

/// @brief The prefix used to lock.
#define LOCK_PREFIX "\n\tlock; "

inline static int32_t atomic_set_and_test(atomic_t * v, int32_t i)
{
    __asm__ __volatile__("xchgl %0, %1" : "=r"(i) : "m"(v->counter), "0"(i) : "memory");
    return i;
}

/// @brief Atomically set v equal to i
inline static void atomic_set(atomic_t * v, int32_t i)
{
    atomic_set_and_test(v, i);
}

/// @brief Atomically read the integer value of v.
inline static int32_t atomic_read(const atomic_t * v)
{
    return READ_ONCE((v)->counter);
}

/// Atomically add i to v
inline static int32_t atomic_add(atomic_t * v, int32_t i)
{
    int result = i;
    asm volatile(LOCK_PREFIX "xaddl %0, %1" : "=r"(i) : "m"(v->counter), "0"(i) : "memory", "cc");
    return result + i;
}

/// Atomically subtract i from v.
inline static int32_t atomic_sub(atomic_t * v, int32_t i)
{
    return atomic_add(v, -i);
}

/// Atomically add one to v
inline static int32_t atomic_inc(atomic_t * v)
{
    return atomic_add(v, 1);
}

/// Atomically subtract one from v
inline static int32_t atomic_dec(atomic_t * v)
{
    return atomic_add(v, -1);
}

/// @brief Atomically add i to v and return true if the result is negative;
/// otherwise false.
inline static bool_t atomic_add_negative(atomic_t * v, int32_t i)
{
    return (bool_t) (atomic_add(v, i) < 0);
}

/// @brief Atomically subtract i from v and return true if the result is
/// zero; otherwise false.
inline static bool_t atomic_sub_and_test(atomic_t * v, int32_t i)
{
    return (bool_t) (atomic_sub(v, i) == 0);
}

/// Atomically increment v by one and return true if the result is zero;
/// false otherwise.
inline static int32_t atomic_inc_and_test(atomic_t * v)
{
    return (bool_t) (atomic_inc(v) == 0);
}

/// @brief Atomically decrement v by one and return true if zero; false
/// otherwise.
inline static int32_t atomic_dec_and_test(atomic_t * v)
{
    return (bool_t) (atomic_dec(v) == 0);
}
