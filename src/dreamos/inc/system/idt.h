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
#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

/// IDT dimension.
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

// Array of interrupt handler functions.
extern interrupt_handler_t IntTable[IDT_SIZE];

/// @brief IDT initialisation function.
void init_idt();

/// @brief Add a new segment.
/// @param i
/// @param base
/// @param options
/// @param seg_sel
void kernel_add_idt_seg(short int i,
                        uint32_t base,
                        unsigned char options,
                        unsigned int seg_sel);


void INT_0(); /**< Eccezione numero 0 - #DE Divide Error*/
void INT_1(); /**< Eccezione numero 1 - #DB Debug*/
void INT_2(); /**< Eccezione numero 2 - Non Mascable Interrupt*/
void INT_3(); /**< Eccezione numero 3 - #BP Breakpoint*/
void INT_4(); /**< Eccezione numero 4 - #OF Overflow*/
void INT_5(); /**< Eccezione numero 5 - #BR Bound Range Exception*/
void INT_6(); /**< Eccezione numero 6 - #UD Invalid OpCode Exception*/
void INT_7(); /**< Eccezione numero 7 - #NM Device Not Available*/
void INT_8(); /**< Eccezione numero 8 - #DF Double Fault*/
void INT_9(); /**< Eccezione numero 9 - Coprocessor Segment Overrun*/
void INT_10(); /**< Eccezione numero 10 - #TS Invalid TSS*/
void INT_11(); /**< Eccezione numero 11 - #NP Segment Not Present*/
void INT_12(); /**< Eccezione numero 12 - #SS Stack Segment Fault*/
void INT_13(); /**< Eccezione numero 13 - #GP General Protection*/
void INT_14(); /**< Eccezione numero 14 - #PF Page Fault*/
void INT_15(); /**< Eccezione numero 15 - #XX Reserverd*/
void INT_16(); /**< Eccezione numero 16 - #MF Floating Point */
void INT_17(); /**< Eccezione numero 17 - #AC Alignment Check*/
void INT_18(); /**< Eccezione numero 18 - #MC Machine Check*/
void INT_19(); /**< Eccezione numero 19 - #XF Streaming SIMD Exception*/

void INT_32();

void INT_33();

void INT_34();

void INT_35();

void INT_36();

void INT_37();

void INT_38();

void INT_39();

void INT_40();

void INT_41();

void INT_42();

void INT_43();

void INT_44();

void INT_45();

void INT_46();

void INT_47();

void INT_48();

void INT_80();

#endif
