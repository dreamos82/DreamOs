/// @file   isr.h
/// @brief  Data structures concerning the Interrupt Service Routines (ISRs).
/// @author Ivan Gualandri
/// @date   Oct 27 2003
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

#pragma once

#include "pic8259.h"

/// @defgroup exceptions Exception Conditions
/// @brief Each description classifies the exception as a fault, trap, or abort
/// @{

#define DIVIDE_ERROR            0
#define DEBUG_EXC               1
#define NMI_INTERRUPT           2
#define BREAKPOINT              3
#define OVERFLOW                4
#define BOUND_RANGE_EXCEED      5
#define INVALID_OPCODE          6
#define DEV_NOT_AVL             7
#define DOUBLE_FAULT            8
#define COPROC_SEG_OVERRUN      9
#define INVALID_TSS             10
#define SEGMENT_NOT_PRESENT     11
#define STACK_SEGMENT_FAULT     12
#define GENERAL_PROTECTION      13
#define PAGE_FAULT              14
#define INT_RSV                 15
#define FLOATING_POINT_ERR      16
#define ALIGNMENT_CHECK         17
#define MACHINE_CHECK           18
#define SIMD_FP_EXC             19

/// @}

/// List of shared IRQs.
extern irq_struct_t * shared_irq_handlers[IRQ_NUM];

/// @brief Questa funzione si occupa di inizializzare la tabella di funzioni
/// che gestiscono le interruzioni ed eccezioni.
/// @author Ivan Gualandri
void init_isr();

/// @brief Questa funzione gestira a livello centralizzato le varie eccezioni.
/// @author Ivan Gualandri
extern void global_exception(int n, int error);

/// @brief Main function used to handle interrupts.
extern void interrupt_handler(unsigned int esp);

/// @brief Function called when a reserved exception occurs.
extern void reserved_exception();
