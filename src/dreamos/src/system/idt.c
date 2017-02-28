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

#include <idt.h>
#include <gdt.h>
#include <handlers.h>
#include <video.h>

//Da perfezionare
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
// PAGEFAULT();
EXCEPTION(15);
EXCEPTION(16);
EXCEPTION_EC(17);
EXCEPTION(18);
EXCEPTION(19);
SYSCALL(80);

void (* IntTable[IDT_SIZE])();

// -----------------------------------------------------------------------------
// DEFINES

/// @brief This structure describes one interrupt gate.
typedef struct __attribute__ ((__packed__)) idt_descriptor_t
{
    /// Il puntatore all'istruzione.
    unsigned short int offset_low;
    /// Seleziona il segmento codice dalla GDT (nel nostro caso primo quindi
    /// 0x8, si specifica l'offset).
    unsigned short int seg_selector;
    /// E' un campo che deve essere sempre impostato a 0.
    unsigned char null_par;
    /// Le opzioni del selettore dela IDT.
    unsigned char options;
    /// Il puntatore all'istruzione.
    unsigned short int offset_high;
} idt_descriptor_t;

/// @brief A pointer structure used for informing the CPU about our IDT.
typedef struct __attribute__ ((__packed__)) idt_pointer_t
{
    /// La dimensione della IDT (in numero di entry).
    unsigned short int limit;
    /// L'indirizzo iniziale della IDT.
    unsigned int base;
} idt_pointer_t;

/// @brief Structure containing register values when the CPU was interrupted.
typedef struct
{
    // Data segment selector.
    uint32_t ds;
    // Pushed by pusha.
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    // Interrupt number and error code (if applicable).
    uint32_t int_no, err_code;
    // Pushed by the processor automatically.
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

// An interrupt handler. It is a pointer to a function which takes a pointer
// to a structure containing register values.
typedef void (* interrupt_handler_t)(registers_t *);

// -----------------------------------------------------------------------------
// FUNCTIONS

void set_idtr();

// -----------------------------------------------------------------------------
// DECLARATIONS

// The IDT itself.
idt_descriptor_t idt_descriptors[IDT_SIZE];
// Pointer structure to give to the CPU.
idt_pointer_t idt_pointer;
// Array of interrupt handler functions.
interrupt_handler_t interrupt_handlers[IDT_SIZE];

void init_idt()
{
    short int i;
    i = 0;
    while (i < IDT_SIZE)
    {
        idt_descriptors[i].offset_low = 0;
        idt_descriptors[i].seg_selector = 0;
        idt_descriptors[i].null_par = 0;
        idt_descriptors[i].options = 0;
        idt_descriptors[i].offset_high = 0;
        i++;
    }
    i = 0;
    kernel_init_interrupt_function_table();
    kernel_add_idt_seg(0, (uint32_t) INT_0, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(1, (uint32_t) INT_1, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(2, (uint32_t) INT_2, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(3, (uint32_t) INT_3, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(4, (uint32_t) INT_4, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(5, (uint32_t) INT_5, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(6, (uint32_t) INT_6, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(7, (uint32_t) INT_7, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(8, (uint32_t) INT_8, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(9, (uint32_t) INT_9, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(10, (uint32_t) INT_10, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(11, (uint32_t) INT_11, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(12, (uint32_t) INT_12, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(13, (uint32_t) INT_13, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(14, (uint32_t) INT_14, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(15, (uint32_t) INT_15, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(16, (uint32_t) INT_16, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(17, (uint32_t) INT_17, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(18, (uint32_t) INT_18, PRESENT | KERNEL, 0x8);
    kernel_add_idt_seg(19, (uint32_t) INT_19, PRESENT | KERNEL, 0x8);
    for (uint16_t it = 20; it < 32; ++it)
    {
        kernel_add_idt_seg(it, (uint32_t) IntTable[it], PRESENT | KERNEL, 0x8);
    }
    kernel_add_idt_seg(80, (uint32_t) INT_80, PRESENT | USER, 0x8);

    // Tell the CPU about our new IDT.
    set_idtr();
}

/**
 * @author Ivan Gualandri
 * @param short int i  -> indice della IDT
 * @param void (*gestore)() -> puntatore alla funzione che gestira' l'interrupt/Eccezione
 * @param options -> Le opzioni del descrittore (PRESENT,NOTPRESENT,KERNEL,USER)
 * @param seg_sel -> Il selettore del segmento della GDT
 * Questa funzione si occupa di aggiungere un nuovo segmento alla IDT.
 */
void kernel_add_idt_seg(short int i,
                        uint32_t base,
                        unsigned char options,
                        unsigned int seg_sel)
{
    idt_descriptors[i].offset_low = (base & 0xFFFF);
    idt_descriptors[i].offset_high = (base >> 16) & 0xFFFF;
    idt_descriptors[i].null_par = 0x00;
    idt_descriptors[i].seg_selector = seg_sel;
    idt_descriptors[i].options = options | INT32_GATE;
}

/**
  * @author Ivan Gualandri
  * @param IDT_Descriptor* addr l'indirizzo base della IDT
  * @param unsigned short int limit il limite della IDT.
  *
  * Questa funzione configura il registro IDTR per caricare la IDT in memoria.
  */
void __attribute__ ((noinline)) set_idtr()
{
    // Just like the GDT, the IDT has a "limit" field that is set to the last
    // valid byte in the IDT, after adding in the start position (i.e. size-1).
    idt_pointer.limit = sizeof(idt_descriptor_t) * IDT_SIZE - 1;
    idt_pointer.base = (uint32_t) &idt_descriptors;
    __asm__ __volatile__("lidt %0": :"g" (idt_pointer));
}
