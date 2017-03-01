/*
 * Dreamos
 * gdt.h
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

#pragma once

#include <stdint.h>

/// The maximum dimension of the IDT.
#define IDT_SIZE 256

//IDT Gate Types (in options)
#define TASK_GATE     0x5
#define INT16_GATE    0x6
#define TRAP16_GATE   0x7
#define INT32_GATE    0xE
#define TRAP32_GATE   0xF

// An interrupt handler. It is a pointer to a function which takes a pointer
// to a structure containing register values.
typedef void (* interrupt_handler_t)();

/// @brief This structure describes one interrupt gate.
typedef struct __attribute__ ((__packed__)) idt_descriptor_t
{
    /// Il puntatore all'istruzione.
    unsigned short int offset_low;
    /// Seleziona il segmento codice dalla GDT (nel nostro caso primo quindi
    /// 0x8, si specifica l'offset).
    unsigned short int seg_selector;
    /// E' un campo che deve essere sempre impostato a 0.
    unsigned char null_par;
    /// Le opzioni del selettore dela IDT.
    unsigned char options;
    /// Il puntatore all'istruzione.
    unsigned short int offset_high;
} idt_descriptor_t;

/// @brief A pointer structure used for informing the CPU about our IDT.
typedef struct __attribute__ ((__packed__)) idt_pointer_t
{
    /// La dimensione della IDT (in numero di entry).
    unsigned short int limit;
    /// L'indirizzo iniziale della IDT.
    unsigned int base;
} idt_pointer_t;

/// @brief Initialise the interrupt descriptor table.
void init_idt();

/// @brief Questa funzione si occupa di aggiungere un nuovo segmento alla IDT.
/// @author Ivan Gualandri
/// @param index    Indice della IDT.
/// @param base     Puntatore alla funzione che gestira' l'interrupt/Eccezione
/// @param options  Le opzioni del descrittore (PRESENT,NOTPRESENT,KERNEL,USER)
/// @param seg_sel  Il selettore del segmento della GDT.
void idt_set_gate(uint8_t index,
                  interrupt_handler_t handler,
                  uint16_t options,
                  uint8_t seg_sel);
