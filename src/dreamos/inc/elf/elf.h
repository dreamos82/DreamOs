/// @file   elf.h
/// @brief  Function for multiboot support.
/// @details Based on JamesM's kernel developement tutorials.
/// @author Dario Casalinuovo
/// @date   Mar 31 2007
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

#define ELF32_ST_TYPE(i) ((i)&0xf)

typedef enum elf_type_t
{
    ET_NONE = 0,    ///< @brief Unkown Type
    ET_REL = 1,     ///< @brief Relocatable File
    ET_EXEC = 2     ///< @brief Executable File
} elf_type_t;

typedef struct elf_section_header_t
{
    uint32_t name;
    /// @brief Identifies the object file type.
    uint32_t type;
    /// @brief Holds processor-specific flags associated with the file.
    uint32_t flags;
    /// @brief The virtual address to which the system first transfers
    /// control, thus starting the process.
    uint32_t addr;
    /// @brief Offset from the begining of the file
    uint32_t offset;
    /// @brief The ELF header's size in bytes.
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} __attribute__((packed)) elf_section_header_t;

typedef struct elf_symbol_t
{
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
} __attribute__((packed)) elf_symbol_t;

/// @brief Structure used to store the symbol table.
typedef struct elf_t
{
    elf_symbol_t * symtab;
    uint32_t symtabsz;
    const char * strtab;
    uint32_t strtabsz;
} elf_t;

/// @brief Takes a multiboot structure and returns an elf structure
/// containing the symbol information.
/// @param mb The input multiboot structure.
/// @return Elf structure containing the symbol information.
elf_t elf_from_multiboot(multiboot_info_t * mb);

//
/// @brief Looks up a symbol by address.
/// @param address  The address used to get the symbol
/// @param elf      The elf containing the symbol map.
/// @return The symbol if found.
const char * elf_lookup_symbol(uint32_t address, elf_t * elf);
