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

/// End-of-interrupt command code.
#define EOI                 0x20
/// IO base address for master PIC.
#define MASTER_PORT_COMMAND 0x20
/// I/O address for data to master.
#define MASTER_PORT_DATA    (MASTER_PORT_COMMAND + 1)
/// IO base address for slave PIC.
#define SLAVE_PORT_COMMAND  0xA0
/// I/O address for data to slave.
#define SLAVE_PORT_DATA     (SLAVE_PORT_COMMAND + 1)
/// TODO: Check meaning!
#define ICW_1           0x11
/// TODO: Check meaning!
#define ICW_2_M         0x20
/// TODO: Check meaning!
#define ICW_2_S         0x28
/// TODO: Check meaning!
#define ICW_3_M         0x04
/// TODO: Check meaning!
#define ICW_3_S         0x02
/// TODO: Check meaning!
#define ICW_4           0x01
/// OCW3 irq ready next CMD read.
#define PIC_READ_IRR                0x0A
/// OCW3 irq service next CMD read.
#define PIC_READ_ISR                0x0B
/// The total number of IRQs.
#define IRQ_NUM         16

/// @defgroup irqs Interrupt Requests (IRQs)
/// @brief This is the list of interrupt requests.
/// @{

/// @brief System timer.
#define IRQ_TIMER           0
/// @brief Keyboard controller.
#define IRQ_KEYBOARD        1
/// @brief cascaded signals from IRQs 8–15 (any devices configured to use IRQ
/// 2 will actually be using IRQ 9)
#define IRQ_TO_SLAVE_PIC    2
/// @brief Serial port controller for serial port 2 (and 4).
#define IRQ_COM2_4          3
/// @brief Serial port controller for serial port 1 (and 3).
#define IRQ_COM1_3          4
/// @brief Parallel port 2 and 3 (or sound card).
#define IRQ_LPT2            5
/// @brief Floppy disk controller.
#define IRQ_FLOPPY          6
/// @brief Parallel port 1.
#define IRQ_LPT1            7
/// @brief Real-time clock (RTC)
#define IRQ_REAL_TIME_CLOCK 8
/// @brief Advanced Configuration and Power Interface (ACPI)
/// system control interrupt on Intel chipsets.[1] Other chipset
/// manufacturers might use another interrupt for this purpose, or make it
/// available for the use of peripherals (any devices configured to use IRQ
/// 2 will actually be using IRQ 9)
#define IRQ_AVAILABLE_1     9
/// @brief The Interrupt is left open for the use of
/// peripherals (open interrupt/available, SCSI or NIC)
#define IRQ_AVAILABLE_2     10
/// @brief The Interrupt is left open for the use of
/// peripherals (open interrupt/available, SCSI or NIC)
#define IRQ_AVAILABLE_3     11
/// @brief Mouse on PS/2 connector.
#define IRQ_MOUSE           12
/// @brief CPU co-processor or integrated floating point unit
/// or inter-processor interrupt (use depends on OS)
#define IRQ_MATH_CPU        13
/// @brief primary ATA channel (ATA interface usually serves
/// hard disk drives and CD drives)
#define IRQ_FIRST_HD        14
/// @brief secondary ATA channel
#define IRQ_SECOND_HD       15

/// @}

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
int pic8259_irq_enable(uint32_t irq);

/// @brief This function, disable irqs on the pic.
/// @details This function provide a tool for enabling irq from the pic
/// processor.
/// @author Ivan Gualandri
/// @version 1.0
/// @param irq number of irq to enable.
/// @return 0 if all OK, -1 on errors
int pic8259_irq_disable(uint32_t irq);

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
