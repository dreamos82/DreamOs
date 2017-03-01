/// @file   descriptor_tables.c

#include <descriptor_tables.h>

interrupt_handler_t IntTable[IDT_SIZE];

void init_descriptor_tables()
{
    init_gdt();
    init_idt();
}

void kernel_add_interrupt_function_table(int i,
                                         interrupt_handler_t handler)
{
    IntTable[i] = handler;
}
