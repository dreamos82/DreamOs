/// @file   gdt.h
/// @brief  Data structures concerning the Global Descriptor Table (GDT).
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

/// Access flags, determines what ring this segment can be used in.
typedef enum gdt_access_option_t
{
    PRESENT = 0x80,
    KERNEL = 0x00,
    USER = 0x03,
    CODE = 0x10,
    DATA = 0x10,
} __attribute__ ((__packed__)) gdt_access_option_t;

/// Options for the second option.
typedef enum gdt_granularity_option_t
{
    GRANULARITY = 0x80,
    SZBITS = 0x40
} __attribute__ ((__packed__)) gdt_granularity_option_t;

/// @brief Struttura dati che rappresenta un descrittore della GDT.
typedef struct gdt_descriptor_t
{
    /// Primi 16 bit del limite (20 bit in totale)
    uint16_t limit_low;
    /// The lower 16 bits of the base.
    uint16_t base_low;
    /// The next 8 bits of the base.
    uint8_t base_middle;
    /// Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit).
    uint8_t access;
    /// SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)
    uint8_t granularity;
    /// The last 8 bits of the base.
    uint8_t base_high;
} __attribute__((packed)) gdt_descriptor_t;

/// @brief Struttura dati che serve a caricare la GDT nel GDTR.
typedef struct gdt_pointer_t
{
    /// La dimensione della GDT (in numero di entry).
    uint16_t limit;
    /// L'indirizzo iniziale della GDT;
    uint32_t base;
} __attribute__((packed)) gdt_pointer_t;

/// @brief Initialise the GDT.
/// @details This will setup the special GDT
/// pointer, set up the first 3 entries in our GDT, and then
/// finally call gdt_flush() in our assembler file in order
/// to tell the processor where the new GDT is and update the
/// new segment registers.
void init_gdt();

/// @brief Sets the value of one GDT entry.
/// @param index    The index inside the GDT.
/// @param base     Memory address where the segment we are defining starts.
/// @param limit    The memory address which determines the end of the segnment.
/// @param _access  Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit)
/// @param _granul  SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)
void gdt_set_gate(uint8_t index,
                  uint32_t base,
                  uint32_t limit,
                  uint8_t _access,
                  uint8_t _granul);
