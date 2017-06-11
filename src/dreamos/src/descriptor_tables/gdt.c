/// @file   gdt.c
/// @brief  Functions which manage the Global Descriptor Table (GDT).
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

#include "gdt.h"
#include "descriptor_tables.h"

/// The maximum dimension of the GDT.
#define GDT_SIZE 10

/// @brief This will be a function in gdt.s. We use this to properly
/// reload the new segment registers
/// @param _gdt_pointer addresss of the gdt.
extern void gdt_flush(uint32_t _gdt_pointer);

/// The GDT itself.
gdt_descriptor_t gdt[GDT_SIZE];
/// Pointer structure to give to the CPU.
gdt_pointer_t gdt_pointer;

void init_gdt()
{
    // Prepare GDT vector.
    for (uint32_t it = 0; it < GDT_SIZE; ++it)
    {
        gdt[it].limit_low = 0;
        gdt[it].base_low = 0;
        gdt[it].base_middle = 0;
        gdt[it].access = 0;
        gdt[it].granularity = 0;
        gdt[it].base_high = 0;
    }

    // Setup the GDT pointer and limit.
    // We have six entries in the GDT:
    //  - Two for kernel mode.
    //  - Two for user mode.
    //  - The NULL descriptor.
    //  - And one for the TSS (task state segment).
    // The limit is the last valid byte from the start of the GDT.
    // i.e. the size of the GDT - 1.
    gdt_pointer.limit = sizeof(gdt_descriptor_t) * 6 - 1;
    gdt_pointer.base = (uint32_t) &gdt;

    // ------------------------------------------------------------------------
    // NULL
    // ------------------------------------------------------------------------
    gdt_set_gate(0, 0, 0, 0, 0);

    // ------------------------------------------------------------------------
    // CODE
    // ------------------------------------------------------------------------
    // The base address is 0, the limit is 4GBytes, it uses 4KByte
    // granularity, uses 32-bit opcodes, and is a Code Segment descriptor.
    gdt_set_gate(1, 0, 0xFFFFFFFF,
                 PRESENT | KERNEL | CODE | 0x0A,
                 GRANULARITY | SZBITS | 0x0F);

    // ------------------------------------------------------------------------
    // DATA
    // ------------------------------------------------------------------------
    // It's EXACTLY the same as our code segment, but the descriptor type in
    // this entry's access byte says it's a Data Segment.
    gdt_set_gate(2, 0, 0xFFFFFFFF,
                 PRESENT | KERNEL | DATA | 0x02,
                 GRANULARITY | SZBITS | 0x0F);

    // ------------------------------------------------------------------------
    // USER MODE CODE
    // ------------------------------------------------------------------------
    gdt_set_gate(3, 0, 0xFFFFFFFF,
                 0xFA,
                 GRANULARITY | SZBITS | 0x0F);

    // ------------------------------------------------------------------------
    // USER MODE DATA
    // ------------------------------------------------------------------------
    gdt_set_gate(4, 0, 0xFFFFFFFF,
                 0xF2,
                 GRANULARITY | SZBITS | 0x0F);

    // Inform the CPU about the changes on the GDT.
    gdt_flush((uint32_t) &gdt_pointer);
}

void gdt_set_gate(uint8_t index,
                  uint32_t base,
                  uint32_t limit,
                  uint8_t _access,
                  uint8_t _granul)
{
    // Setup the descriptor base address.
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits.
    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].granularity = (limit >> 16) & 0x0F;

    // Finally, set up the granularity and access flags.
    gdt[index].access = _access;
    gdt[index].granularity |= _granul & 0xF0;
}