/// @file   spinlock.h

#pragma once

#include <stdatomic.h>
#include <stddef.h>
#include <scheduler.h>
#include <debug.h>
#include <irqflags.h>

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

inline static bool_t spinlock_init(spinlock_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    dbg_print("Initialize spinlock...\n");
    atomic_set(&spinlock->queue, 0);
    atomic_set(&spinlock->dequeue, 1);
    spinlock->owner = MAX_THREADS;
    spinlock->counter = 0;
    return true;
}

inline static bool_t spinlock_destroy(spinlock_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    spinlock->owner = MAX_THREADS;
    spinlock->counter = 0;
    return true;
}


inline static bool_t spinlock_lock(spinlock_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    thread_t * current_thread = kernel_get_current_thread();
    dbg_print("[%d] Trying to lock...\n", current_thread->id);
    int32_t ticket;
    if (spinlock->owner == current_thread->id)
    {
        spinlock->counter++;
        return true;
    }
    ticket = atomic_add(&spinlock->queue, 1);
    while (atomic_read(&spinlock->dequeue) != ticket)
    {
        __asm__("nop");
    }
    spinlock->owner = current_thread->id;
    spinlock->counter = 1;
    dbg_print("[%d] Acquired lock...\n", current_thread->id);
    return true;
}

inline static bool_t spinlock_unlock(spinlock_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    spinlock->counter--;
    if (!spinlock->counter)
    {
        spinlock->owner = MAX_THREADS;
        atomic_inc(&spinlock->dequeue);
    }
    thread_t * current_thread = kernel_get_current_thread();
    dbg_print("[%d] Unlock spinlock...\n", current_thread->id);
    return true;
}

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

inline static bool_t spinlock_irq_save_init(spinlock_irq_save_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    atomic_set(&spinlock->queue, 0);
    atomic_set(&spinlock->dequeue, 1);
    spinlock->flags = 0;
    spinlock->counter = 0;
    return true;
}

inline static bool_t spinlock_irq_save_destroy(spinlock_irq_save_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    spinlock->flags = 0;
    spinlock->counter = 0;
    return true;
}

inline static bool_t spinlock_irq_save_lock(spinlock_irq_save_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    int32_t ticket;
    uint8_t flags = irq_nested_disable();
    if (spinlock->counter == 1)
    {
        spinlock->counter++;
        return true;
    }
    ticket = atomic_add(&spinlock->queue, 1);
    while (atomic_read(&spinlock->dequeue) != ticket)
    {
        __asm__("nop");
    }
    spinlock->flags = flags;
    spinlock->counter = 1;
    return true;
}

inline static int spinlock_irq_save_unlock(spinlock_irq_save_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    uint8_t flags;
    spinlock->counter--;
    if (!spinlock->counter)
    {
        flags = spinlock->flags;
        spinlock->flags = 0;
        atomic_inc(&spinlock->dequeue);
        irq_nested_enable(flags);
    }
    return 0;
}
