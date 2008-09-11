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

struct cpuinfo_generic {
    char *cpu_vendor;
    char *cpu_type;
    int cpu_family;
    int cpu_model;
    int cpu_csize;
};

/* Flags about what is supported or not */
struct cpuinfo_flags {
    ushort est  : 1;
    ushort tm2  : 1;
    ushort htt  : 1;
    ushort ss   : 1;
    ushort sse2 : 1;
    ushort sse  : 1;
    ushort fxsr : 1;
    ushort mmx  : 1;
    ushort acpi : 1;
    ushort ds   : 1;
    ushort pat  : 1;
    ushort mca  : 1;
    ushort pge  : 1;
    ushort mtrr : 1;
    ushort sep  : 1;
    ushort mce  : 1;
    ushort msr  : 1;
    ushort pse  : 1;
    ushort de   : 1;
    ushort fpu  : 1;
};

char cpu_vendor[12]; // This will contain the main string
/* Have a look into cpuid.c for this procedure */
void get_cpuid ();

#endif
