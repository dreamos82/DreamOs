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
void INT_0();

/// @brief #DB Debug
void INT_1();

/// @brief Non Mascable Interrupt
void INT_2();

/// @brief #BP Breakpoint
void INT_3();

/// @brief #OF Overflow
void INT_4();

/// @brief #BR Bound Range Exception
void INT_5();

/// @brief #UD Invalid OpCode Exception
void INT_6();

/// @brief #NM Device Not Available
void INT_7();

/// @brief #DF Double Fault
void INT_8();

/// @brief Coprocessor Segment Overrun
void INT_9();

/// @brief #TS Invalid TSS
void INT_10();

/// @brief #NP Segment Not Present
void INT_11();

/// @brief #SS Stack Segment Fault
void INT_12();

/// @brief #GP General Protection
void INT_13();

/// @brief #PF Page Fault
void INT_14();

/// @brief #XX Reserverd
void INT_15();

/// @brief #MF Floating Point
void INT_16();

/// @brief #AC Alignment Check
void INT_17();

/// @brief #MC Machine Check
void INT_18();

/// @brief #XF Streaming SIMD Exception
void INT_19();

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