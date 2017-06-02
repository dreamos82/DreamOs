/// @file   pic8259.h
/// @brief  pic8259 definitions.
/// @author shainer <shainer@debianclan.org>
/// @date   Jun 2007
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

#include "stddef.h"
#include "idt.h"

#define MASTER_PORT     0x20
#define SLAVE_PORT      0xA0

#define MASTER_PORT_1   0x21
#define SLAVE_PORT_1    0xA1

#define ICW_1           0x11

#define ICW_2_M         0x20
#define ICW_2_S         0x28

#define ICW_3_M         0x04
#define ICW_3_S         0x02

#define ICW_4           0x01

#define GET_IRR_STATUS  0x0b

#define EOI             0x20

#define IRQ_NUM         16

extern byte_t master_cur_mask;

extern byte_t slave_cur_mask;

/// @brief Types of irq.
typedef enum __attribute__ ((__packed__)) irq_type_t
{
    TIMER,
    KEYBOARD,
    TO_SLAVE_PIC,
    COM2_4,
    COM1_3,
    LPT2,
    FLOPPY,
    LPT1,
    REAL_TIME_CLOCK,
    AVAILABLE_1,
    AVAILABLE_2,
    AVAILABLE_3,
    MOUSE,
    MATH_CPU,
    FIRST_HD,
    SECOND_HD
} irq_type_t;

/// @brief Struttura dati per gestire gli IRQ  condivisi
typedef struct irq_struct_t
{
    /// Puntatore alla funzione handler di un IRQ.
    interrupt_handler_t handler;
    /// Prossimo handler per questo IRQ
    struct irq_struct_t * next;
} irq_struct_t;

/// @brief Funzione che inizializza il processore pic 8259 che gestira' le
/// interruzioni.
/// @author Ivan Gualandri
/// @version 1.1
void pic8259_init_irq();

/// @brief This function, enable irqs on the pic.
/// @details This function provide a tool for enabling irq from the pic
/// processor.
/// @author Ivan Gualandri
/// @version 1.0
/// @param irq number of irq to enable.
/// @return 0 if all OK, -1 on errors
int pic8259_irq_enable(irq_type_t irq);

/// @brief This function, disable irqs on the pic.
/// @details This function provide a tool for enabling irq from the pic
/// processor.
/// @author Ivan Gualandri
/// @version 1.0
/// @param irq number of irq to enable.
/// @return 0 if all OK, -1 on errors
int pic8259_irq_disable(irq_type_t irq);

/// @brief This Function return the number of current IRQ Request.
/// @author Ivan Gualandri
/// @version 1.0
/// @return Number of IRQ + 1 currently serving. If 0 there are no IRQ
int pic8259_irq_get_current();

/// @brief This Function add an IRQ Handler to the givent irq number.
/// @author Ivan Gualandri
/// @version 1.0
/// @param irq_number   Number of IRQ to serve (from 0 to 16).
/// @param handler      Function to add.
void
pic8259_irq_install_handler(uint32_t irq_number, interrupt_handler_t handler);
