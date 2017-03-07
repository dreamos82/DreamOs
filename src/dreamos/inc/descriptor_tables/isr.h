/*
 * Dreamos
 * handlers.h
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

/*
 * Autore Ivan Gualandri
 * Prima versione: 27/10/2003
 * Contiene le definizioni di alcuni tipi di dato :D
 */

#ifndef _HANDLERS_H
#define _HANDLERS_H

#include <pic8259.h>

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

#if 0
#define EXCEPTION(n)\
__asm__("   INT_"#n": \
                pusha; \
                pushl $"#n"; \
                call *(IntTable+(4*"#n")); \
                add $4, %esp; \
                popa; \
                iret;")

#define EXCEPTION_EC(n)\
__asm__("   INT_"#n": \
                nop; \
                xchgl %eax, (%esp); \
                pushl %ecx; \
                pushl %edx; \
                movl $0x10, %edx; \
                movw %dx, %ds; \
                movw %dx, %es; \
                movw %dx, %fs; \
                pushl %eax; \
                pushl $"#n"; \
                call *(IntTable+(4*"#n")); \
                add $8, %esp; \
                popl %edx; \
                popl %ecx; \
                popl %eax; \
                iret;")

#define SYSCALL(n)\
__asm__("   INT_"#n": \
                pusha; \
                call syscall_handler; \
                popa; \
                iret;")

#define IRQ(n)\
__asm__("   INT_"#n": \
                pusha; \
                movl %esp, %eax; \
                pushl %eax; \
                call _irqinterrupt; \
                popl %eax; \
                movl %eax, %esp; \
                popa; \
                iret;")
#endif

/// List of shared IRQs.
extern irq_struct_t * shared_irq_handlers[IRQ_NUM];

/// @brief Questa funzione si occupa di inizializzare la tabella di funzioni
/// che gestiscono le interruzioni ed eccezioni.
/// @author Ivan Gualandri
void init_isr();

/// @brief Questa funzione gestira a livello centralizzato le varie eccezioni.
/// @author Ivan Gualandri
extern void global_exception(int n, int error);

extern void interrupt_handler();

extern void reserved_exception();

#endif
