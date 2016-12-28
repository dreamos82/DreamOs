/*
 * Dreamos
 * cpuid.h
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* CPUID definitions
 * Copyright by shainer <shainer@debianclan.org>
 * June 2007
 */

#ifndef _CPUID_H
#define _CPUID_H

#include <stddef.h>

#define ECX_FLAGS_SIZE 24
#define EDX_FLAGS_SIZE 32

struct cpuinfo_generic {
    char *cpu_vendor;
    char *cpu_type;
    int cpu_family;
    int cpu_model;
    int apic_id;
    int cpuid_ecx_flags[ECX_FLAGS_SIZE];
    int cpuid_edx_flags[EDX_FLAGS_SIZE];
    int is_brand_string;
    char *brand_string;
};

struct registri {
    int eax;
    int ebx;
    int ecx;
    int edx;
};

char cpu_vendor[12]; // This will contain the main string
void get_cpuid (struct cpuinfo_generic *);

void call_cpuid (struct registri *);
void cpuid_write_vendor (struct cpuinfo_generic *, struct registri *);
void cpuid_write_proctype (struct cpuinfo_generic *, struct registri *);
void cpuid_feature_ecx (struct cpuinfo_generic *, int);
void cpuid_feature_edx (struct cpuinfo_generic *, int);
int cpuid_get_byte (int, int, int);

char *cpuid_brand_index (struct registri *);
char *cpuid_brand_string (struct registri *);

struct cpuinfo_generic *sinfo;
#endif

