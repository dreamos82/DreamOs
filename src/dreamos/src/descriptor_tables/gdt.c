/*
 * Dreamos
 * gdt.c
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * Autore Ivan Gualandri
 * Prima versione: 27/10/2003
 * Contiene le definizioni di alcuni tipi di dato :D
 */

#include <gdt.h>
#include <descriptor_tables.h>

// Lets us access our ASM functions from our C code.
extern void gdt_flush(uint32_t idt_pointer);

// The GDT itself.
gdt_descriptor_t gdt_descriptors[GDT_SIZE];
// Pointer structure to give to the CPU.
gdt_pointer_t gdt_pointer;

void init_gdt()
{
    // Prepare GDT vector.
    for (uint32_t it = 0; it < GDT_SIZE; ++it)
    {
        gdt_descriptors[it].limit_low = 0;
        gdt_descriptors[it].base_low = 0;
        gdt_descriptors[it].base_middle = 0;
        gdt_descriptors[it].options_1 = 0;
        gdt_descriptors[it].options_2 = 0;
        gdt_descriptors[it].base_high = 0;
    }

    // We have six entries in the GDT:
    //  - Two for kernel mode.
    //  - Two for user mode.
    //  - The NULL descriptor.
    //  - And one for the TSS (task state segment).
    // The limit is the last valid byte from the start of the GDT.
    // i.e. the size of the GDT - 1.
    gdt_pointer.limit = sizeof(gdt_descriptor_t) * 6 - 1;
    gdt_pointer.base = (uint32_t) &gdt_descriptors;

    // --------------------------------
    // Null segment.
    gdt_set_gate(0, 0, 0, 0, 0);
    // --------------------------------
    // Code segment.
    gdt_set_gate(1, 0, 0xFFFFFFFF,
                 PRESENT | KERNEL | CODE | 0x0A,
                 GRANULARITY | SZBITS | 0x0F);
    // --------------------------------
    // Data segment.
    gdt_set_gate(2, 0, 0xFFFFFFFF,
                 PRESENT | KERNEL | DATA | 0x02,
                 GRANULARITY | SZBITS | 0x0F);
    // --------------------------------
    // User mode code segment.
    gdt_set_gate(3, 0, 0xFFFFFFFF,
                 0xFA,
                 GRANULARITY | SZBITS | 0x0F);
    // --------------------------------
    // User mode data segment.
    gdt_set_gate(4, 0, 0xFFFFFFFF,
                 0xF2,
                 GRANULARITY | SZBITS | 0x0F);

    // Inform the CPU about our GDT.
    gdt_flush((uint32_t) &gdt_pointer);
}

void gdt_set_gate(uint8_t index,
                  uint32_t base,
                  uint32_t limit,
                  uint8_t option_1,
                  uint8_t option_2)
{
    gdt_descriptors[index].base_low = (base & 0xFFFF);
    gdt_descriptors[index].base_middle = (base >> 16) & 0xFF;
    gdt_descriptors[index].base_high = (base >> 24) & 0xFF;
    gdt_descriptors[index].limit_low = (limit & 0xFFFF);
    gdt_descriptors[index].options_1 = option_1;
    gdt_descriptors[index].options_2 = (limit >> 16) & 0x0F;
    gdt_descriptors[index].options_2 |= option_2 & 0xF0;
}