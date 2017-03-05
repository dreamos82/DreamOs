/// @file   spinlock.h

#pragma once

#include <stdatomic.h>
#include <stddef.h>
#include <scheduler.h>
#include <debug.h>

/// @brief Spinlock structure.
typedef struct spinlock
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

inline static void spinlock_init(spinlock_t * spinlock)
{
    dbg_print("Initialize spinlock...\n");
    atomic_set(&spinlock->queue, 0);
    atomic_set(&spinlock->dequeue, 1);
    spinlock->owner = MAX_THREADS;
    spinlock->counter = 0;
}

inline static int spinlock_lock(spinlock_t * spinlock)
{
    thread_t * current_thread = kernel_get_current_thread();
    dbg_print("[%d] Trying to lock...\n", current_thread->id);
    int32_t ticket;
    if (spinlock->owner == current_thread->id)
    {
        spinlock->counter++;
        return 0;
    }
    ticket = atomic_add(&spinlock->queue, 1);
    while (atomic_read(&spinlock->dequeue) != ticket)
    {
        __asm__("nop");
    }
    spinlock->owner = current_thread->id;
    spinlock->counter = 1;
    dbg_print("[%d] Acquired lock...\n", current_thread->id);
    return 0;
}

inline static int spinlock_unlock(spinlock_t * spinlock)
{
    spinlock->counter--;
    if (!spinlock->counter)
    {
        spinlock->owner = MAX_THREADS;
        atomic_inc(&spinlock->dequeue);
    }
    thread_t * current_thread = kernel_get_current_thread();
    dbg_print("[%d] Unlock spinlock...\n", current_thread->id);
    return 0;
}