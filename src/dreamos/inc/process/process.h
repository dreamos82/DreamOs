/*
 * Copyright (c), Dario Casalinuovo
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

//
// Based on JamesM's kernel developement tutorials.
//

#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stddef.h>
#include <list.h>

struct process_list;

enum eflags_list
{
    EFLAG_CF = (1 << 0),    ///< 0  Carry flag
    EFLAG_PF = (1 << 2),    ///< 2  Parity flag
    EFLAG_AF = (1 << 4),    ///< 4  Auxiliary carry flag
    EFLAG_ZF = (1 << 6),    ///< 6  Zero flag
    EFLAG_SF = (1 << 7),    ///< 7  Sign flag
    EFLAG_TF = (1 << 8),    ///< 8  Trap flag
    EFLAG_IF = (1 << 9),    ///< 9  Interrupt enable flag
    EFLAG_DF = (1 << 10),   ///< 10 Direction flag
    EFLAG_OF = (1 << 11),   ///< 11 Overflow flag
    EFLAG_NT = (1 << 14),   ///< 14 Nested task flag
    EFLAG_RF = (1 << 16),   ///< 16 Resume flag
    EFLAG_VM = (1 << 17),   ///< 17 Virtual 8086 mode flag
    EFLAG_AC = (1 << 18),   ///< 18 Alignment check flag (486+)
    EFLAG_VIF = (1 << 19),  ///< 19 Virutal interrupt flag
    EFLAG_VIP = (1 << 20),  ///< 20 Virtual interrupt pending flag
    EFLAG_ID = (1 << 21),   ///< 21 ID flag
};
// 12-13  IOPL    I/O Priviledge level

/// @brief The register set of CPU where process need to be stored for
/// execution for running state.
typedef struct context_t
{
    /// Holds the top address of the stack. (Stack pointer register)
    uint32_t esp;
    /// Holds the base address of the stack. (Stack Base pointer register)
    uint32_t ebp;
    /// Used as a base pointer for memory access. (Base register)
    uint32_t ebx;
    /// Used for string and memory array copying. (Source index register)
    uint32_t esi;
    /// Used for string, memory array copying and setting and for far pointer
    /// addressing with ES (Destination index register)
    uint32_t edi;
    /// Register which hold the state of the processor.
    uint32_t eflags;
} context_t;

/// @brief Holds information about a process.
typedef struct process
{
    /// The registers of the process.
    context_t regs;
    /// The id of the process.
    pid_t id;
    /// The command executed by the process.
    char name[50];
    /// The exit flag. When set to 1, the scheduler will deactivate the process.
    __volatile__ uint32_t exit;
    /// Pointer to the node of the scheduler list of processs.
    listnode_t * self;
} process_t;

// If stack is 0 a default one is created
process_t * kernel_create_process(int (* fn)(void *),
                                char * name,
                                void * arg,
                                uint32_t * stack);

#endif
