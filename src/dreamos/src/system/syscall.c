/// @file   syscall.c
/// @brief  System Call management functions.
/// @author Lisa Vitolo
/// @date   Nov 11 2007
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "syscall.h"
#include "video.h"
#include "irqflags.h"
#include "assert.h"
#include "kernel.h"

/// The total number of system calls.
#define SYSCALL_NUMBER 3

/// @brief The signature of a function call.
typedef void (* SystemCall)();

/// @brief The list of function call.
SystemCall syscalls[SYSCALL_NUMBER];

/// @brief Wrapper for video_clear.
void syscall_video_clear()
{
    video_clear();
}

/// @brief Wrapper for video_putc.
void syscall_video_putc(int * args)
{
    video_putc((char) args[0]);
}

/// @brief Wrapper for video_puts.
void syscall_video_puts(int * args)
{
    video_puts((const char *) args[0]);
}

void syscall_init()
{
    // Initialize the list of function calls.
    for (uint32_t it = 0; it < SYSCALL_NUMBER; ++it)
    {
        syscalls[it] = NULL;
    }
    // Set the function calls.
    syscalls[0] = syscall_video_clear;
    syscalls[1] = syscall_video_putc;
    syscalls[2] = syscall_video_puts;
}

void syscall_handler()
{
    // Disable the IRQs.
    irq_disable();
    // Retrieve the parameters.
    register_t regs;
    __asm__("movl %%eax, %0\n\t"
        "movl %%ecx, %1\n\t"
        "movl %%ebx, %2\n\t"
        "movl %%edx, %3\n\t"
    : "=r"(regs.eax), "=r"(regs.ecx), "=r"(regs.ebx), "=r"(regs.edx));
    // Check that the systemcall number is correct.
    assert(regs.eax < SYSCALL_NUMBER);
    // Call the systemcall.
    uint32_t arguments[3] = {regs.ecx, regs.ebx, regs.edx};
    (syscalls[regs.eax])(arguments);

    // Re-Enable the IRQs.
    irq_enable();
}

