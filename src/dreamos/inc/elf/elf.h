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

/// @brief TODO: Check!
#define ELF32_ST_TYPE(i) ((i) & 0xF)

/// @brief Specifies the type of an elf header.
typedef enum elf_header_type_t
{
    ET_NONE,        ///< @brief Unknown
    ET_RELOCATABLE, ///< @brief Relocatable
    ET_EXECUTABLE,  ///< @brief Executable
    ET_SHARED,      ///< @brief Shared
    ET_CORE         ///< @brief Core
} elf_header_type_t;

/// @brief Section header table, describing zero or more sections.
typedef struct elf_section_header_t
{
    /// @brief An offset to a string in the .shstrtab section that represents
    /// the name of this section.
    uint32_t name;
    /// @brief Identifies the type of this header.
    uint32_t type;
    /// @brief Identifies the attributes of the section.
    uint32_t flags;
    /// @brief Virtual address of the section in memory, for sections that
    /// are loaded.
    uint32_t addr;
    /// @brief Offset of the section in the file image.
    uint32_t offset;
    /// @brief Size in bytes of the section in the file image. May be 0.
    uint32_t size;
    /// @brief Contains the section index of an associated section. This
    /// field is used for several purposes, depending on the type of section.
    uint32_t link;
    /// @brief Contains extra information about the section. This field is
    /// used for several purposes, depending on the type of section.
    uint32_t info;
    /// @brief Contains the required alignment of the section. This field
    /// must be a power of two.
    uint32_t addralign;
    /// @brief Contains the size, in bytes, of each entry, for sections that
    /// contain fixed-size entries. Otherwise, this field contains zero.
    uint32_t entsize;
} __attribute__((packed)) elf_section_header_t;

/// @brief Information concerning a symbol.
typedef struct elf_symbol_t
{
    /// Symbol name.
    uint32_t name;
    /// The address of the Symbol.
    uint32_t value;
    /// The size of the symbol.
    uint32_t size;
    /// @brief Symbol type:
    ///    Func = Function, Object, File (source file name),
    /// Section = memory section,
    ///  Notype = untyped absolute symbol or undefined
    uint8_t type;
    /// TODO: Check!
    uint8_t other;
    /// TODO: Check!
    uint16_t shndx;
} __attribute__((packed)) elf_symbol_t;

/// @brief Structure used to store the symbol table.
typedef struct elf_t
{
    /// @brief Pointer to a symbol table.
    elf_symbol_t * symtab;
    /// @brief Pointer to a string table.
    const char * strtab;
    /// @brief Symbol table size.
    uint32_t symtabsz;
    /// @brief String table size.
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
