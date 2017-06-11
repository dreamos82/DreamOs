/// @file   kernel.h
/// @brief  Kernel generic data structure and functions.
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

#include "multiboot.h"
#include "stdint.h"

/// Pointer the beging of the module.
extern char * module_start;
/// Address to the end of the module.
extern unsigned int module_end;

/// @brief Entry point of the kernel.
/// @param boot_informations Information concerning the boot.
/// @return The exit status of the kernel.
int kmain(multiboot_info_t * boot_informations);

/// Register structs for interrupt/exception
typedef struct register_t
{
    uint32_t ds;        ///< Data Segment.
    uint32_t edi;       ///< 32-bit destination register.
    uint32_t esi;       ///< 32-bit source register.
    uint32_t ebp;       ///< 32-bit base pointer register.
    uint32_t esp;       ///< 32-bit stack pointer register.
    uint32_t ebx;       ///< 32-bit base register.
    uint32_t edx;       ///< 32-bit data register.
    uint32_t ecx;       ///< 32-bit counter.
    uint32_t eax;       ///< 32-bit accumulator register.
    uint32_t int_no;    ///< Interrupt number.
    uint32_t err_code;  ///< Error code.
    uint32_t eip;       ///< Instruction Pointer Register
    uint32_t cs;        ///< Code Segment.
    uint32_t eflags;    ///< 32-bit flag register.
    uint32_t useresp;   ///< TODO: Check meaning!
    uint32_t ss;        ///< Stack Segment.
} register_t;

/// Register structs for bios service
typedef struct register16_t
{
    uint16_t di;    ///< Destination Index.
    uint16_t si;    ///< Source Index.
    uint16_t bp;    ///< Base Pointer.
    uint16_t sp;    ///< Stack Pointer.
    uint16_t bx;    ///< Also known as the base register.
    uint16_t dx;    ///< Also known as the data register.
    uint16_t cx;    ///< Also known as the count register.
    uint16_t ax;    ///< Is the primary accumulator.
    uint16_t ds;    ///< Data Segment.
    uint16_t es;    ///< Extra Segment determined by the programmer
    uint16_t fs;    ///< FS and GS have no hardware-assigned uses
    uint16_t gs;    ///< FS and GS have no hardware-assigned uses
    uint16_t ss;    ///< Stack Segment.
    uint16_t eflags;///< 32-bit flag register.
} register16_t;
