/*
 * Dreamos
 * pic8259.h
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

#include <pic8259.h>
#include <isr.h>
#include <video.h>
#include <port_io.h>
#include <stdio.h>
#include <keyboard.h>
#include <timer.h>
#include <bitops.h>
#include <paging.h>
#include <mouse.h>
#include <language.h>
#include <descriptor_tables.h>
#include "irqflags.h"

byte_t master_cur_mask;

byte_t slave_cur_mask;

void pic8259_init_irq()
{
    // Disable the IRQs.
    irq_disable();

    // Set the masks for the master and slave.
    master_cur_mask = 0xFF;
    slave_cur_mask = 0xFF;

    // Inizializzo i 2 processori pic con ICw1 ICW2 ICW3 e ICW4
    outportb(MASTER_PORT, ICW_1);
    outportb(SLAVE_PORT, ICW_1);

    outportb(MASTER_PORT_1, ICW_2_M);
    outportb(SLAVE_PORT_1, ICW_2_S);

    outportb(MASTER_PORT_1, ICW_3_M);
    outportb(SLAVE_PORT_1, ICW_3_S);

    outportb(MASTER_PORT_1, ICW_4);
    outportb(SLAVE_PORT_1, ICW_4);

    outportb(MASTER_PORT_1, master_cur_mask);
    outportb(SLAVE_PORT_1, slave_cur_mask);

    // Registers the interrupt functions inside the IDT.
    idt_set_gate(32, INT_32, PRESENT | KERNEL, 0x8);
    idt_set_gate(33, INT_33, PRESENT | KERNEL, 0x8);
    idt_set_gate(34, INT_34, PRESENT | KERNEL, 0x8);
    idt_set_gate(35, INT_35, PRESENT | KERNEL, 0x8);
    idt_set_gate(36, INT_37, PRESENT | KERNEL, 0x8);
    idt_set_gate(38, INT_38, PRESENT | KERNEL, 0x8);
    idt_set_gate(39, INT_39, PRESENT | KERNEL, 0x8);
    idt_set_gate(40, INT_40, PRESENT | KERNEL, 0x8);
    idt_set_gate(41, INT_41, PRESENT | KERNEL, 0x8);
    idt_set_gate(42, INT_42, PRESENT | KERNEL, 0x8);
    idt_set_gate(43, INT_43, PRESENT | KERNEL, 0x8);
    idt_set_gate(44, INT_44, PRESENT | KERNEL, 0x8);
    idt_set_gate(45, INT_45, PRESENT | KERNEL, 0x8);
    idt_set_gate(46, INT_46, PRESENT | KERNEL, 0x8);
    idt_set_gate(47, INT_47, PRESENT | KERNEL, 0x8);
    idt_set_gate(48, INT_48, PRESENT | KERNEL, 0x8);

    for (uint32_t i = 0; i < IRQ_NUM; ++i)
    {
        shared_irq_handlers[i] = NULL;
    }
    pic8259_irq_enable(TO_SLAVE_PIC);

    // Install the keyboard.
    printf(" * "LNG_KEYBOARD_SETUP);
    keyboard_install();
    video_print_ok();

    // Install the mouse.
    printf(" * "LNG_MOUSE_SETUP);
    mouse_install();
    video_print_ok();

    // Install the timer.
    printf(" * "LNG_TIMER_SETUP);
    timer_install();
    video_print_ok();

    outportb(0xFF, MASTER_PORT_1);
    outportb(0xFF, SLAVE_PORT_1);

    // Re-Enable the IRQs.
    irq_enable();
}

int pic8259_irq_enable(irq_type_t irq)
{
    byte_t cur_mask;
    byte_t new_mask;
    if (irq < 15)
    {
        if (irq < 8)
        {
            new_mask = ~(1 << irq);
            cur_mask = inportb(MASTER_PORT_1);
            outportb(MASTER_PORT_1, (new_mask & cur_mask));
            master_cur_mask = (new_mask & cur_mask);
        } else
        {
            irq -= 8;
            new_mask = ~(1 << irq);
            cur_mask = inportb(SLAVE_PORT_1);
            outportb(SLAVE_PORT_1, (new_mask & cur_mask));
            slave_cur_mask = (new_mask & cur_mask);
        }
        return 0;
    }
    return -1;
}

int pic8259_irq_disable(irq_type_t irq)
{
    byte_t cur_mask;
    if (irq < 15)
    {
        if (irq < 8)
        {
            cur_mask = inportb(MASTER_PORT_1);
            cur_mask |= (1 << irq);
            outportb(MASTER_PORT_1, cur_mask & 0xFF);
        } else
        {
            irq = irq - 8;
            cur_mask = inportb(SLAVE_PORT_1);
            cur_mask |= (1 << irq);
            outportb(SLAVE_PORT_1, cur_mask & 0xFF);
        }
        return 0;
    }
    return -1;
}

int pic8259_irq_get_current()
{
    outportb(MASTER_PORT, GET_IRR_STATUS);
    int cur_irq = inportb(MASTER_PORT);
    if (cur_irq == 4)
    {
        outportb(SLAVE_PORT, GET_IRR_STATUS);
        cur_irq = inportb(SLAVE_PORT);
        return 8 + find_first_bit(cur_irq);
    }
    return find_first_bit(cur_irq);
}

void pic8259_irq_install_handler(uint32_t irq_number, interrupt_handler_t handler)
{
    if (irq_number >= IRQ_NUM)
    {
        return;
    }

    irq_struct_t *tmpHandler;
    tmpHandler = shared_irq_handlers[irq_number];
    if (shared_irq_handlers[irq_number] == NULL)
    {
        shared_irq_handlers[irq_number] = (irq_struct_t *) kernel_alloc_page();
        shared_irq_handlers[irq_number]->next = NULL;
        shared_irq_handlers[irq_number]->handler = handler;
    } else
    {
        while (tmpHandler->next != NULL)
        {
            tmpHandler = tmpHandler->next;
        }
        tmpHandler->next = (irq_struct_t *) kernel_alloc_page();
        tmpHandler = tmpHandler->next;
        tmpHandler->next = NULL;
        tmpHandler->handler = handler;
    }
}

#if 0
IRQ(32);
IRQ(33);
IRQ(34);
IRQ(35);
IRQ(36);
IRQ(37);
IRQ(38);
IRQ(39);
IRQ(40);
IRQ(41);
IRQ(42);
IRQ(43);
IRQ(44);
IRQ(45);
IRQ(46);
IRQ(47);
IRQ(48);
#endif