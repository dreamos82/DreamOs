/// @file   cpuid.h
/// @brief  CPUID definitions.
/// @author shainer <shainer@debianclan.org>
/// @date   Jun 2007
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

#include "stddef.h"
#include "stdint.h"
#include "kernel.h"

/// Dimension of the exc flags.
#define ECX_FLAGS_SIZE 24
/// Dimension of the edx flags.
#define EDX_FLAGS_SIZE 32

/// @brief Contains the information concerning the CPU.
typedef struct cpuinfo_t
{
    /// The name of the vendor.
    char cpu_vendor[13];
    /// The type of the CPU.
    char * cpu_type;
    /// The family of the CPU.
    uint32_t cpu_family;
    /// The model of the CPU.
    uint32_t cpu_model;
    /// Identifier for individual cores when the CPU is interrogated by the
    /// CPUID instruction.
    uint32_t apic_id;
    /// Ecx flags.
    uint32_t cpuid_ecx_flags[ECX_FLAGS_SIZE];
    /// Edx flags.
    uint32_t cpuid_edx_flags[EDX_FLAGS_SIZE];
    /// TODO: Check meaning!
    int is_brand_string;
    /// TODO: Check meaning!
    char * brand_string;
} cpuinfo_t;

/// This will be populated with the information concerning the CPU.
cpuinfo_t sinfo;

/// @brief Main CPUID procedure
/// @param cpuinfo Structure to fill with CPUID information
void get_cpuid(cpuinfo_t * cpuinfo);

/// @brief Actual CPUID call.
/// @param registers The registers to fill with the result of the call.
void call_cpuid(register_t * registers);

/// @brief Extract vendor string.
/// @param cpuinfo   The struct containing the CPUID infos.
/// @param registers The registers.
void cpuid_write_vendor(cpuinfo_t * cpuinfo, register_t * registers);

/// @brief CPUID is called with EAX=1
/// EAX contains Type, Family, Model and Stepping ID
/// EBX contains the Brand Index if supported, and the APIC ID
/// ECX/EDX contains feature information
void cpuid_write_proctype(cpuinfo_t * cpuinfo, register_t * registers);

/// @brief EAX=1, ECX contains a list of supported features.
void cpuid_feature_ecx(cpuinfo_t *, uint32_t);

/// @brief EAX=1, EDX contains a list of supported features.
void cpuid_feature_edx(cpuinfo_t *, uint32_t);

/// @brief Extract single byte from a register.
uint32_t cpuid_get_byte(const uint32_t reg,
                        const uint32_t position,
                        const uint32_t value);

/// @brief Index of brand strings.
char * cpuid_brand_index(register_t *);

/// @brief Brand string is contained in EAX, EBX, ECX and EDX.
char * cpuid_brand_string(register_t *);
