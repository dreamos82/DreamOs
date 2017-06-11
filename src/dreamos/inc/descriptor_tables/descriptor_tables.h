/// @file   descriptor_tables.h
/// @brief  Data structures concerning descriptor tables.
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
#include "gdt.h"
#include "idt.h"

/// Array of interrupt handler functions.
extern interrupt_handler_t IntTable[IDT_SIZE];

/// @brief Questa funzione aggiunge un handler di interruzione alla tabella
/// per che contiene le funzioni di gestione chiamate dalle
/// eccezioni/interruzioni della IDT.
/// @param i        Posizione all'interno del vettore.
/// @param handler  Funzione da aggiungere.
void kernel_add_interrupt_function_table(int i,
                                         interrupt_handler_t handler);

/// @defgroup interrupts Interrupt Functions
/// @brief This is the list of interrupt functions.
/// @{

/// @brief DE Divide Error
extern void INT_0();

/// @brief DB Debug
extern void INT_1();

/// @brief Non Mascable Interrupt
extern void INT_2();

/// @brief BP Breakpoint
extern void INT_3();

/// @brief OF Overflow
extern void INT_4();

/// @brief BR Bound Range Exception
extern void INT_5();

/// @brief UD Invalid OpCode Exception
extern void INT_6();

/// @brief NM Device Not Available
extern void INT_7();

/// @brief DF Double Fault
extern void INT_8();

/// @brief Coprocessor Segment Overrun
extern void INT_9();

/// @brief TS Invalid TSS
extern void INT_10();

/// @brief NP Segment Not Present
extern void INT_11();

/// @brief SS Stack Segment Fault
extern void INT_12();

/// @brief GP General Protection
extern void INT_13();

/// @brief PF Page Fault
extern void INT_14();

/// @brief XX Reserverd
extern void INT_15();

/// @brief MF Floating Point
extern void INT_16();

/// @brief AC Alignment Check
extern void INT_17();

/// @brief MC Machine Check
extern void INT_18();

/// @brief XF Streaming SIMD Exception
extern void INT_19();

/// @brief Interrupt number 32.
extern void INT_32();

/// @brief Interrupt number 33.
extern void INT_33();

/// @brief Interrupt number 34.
extern void INT_34();

/// @brief Interrupt number 35.
extern void INT_35();

/// @brief Interrupt number 36.
extern void INT_36();

/// @brief Interrupt number 37.
extern void INT_37();

/// @brief Interrupt number 38.
extern void INT_38();

/// @brief Interrupt number 39.
extern void INT_39();

/// @brief Interrupt number 40.
extern void INT_40();

/// @brief Interrupt number 41.
extern void INT_41();

/// @brief Interrupt number 42.
extern void INT_42();

/// @brief Interrupt number 43.
extern void INT_43();

/// @brief Interrupt number 44.
extern void INT_44();

/// @brief Interrupt number 45.
extern void INT_45();

/// @brief Interrupt number 46.
extern void INT_46();

/// @brief Interrupt number 47.
extern void INT_47();

/// @brief Interrupt number 48.
extern void INT_48();

/// @brief Interrupt used to implement System Call.
extern void INT_80();

/// @}