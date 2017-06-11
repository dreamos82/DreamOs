/// @file   idt.h
/// @brief  Data structures concerning the Interrupt Descriptor Table (IDT).
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

#include "stdint.h"

/// The maximum dimension of the IDT.
#define IDT_SIZE 256

/// When an exception occurs whose entry is a Task Gate, a task switch results.
#define TASK_GATE     0x5
/// Used to specify an interrupt service routine (16-bit).
#define INT16_GATE    0x6
/// @brief Similar to an Interrupt gate (16-bit).
#define TRAP16_GATE   0x7
/// Used to specify an interrupt service routine (32-bit).
#define INT32_GATE    0xE
/// @brief Similar to an Interrupt gate (32-bit).
#define TRAP32_GATE   0xF

// Trap and Interrupt gates are similar, and their descriptors are
// structurally the same, they differ only in the "type" field. The
// difference is that for interrupt gates, interrupts are automatically
// disabled upon entry and reenabled upon IRET which restores the saved EFLAGS.

/// @brief An interrupt handler.
/// @details It is a pointer to a function which takes a pointer to a
/// structure containing register values.
typedef void (* interrupt_handler_t)();

/// @brief This structure describes one interrupt gate.
typedef struct idt_descriptor_t
{
    /// Il puntatore all'istruzione.
    unsigned short int offset_low;
    /// Seleziona il segmento codice dalla GDT.
    unsigned short int seg_selector;
    /// E' un campo che deve essere sempre impostato a 0.
    unsigned char null_par;
    /// Le opzioni del selettore dela IDT.
    unsigned char options;
    /// Il puntatore all'istruzione.
    unsigned short int offset_high;
} __attribute__((packed)) idt_descriptor_t;

/// @brief A pointer structure used for informing the CPU about our IDT.
typedef struct idt_pointer_t
{
    /// La dimensione della IDT (in numero di entry).
    unsigned short int limit;
    /// L'indirizzo iniziale della IDT.
    unsigned int base;
} __attribute__((packed)) idt_pointer_t;

/// @brief Initialise the interrupt descriptor table.
void init_idt();

/// @brief Use this function to set an entry in the IDT.
/// @author Ivan Gualandri
/// @param index    Indice della IDT.
/// @param handler  Puntatore alla funzione che gestira' l'interrupt/Eccezione
/// @param options  Le opzioni del descrittore (PRESENT,NOTPRESENT,KERNEL,USER)
/// @param seg_sel  Il selettore del segmento della GDT.
void idt_set_gate(uint8_t index,
                  interrupt_handler_t handler,
                  uint16_t options,
                  uint8_t seg_sel);
