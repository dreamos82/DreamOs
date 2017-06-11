/// @file   multiboot.h
/// @brief  Data structures used for multiboot.
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

// Do not inc here in boot.s

#pragma once

/// @brief The Multiboot header.
typedef struct multiboot_header_t
{
    /// TODO: Check meaning!
    unsigned long magic;
    /// TODO: Check meaning!
    unsigned long flags;
    /// TODO: Check meaning!
    unsigned long checksum;
    /// TODO: Check meaning!
    unsigned long header_addr;
    /// TODO: Check meaning!
    unsigned long load_addr;
    /// TODO: Check meaning!
    unsigned long load_end_addr;
    /// TODO: Check meaning!
    unsigned long bss_end_addr;
    /// TODO: Check meaning!
    unsigned long entry_addr;
} multiboot_header_t;

/// @brief The symbol table for a.out.
typedef struct aout_symbol_table_t
{
    /// TODO: Check meaning!
    unsigned long tabsize;
    /// TODO: Check meaning!
    unsigned long strsize;
    /// TODO: Check meaning!
    unsigned long addr;
    /// TODO: Check meaning!
    unsigned long reserved;
} aout_symbol_table_t;

/// @brief The section header table for ELF.
typedef struct elf_section_header_table_t
{
    /// TODO: Check meaning!
    unsigned long num;
    /// TODO: Check meaning!
    unsigned long size;
    /// TODO: Check meaning!
    unsigned long addr;
    /// TODO: Check meaning!
    unsigned long shndx;
} elf_section_header_table_t;

/// @brief The Multiboot information.
typedef struct multiboot_info_t
{
    /// TODO: Check meaning!
    unsigned long flags;
    /// TODO: Check meaning!
    unsigned long mem_lower;
    /// TODO: Check meaning!
    unsigned long mem_upper;
    /// TODO: Check meaning!
    unsigned long boot_device;
    /// TODO: Check meaning!
    unsigned long cmdline;
    /// TODO: Check meaning!
    unsigned long mods_count;
    /// TODO: Check meaning!
    unsigned long mods_addr;
    /// TODO: Check meaning!
    union
    {
        /// TODO: Check meaning!
        aout_symbol_table_t aout_sym;
        /// TODO: Check meaning!
        elf_section_header_table_t elf_sec;
    } u;
    /// TODO: Check meaning!
    unsigned long mmap_length;
    /// TODO: Check meaning!
    unsigned long mmap_addr;
} multiboot_info_t;

/// @brief The module structure.
typedef struct module_t
{
    /// TODO: Check meaning!
    unsigned long mod_start;
    /// TODO: Check meaning!
    unsigned long mod_end;
    /// TODO: Check meaning!
    unsigned long string;
    /// TODO: Check meaning!
    unsigned long reserved;
} module_t;

/// @brief The memory map. Be careful that the offset 0 is base_addr_low but
/// no size.
typedef struct memory_map_t
{
    /// TODO: Check meaning!
    unsigned long size;
    /// TODO: Check meaning!
    unsigned long base_addr_low;
    /// TODO: Check meaning!
    unsigned long base_addr_high;
    /// TODO: Check meaning!
    unsigned long length_low;
    /// TODO: Check meaning!
    unsigned long length_high;
    /// TODO: Check meaning!
    unsigned long type;
} memory_map_t;
