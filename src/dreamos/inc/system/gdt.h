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
#ifndef _GDT_H
#define _GDT_H

#include <stddef.h>
#include <stdint.h>

/// Access flags, determines what ring this segment can be used in.
typedef enum __attribute__ ((__packed__)) gdt_access_option_t
{
    PRESENT = 0x80,
    KERNEL = 0x00,
    USER = 0x03,
    CODE = 0x10,
    DATA = 0x10,
} gdt_access_option_t;

/// Options for the second option.
typedef enum __attribute__ ((__packed__)) gdt_granularity_option_t
{
    GRANULARITY = 0x80,
    SZBITS = 0x40
} gdt_granularity_option_t;

/// @brief Initialize the
void kernel_init_gdt();

#endif
