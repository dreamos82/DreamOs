/// @file   descriptor_tables.h

#pragma once

#include <stdint.h>
#include <gdt.h>
#include <idt.h>

/// Access flags, determines what ring this segment can be used in.
typedef enum __attribute__ ((__packed__)) gdt_access_option_t
{
    PRESENT = 0x80,
    KERNEL = 0x00,
    USER = 0x03,
    CODE = 0x10,
    DATA = 0x10,
} gdt_access_option_t;

/// Options for the second option.
typedef enum __attribute__ ((__packed__)) gdt_granularity_option_t
{
    GRANULARITY = 0x80,
    SZBITS = 0x40
} gdt_granularity_option_t;

/// Array of interrupt handler functions.
extern interrupt_handler_t IntTable[IDT_SIZE];

/// @brief Initialisation routine - zeroes all the interrupt service
/// routines, initialises the GDT and IDT.
void init_descriptor_tables();

/// @brief Questa funzione aggiunge un handler di interruzione alla tabella per che contiene le funzioni di gestione chiamate dalle eccezioni/interruzioni della IDT
/// @param i        Posizione all'interno del vettore.
/// @param handler  Funzione da aggiungere.
void kernel_add_interrupt_function_table(int i,
                                         interrupt_handler_t handler);

/// @brief #DE Divide Error
extern void INT_0();

/// @brief #DB Debug
extern void INT_1();

/// @brief Non Mascable Interrupt
extern void INT_2();

/// @brief #BP Breakpoint
extern void INT_3();

/// @brief #OF Overflow
extern void INT_4();

/// @brief #BR Bound Range Exception
extern void INT_5();

/// @brief #UD Invalid OpCode Exception
extern void INT_6();

/// @brief #NM Device Not Available
extern void INT_7();

/// @brief #DF Double Fault
extern void INT_8();

/// @brief Coprocessor Segment Overrun
extern void INT_9();

/// @brief #TS Invalid TSS
extern void INT_10();

/// @brief #NP Segment Not Present
extern void INT_11();

/// @brief #SS Stack Segment Fault
extern void INT_12();

/// @brief #GP General Protection
extern void INT_13();

/// @brief #PF Page Fault
extern void INT_14();

/// @brief #XX Reserverd
extern void INT_15();

/// @brief #MF Floating Point
extern void INT_16();

/// @brief #AC Alignment Check
extern void INT_17();

/// @brief #MC Machine Check
extern void INT_18();

/// @brief #XF Streaming SIMD Exception
extern void INT_19();

extern void INT_32();

extern void INT_33();

extern void INT_34();

extern void INT_35();

extern void INT_36();

extern void INT_37();

extern void INT_38();

extern void INT_39();

extern void INT_40();

extern void INT_41();

extern void INT_42();

extern void INT_43();

extern void INT_44();

extern void INT_45();

extern void INT_46();

extern void INT_47();

extern void INT_48();

extern void INT_80();