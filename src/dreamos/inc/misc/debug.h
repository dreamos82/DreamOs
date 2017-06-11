/// @file   debug.h
/// @brief  Debugging primitives.
/// @author Theking0 - Ivan Gualandri
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

#include "port_io.h"
#include "kernel.h"

/// @brief Used to enable the device. Any I/O to the debug module before this
/// command is sent will simply be ignored.
#define DBG_ENABLE        0x8A00

/// @brief Disable the I/O interface to the debugger and the memory
/// monitoring functions.
#define DBG_DISABLE        0x8AFF

/// @brief Selects register 0: Memory monitoring range start address (inclusive)
#define SELECTS_REG_0    0x8A01

/// @brief Selects register 1: Memory monitoring range end address (exclusive)
#define SELECTS_REG_1    0x8A02

/// @brief Enable address range memory monitoring as indicated by register 0
/// and 1 and clears both registers.
#define ENABLE_ADDR_RANGE_MEM_MONITOR    0x8A80

/// @brief If the debugger is enabled (via --enable-debugger), sending 0x8AE0
/// to port 0x8A00 after the device has been enabled will return the Bochs
/// to the debugger prompt. Basically the same as doing CTRL+C.
#define RET_TO_DBGPROMPT    0x8AE0

/// @brief If the debugger is enabled (via --enable-debugger), sending 0x8AE2
/// to port 0x8A00 after the device has been enabled will enable instruction
/// tracing.
#define INSTRUCTION_TRACE_ENABLE    0x8AE3

/// @brief If the debugger is enabled (via --enable-debugger), sending 0x8AE2
/// to port 0x8A00 after the device has been enabled will disable instruction
/// tracing.
#define INSTRUCTION_TRACE_DISABLE    0x8AE2

/// @brief If the debugger is enabled (via --enable-debugger), sending 0x8AE5
/// to port 0x8A00 after the device has been enabled will enable register
/// tracing. This currently output the value of all the registers for each
/// instruction traced. Note: instruction tracing must be enabled to view
/// the register tracing.
#define REGISTER_TRACE_ENABLE    0x8AE5

/// @brief If the debugger is enabled (via --enable-debugger), sending 0x8AE4
/// to port 0x8A00 after the device has been enabled will disable register
/// tracing.
#define REGISTER_TRACE_DISABLE    0x8AE4

/// @brief Prints the given string to the debug output.
void dbg_print(const char * msg, ...);

/// @brief Prints the given registers to the debug output.
void print_reg(register_t * reg);

/// @brief Prints the given 16-bit registers to the debug output.
void print_reg16(register16_t * reg);
