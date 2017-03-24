/*
 * Dreamos
 * handlers.c
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * Autore Ivan Gualandri
 * Prima versione: 19/05/2007
 */

#include <isr.h>
#include <video.h>
#include <port_io.h>
#include <scheduler.h>
#include <descriptor_tables.h>
#include <vm.h>
#include <debug.h>

irq_struct_t * shared_irq_handlers[IRQ_NUM];

void init_isr()
{
    for (uint32_t it = 0; it < IDT_SIZE; ++it)
    {
        if (it > 19 && it < 32)
        {
            kernel_add_interrupt_function_table(it, reserved_exception);
        }
        else if (it > 31 && it < 48)
        {
            kernel_add_interrupt_function_table(it, interrupt_handler);
        }
        else
        {
            kernel_add_interrupt_function_table(it, global_exception);
        }
    }
}

void global_exception(int n, int error)
{
    switch (n)
    {
        case DIVIDE_ERROR:
            _kputs("Divide Error\n");
            break;
        case DEBUG_EXC:
            _kputs("Debug Exception\n");
            break;
        case NMI_INTERRUPT:
            _kputs("NMI Exception\n");
            break;
        case OVERFLOW:
            _kputs("OverFlow Exception\n");
            break;
        case BOUND_RANGE_EXCEED:
            _kputs("Bound Exception\n");
            break;
        case DEV_NOT_AVL:
            _kputs("Device Not Available Exception\n");
            break;
        case COPROC_SEG_OVERRUN:
            _kputs("CoProcessor Segment Overrun\n");
            break;
        case BREAKPOINT:
            _kputs("BreakPoint\n");
            break;
        case INVALID_TSS:
            _kputs("Invalid TSS\n");
            break;
        case SEGMENT_NOT_PRESENT:
            _kputs("Segment Not Present\n");
            break;
        case STACK_SEGMENT_FAULT:
            _kputs("Stack Segment Fault Exception\n");
            break;
        case GENERAL_PROTECTION:
            _kputs("General Protection Exception\n");
            break;
        case INVALID_OPCODE:
            _kputs("Invalid Opcode Exception\n");
            break;
        case PAGE_FAULT:
            page_fault_handler(error);
            break;
        case INT_RSV:
            _kputs("Intel Reserved\n");
            break;
        case FLOATING_POINT_ERR:
            _kputs("Floating Point Exception\n");
            break;
        case ALIGNMENT_CHECK:
            _kputs("Alignment Check Exception\n");
            break;
        case MACHINE_CHECK:
            _kputs("Machine Check Exception\n");
            break;
        case DOUBLE_FAULT:
            _kputs("Double Fault Exception\n");
            break;
        case SIMD_FP_EXC:
            _kputs("Simd Floating Point Exception\n");
            break;
        default:
            _kputs("Unknown exception\n");
            break;
    }
}

void interrupt_handler(unsigned int esp)
{
    (void) esp;
    __asm__ __volatile__("cli;");
    // Get the current interrupt request number.
    int irq_num = irq_get_current();
    if (irq_num >= 0)
    {
        irq_struct_t * irq_struct = shared_irq_handlers[irq_num];
        if (irq_struct != NULL)
        {
            do
            {
                // Call the interrupt function.
                irq_struct->handler();
                // Move to the next interrupt function.
                irq_struct = irq_struct->next;
            } while (irq_struct != NULL);
        }
        else
        {
            dbg_print("Can't find irq_num: %d\n", irq_num);
        }
    }
    else
    {
        dbg_print("Can't handle irq_num: %d\n", irq_num);
    }
    if (irq_num <= 8 && irq_num != 2)
    {
        outportb(MASTER_PORT, EOI);
    }
    else if (irq_num <= 16 || irq_num == 2)
    {
        outportb(SLAVE_PORT, EOI);
        outportb(MASTER_PORT, EOI);
    }
    // Call the scheduler.
    schedule();
    __asm__ __volatile__("sti;");
    return;
}

void reserved_exception()
{
    _kputs("Reserved Exception - PANIC\n");
    while (1);
}
