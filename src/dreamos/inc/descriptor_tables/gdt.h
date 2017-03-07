/*
 * Dreamos
 * gdt.h
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
#pragma once

#include <stdint.h>

/// The maximum dimension of the GDT.
#define GDT_SIZE 10

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

/// @brief Initialise the global descriptor table.
void init_gdt();

/**
  * @author Ivan Gualandri
  * @version 1.0
  * @param pos indica la posizione all'interno della IDT
  * @param base indirizzo di memoria da cui inizia il segmento che stiamo definendo
  * @param limit indirizzo di memoria che delimita il segmento
  * @param opt1 in ordine: Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit)
  * @param opt2 in ordine: SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)
*/
void gdt_set_gate(uint8_t index,
                  uint32_t base,
                  uint32_t limit,
                  uint8_t option_1,
                  uint8_t option_2);
