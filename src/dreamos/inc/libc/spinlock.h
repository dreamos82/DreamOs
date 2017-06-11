/// @file   spinlock.h

#pragma once

#include "stdatomic.h"
#include "stddef.h"
#include "scheduler.h"
#include "debug.h"
#include "irqflags.h"

/// @brief Spinlock structure.
typedef struct spinlock_t
{
    /// Internal queue.
    atomic_t queue;
    /// Internal dequeue.
    atomic_t dequeue;
    /// Owner of this spinlock structure.
    pid_t owner;
    /// Internal counter var.
    uint32_t counter;
} spinlock_t;

/// @brief Initialie the spinlock.
bool_t spinlock_init(spinlock_t * spinlock);

/// @brief Destroy the spinlock and free the memory.
bool_t spinlock_destroy(spinlock_t * spinlock);

/// @brief Try to lock the spinlock.
bool_t spinlock_lock(spinlock_t * spinlock);

/// @brief Try to unlock the spinlock.
bool_t spinlock_unlock(spinlock_t * spinlock);

/// @brief Is basically used to save the interrupt state before taking the
/// spin lock, this is because spin lock disables the interrupt, when the
/// lock is taken in interrupt context, and re-enables it when while unlocking.
typedef struct spinlock_irq_save_t
{
    /// Internal queue.
    atomic_t queue;
    /// Internal dequeue.
    atomic_t dequeue;
    /// Internal counter var.
    uint32_t counter;
    /// Interrupt flag.
    uint8_t flags;
} spinlock_irq_save_t;

/// @brief Initialie the spinlock.
bool_t spinlock_irq_save_init(spinlock_irq_save_t * spinlock);

/// @brief Destroy the spinlock and free the memory.
bool_t spinlock_irq_save_destroy(spinlock_irq_save_t * spinlock);

/// @brief Try to lock the spinlock.
bool_t spinlock_irq_save_lock(spinlock_irq_save_t * spinlock);

/// @brief Try to unlock the spinlock.
bool_t spinlock_irq_save_unlock(spinlock_irq_save_t * spinlock);
