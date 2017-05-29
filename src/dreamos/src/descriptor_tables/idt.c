/*
 * Dreamos
 * idt.c
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

#include "idt.h"
#include "isr.h"
#include "descriptor_tables.h"

// Lets us access our ASM functions from our C code.
extern void idt_flush(uint32_t idt_pointer);

// The IDT itself.
idt_descriptor_t idt_descriptors[IDT_SIZE];
// Pointer structure to give to the CPU.
idt_pointer_t idt_pointer;

void init_idt()
{
    // Prepare IDT vector.
    for (uint32_t it = 0; it < IDT_SIZE; ++it)
    {
        idt_descriptors[it].offset_low = 0;
        idt_descriptors[it].seg_selector = 0;
        idt_descriptors[it].null_par = 0;
        idt_descriptors[it].options = 0;
        idt_descriptors[it].offset_high = 0;
    }

    // Just like the GDT, the IDT has a "limit" field that is set to the last
    // valid byte in the IDT, after adding in the start position (i.e. size-1).
    idt_pointer.limit = sizeof(idt_descriptor_t) * IDT_SIZE - 1;
    idt_pointer.base = (uint32_t) &idt_descriptors;

    init_isr();

    idt_set_gate(0, INT_0, PRESENT | KERNEL, 0x8);
    idt_set_gate(1, INT_1, PRESENT | KERNEL, 0x8);
    idt_set_gate(2, INT_2, PRESENT | KERNEL, 0x8);
    idt_set_gate(3, INT_3, PRESENT | KERNEL, 0x8);
    idt_set_gate(4, INT_4, PRESENT | KERNEL, 0x8);
    idt_set_gate(5, INT_5, PRESENT | KERNEL, 0x8);
    idt_set_gate(6, INT_6, PRESENT | KERNEL, 0x8);
    idt_set_gate(7, INT_7, PRESENT | KERNEL, 0x8);
    idt_set_gate(8, INT_8, PRESENT | KERNEL, 0x8);
    idt_set_gate(9, INT_9, PRESENT | KERNEL, 0x8);
    idt_set_gate(10, INT_10, PRESENT | KERNEL, 0x8);
    idt_set_gate(11, INT_11, PRESENT | KERNEL, 0x8);
    idt_set_gate(12, INT_12, PRESENT | KERNEL, 0x8);
    idt_set_gate(13, INT_13, PRESENT | KERNEL, 0x8);
    idt_set_gate(14, INT_14, PRESENT | KERNEL, 0x8);
    idt_set_gate(15, INT_15, PRESENT | KERNEL, 0x8);
    idt_set_gate(16, INT_16, PRESENT | KERNEL, 0x8);
    idt_set_gate(17, INT_17, PRESENT | KERNEL, 0x8);
    idt_set_gate(18, INT_18, PRESENT | KERNEL, 0x8);
    idt_set_gate(19, INT_19, PRESENT | KERNEL, 0x8);
    for (uint8_t it = 20; it < 32; ++it)
    {
        idt_set_gate(it, IntTable[it], PRESENT | KERNEL, 0x8);
    }
    idt_set_gate(80, INT_80, PRESENT | USER, 0x8);

    // Tell the CPU about our new IDT.
    idt_flush((uint32_t) &idt_pointer);
}

void idt_set_gate(uint8_t index,
                  interrupt_handler_t handler,
                  uint16_t options,
                  uint8_t seg_sel)
{
    uint32_t base_prt = (uint32_t) handler;
    idt_descriptors[index].offset_low = (base_prt & 0xFFFF);
    idt_descriptors[index].offset_high = (base_prt >> 16) & 0xFFFF;
    idt_descriptors[index].null_par = 0x00;
    idt_descriptors[index].seg_selector = seg_sel;
    idt_descriptors[index].options = options | INT32_GATE;
}

#if 0
EXCEPTION(0);
EXCEPTION(1);
EXCEPTION(2);
EXCEPTION(3);
EXCEPTION(4);
EXCEPTION(5);
EXCEPTION(6);
EXCEPTION(7);
EXCEPTION_EC(8);
EXCEPTION(9);
EXCEPTION_EC(10);
EXCEPTION_EC(11);
EXCEPTION_EC(12);
EXCEPTION_EC(13);
EXCEPTION_EC(14);
EXCEPTION(15);
EXCEPTION(16);
EXCEPTION_EC(17);
EXCEPTION(18);
EXCEPTION(19);
SYSCALL(80);
#endif