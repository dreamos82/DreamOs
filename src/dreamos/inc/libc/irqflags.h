/// @file   irqflags.h

#pragma once

#include <stddef.h>
#include <stdint.h>

/// @brief Disable IRQs
inline static void irq_disable()
{
    __asm__ __volatile__("cli":: : "memory");
}

/// @brief Disable IRQs (nested)
/// @details
/// Disable IRQs when unsure if IRQs were enabled at all.
/// This function together with irq_nested_enable can be used
/// in situations when interrupts shouldn't be activated if they
/// were not activated before calling this function.
inline static uint8_t irq_nested_disable()
{
    size_t flags;
    __asm__ __volatile__("pushf; cli; pop %0": "=r"(flags) : : "memory");
    if (flags & (1 << 9)) return 1;
    return 0;
}

/// @brief Enable IRQs.
inline static void irq_enable()
{
    __asm__ __volatile__("sti":: : "memory");
}

/// @brief Enable IRQs (nested)
/// @details
/// If called after calling irq_nested_disable, this function will
/// not activate IRQs if they were not active before.
inline static void irq_nested_enable(uint8_t flags)
{
    if (flags)
    {
        irq_enable();
    }
}

/// @brief Determines, if the interrupt flags (IF) is set.
inline static uint8_t is_irq_enabled()
{
    size_t flags;
    __asm__ __volatile__("pushf; pop %0": "=r"(flags) : : "memory");
    if (flags & (1 << 9))
    {
        return 1;
    }
    return 0;
}
