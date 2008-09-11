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

#ifndef _PIC8259_H
#define _PIC8259_H

#include <stddef.h>

#define MASTER_PORT 0x20
#define SLAVE_PORT 0xA0

#define MASTER_PORT_1 0x21
#define SLAVE_PORT_1 0xA1

#define ICW_1 0x11 

#define ICW_2_M 0x20
#define ICW_2_S 0x28

#define ICW_3_M 0x04
#define ICW_3_S 0x02

#define ICW_4 0x01

#define GET_IRR_STATUS 0x0b

#define EOI 0x20


typedef enum irq_types{
    TIMER,
    KEYBOARD,
    TO_SLAVE_PIC,
    COM2_4,
    COM1_3,
    LPT2,
    FLOPPY,
    REAL_TIME_CLOCK,
    AVAILABLE_1,
    AVAILABLE_2,
    AVAILABLE_3,
    AVAILABLE_4,
    MATH_CPU,
    FIRST_HD,
    SECOND_HD
} IRQ_t;

/*! 
    \struct IRQ_s
    \brief Struttura dati per gestire gli IRQ  condivisi
*/
typedef struct IRQ_struct {
    void (*IRQ_func)();/**< Puntatore alla funzione handler di un IRQ*/
    struct IRQ_struct *next;/**< Prossimo handler per questo IRQ*/
}IRQ_s;

extern byte master_cur_mask;
extern byte slave_cur_mask;

void init_IRQ();
void setup_IRQ();
int enable_IRQ(IRQ_t);
int disable_IRQ(IRQ_t);
int get_current_irq();
void add_IRQ_handler(int,void (*func)());
#endif
