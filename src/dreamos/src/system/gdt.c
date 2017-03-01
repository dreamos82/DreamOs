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

// -----------------------------------------------------------------------------
// DEFINES

/// @brief Struttura dati che rappresenta un descrittore della GDT.
/// @details
/// We use the attribute "packed" to tell GCC not to change
/// any of the alignment in the structure.
typedef struct __attribute__ ((__packed__)) gdt_descriptor_t
{
    /// Primi 16 bit del limite (20 bit in totale)
    uint16_t limit_low;
    /// The lower 16 bits of the base.
    uint16_t base_low;
    /// The next 8 bits of the base.
    uint8_t base_middle;
    /// Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit).
    uint8_t options_1;
    /// SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)
    uint8_t options_2;
    /// The last 8 bits of the base.
    uint8_t base_high;
} gdt_descriptor_t;

/// @brief Struttura dati che serve a caricare la GDT nel GDTR.
typedef struct __attribute__ ((__packed__)) gdt_pointer_t
{
    /// La dimensione della GDT (in numero di entry).
    uint16_t limit;
    /// L'indirizzo iniziale della GDT;
    uint32_t base;
} gdt_pointer_t;

/// The maximum dimension of the GDT.
#define GDT_SIZE 10

// -----------------------------------------------------------------------------
// FUNCTIONS

/**
  * @author Ivan Gualandri
  * @version 1.0
  * @param pos indica la posizione all'interno della IDT
  * @param base indirizzo di memoria da cui inizia il segmento che stiamo definendo
  * @param limit indirizzo di memoria che delimita il segmento
  * @param opt1 in ordine: Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit)
  * @param opt2 in ordine: SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)
*/
void add_gdt_seg(int32_t position,
                 uint32_t base,
                 uint32_t limit,
                 uint8_t option_1,
                 uint8_t option_2);

extern void gdt_flush(uint32_t);

// -----------------------------------------------------------------------------
// DECLARATIONS

// The GDT itself.
gdt_descriptor_t gdt_descriptors[GDT_SIZE];
// Pointer structure to give to the CPU.
gdt_pointer_t gdt_pointer;

// -----------------------------------------------------------------------------
// IMPLEMENTATIONS

void kernel_init_gdt()
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
    add_gdt_seg(0, 0, 0, 0, 0);
    // --------------------------------
    // Code segment.
    add_gdt_seg(1, 0, 0xFFFFFFFF,
                PRESENT | KERNEL | CODE | 0x0A,
                GRANULARITY | SZBITS | 0x0F);
    // --------------------------------
    // Data segment.
    add_gdt_seg(2, 0, 0xFFFFFFFF,
                PRESENT | KERNEL | DATA | 0x02,
                GRANULARITY | SZBITS | 0x0F);
    // --------------------------------
    // User mode code segment.
    add_gdt_seg(3, 0, 0xFFFFFFFF,
                0xFA,
                GRANULARITY | SZBITS | 0x0F);
    // --------------------------------
    // User mode data segment.
    add_gdt_seg(4, 0, 0xFFFFFFFF,
                0xF2,
                GRANULARITY | SZBITS | 0x0F);

    // Inform the CPU about our GDT.
    gdt_flush((uint32_t) &gdt_pointer);
}

void add_gdt_seg(int32_t position,
                 uint32_t base,
                 uint32_t limit,
                 uint8_t option_1,
                 uint8_t option_2)
{
    gdt_descriptors[position].base_low = (base & 0xFFFF);
    gdt_descriptors[position].base_middle = (base >> 16) & 0xFF;
    gdt_descriptors[position].base_high = (base >> 24) & 0xFF;
    gdt_descriptors[position].limit_low = (limit & 0xFFFF);
    gdt_descriptors[position].options_1 = option_1;
    gdt_descriptors[position].options_2 = (limit >> 16) & 0x0F;
    gdt_descriptors[position].options_2 |= option_2 & 0xF0;
}
