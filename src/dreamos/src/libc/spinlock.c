/// @file   spinlock.c

#include "spinlock.h"

bool_t spinlock_init(spinlock_t * spinlock)
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

bool_t spinlock_destroy(spinlock_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    spinlock->owner = MAX_THREADS;
    spinlock->counter = 0;
    return true;
}


bool_t spinlock_lock(spinlock_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    process_t * current_process = kernel_get_current_process();
    dbg_print("[%d] Trying to lock...\n", current_process->id);
    int32_t ticket;
    if (spinlock->owner == current_process->id)
    {
        spinlock->counter++;
        return true;
    }
    ticket = atomic_add(&spinlock->queue, 1);
    while (atomic_read(&spinlock->dequeue) != ticket)
    {
        __asm__("nop");
    }
    spinlock->owner = current_process->id;
    spinlock->counter = 1;
    dbg_print("[%d] Acquired lock...\n", current_process->id);
    return true;
}

bool_t spinlock_unlock(spinlock_t * spinlock)
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
    process_t * current_process = kernel_get_current_process();
    dbg_print("[%d] Unlock spinlock...\n", current_process->id);
    return true;
}

bool_t spinlock_irq_save_init(spinlock_irq_save_t * spinlock)
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

bool_t spinlock_irq_save_destroy(spinlock_irq_save_t * spinlock)
{
    if (!spinlock)
    {
        return false;
    }
    spinlock->flags = 0;
    spinlock->counter = 0;
    return true;
}

bool_t spinlock_irq_save_lock(spinlock_irq_save_t * spinlock)
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

bool_t spinlock_irq_save_unlock(spinlock_irq_save_t * spinlock)
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
    return true;
}
