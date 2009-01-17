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
#include <idt.h>
#include <gdt.h>
#include <handlers.h>
#include <video.h>
#include <io.h>
#include <stdio.h>
#include <stddef.h>
#include <keyboard.h>
#include <fismem.h>
#include <8253.h>
#include <bitops.h>

IRQ_s *shareHandler[16];
// IRQ_s shareHandler[16];
// IRQ_s *tmpHandler;
byte master_cur_mask;
byte slave_cur_mask;

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

/**
  * Funzione che inizializza il processore pic 8259 che gestira' le interruzioni
  * @author Ivan Gualandri
  * @version 1.1
  */
void init_IRQ(){
    int i;
    asm("cli");
// Inizializzo i 2 processori pic con ICw1 ICW2 ICW3 e ICW4
    outportb(ICW_1,MASTER_PORT);
    outportb(ICW_1,SLAVE_PORT);

    outportb(ICW_2_M,MASTER_PORT_1);
    outportb(ICW_2_S,SLAVE_PORT_1);

    outportb(ICW_3_M,MASTER_PORT_1);
    outportb(ICW_3_S,SLAVE_PORT_1);

    outportb(ICW_4,MASTER_PORT_1);
    outportb(ICW_4,SLAVE_PORT_1);

    master_cur_mask = 0xFF;
    slave_cur_mask = 0xFF;

    outportb(0xFF,MASTER_PORT_1);
    enable_IRQ(KEYBOARD);
    enable_IRQ(TIMER);
    outportb(0xFF,SLAVE_PORT_1);

    outportb (0xFC, MASTER_PORT_1);
//<<<<<<< .mine
    //enable_IRQ (0);
//=======
 //     enable_IRQ (0);
//>>>>>>> .r13

    setup_IRQ();
    asm("sti");
    i=0;
    while(i<16){
        shareHandler[i] = NULL;
        i++;
    }
    add_IRQ_handler(1, keyboard_isr);
    add_IRQ_handler(0, PIT_handler);
}

void setup_IRQ(){
    add_IDTseg(32, INT_32,PRESENT|KERNEL, 0x8);
    add_IDTseg(33, INT_33,PRESENT|KERNEL, 0x8);
    add_IDTseg(34, INT_34,PRESENT|KERNEL, 0x8);
    add_IDTseg(35, INT_35,PRESENT|KERNEL, 0x8);
    add_IDTseg(36, INT_37,PRESENT|KERNEL, 0x8);
    add_IDTseg(38, INT_38,PRESENT|KERNEL, 0x8);
    add_IDTseg(39, INT_39,PRESENT|KERNEL, 0x8);
    add_IDTseg(40, INT_40,PRESENT|KERNEL, 0x8);
    add_IDTseg(41, INT_41,PRESENT|KERNEL, 0x8);
    add_IDTseg(42, INT_42,PRESENT|KERNEL, 0x8);
    add_IDTseg(43, INT_43,PRESENT|KERNEL, 0x8);
    add_IDTseg(44, INT_44,PRESENT|KERNEL, 0x8);
    add_IDTseg(45, INT_45,PRESENT|KERNEL, 0x8);
    add_IDTseg(46, INT_46,PRESENT|KERNEL, 0x8);
    add_IDTseg(47, INT_47,PRESENT|KERNEL, 0x8);
    add_IDTseg(48, INT_48,PRESENT|KERNEL, 0x8);
}

/** This function, enable irqs on the pic.
  * @author Ivan Gualandri
  * @version 1.0
  * @param irq number of irq to enable.
  * @return 0 if all OK, -1 on errors
  * This function provide a tool for enabling irq from the pic processor. 
  */
int enable_IRQ (IRQ_t irq){
    byte cur_mask;
    byte new_mask;
    if(irq>=0 && irq<15) {
        if(irq<8){
            new_mask = ~(1<<irq);
            cur_mask = inportb(MASTER_PORT_1);
            outportb((new_mask&cur_mask), MASTER_PORT_1);
            master_cur_mask=(new_mask&cur_mask);
            }
        else{
            irq = irq-8;
            new_mask = ~(1<<irq);
            cur_mask = inportb(SLAVE_PORT_1);
            outportb((new_mask&cur_mask), SLAVE_PORT_1);
            slave_cur_mask=(new_mask&cur_mask);
        }
        return 0;
    }
    return -1;
}

/** This function, disable irqs on the pic.
  * @author Ivan Gualandri
  * @version 1.0
  * @param irq number of irq to enable.
  * @return 0 if all OK, -1 on errors
  * This function provide a tool for enabling irq from the pic processor. 
  */
int disable_IRQ(IRQ_t irq){
    byte cur_mask;
    if(irq<15){
        if(irq<8){
            cur_mask = inportb(MASTER_PORT_1);
            cur_mask |= (1<< irq);
            outportb(cur_mask&0xFF, MASTER_PORT_1);
        }
        else {
            irq = irq-8;
            cur_mask = inportb(SLAVE_PORT_1);
            cur_mask |= (1<< irq);
            outportb(cur_mask&0xFF, SLAVE_PORT_1);
        }
        return 0;
    }
    return -1;
}

/** This Function return the number of current IRQ Request.
  * @author Ivan Gualandri
  * @version 1.0
  * @return  Number of IRQ + 1 currently serving. If 0 there are no IRQ
  **/
int get_current_irq(){
    int cur_irq;
    outportb(GET_IRR_STATUS, MASTER_PORT);
    cur_irq = inportb(MASTER_PORT);
//     if(cur_irq!=1) printf("%d\n", cur_irq);
    if(cur_irq == 0) {
      outportb(GET_IRR_STATUS, SLAVE_PORT);
      cur_irq = inportb(SLAVE_PORT);
    }
//     printf("%d\n", find_first_bit(cur_irq));
    return find_first_bit(cur_irq);
}

/** This Function add an IRQ Handler to the givent irq number
  * @author Ivan Gualandri
  * @version 1.0
  * @param irq_number number of IRQ to serve (from 0 to 16)
  * @param func function to add
  * @return  None
  **/
void add_IRQ_handler(int irq_number, void (*func)()){
    if(irq_number<16){
        IRQ_s *tmpHandler;
        tmpHandler = shareHandler[irq_number];
        if(shareHandler[irq_number]==NULL){
             shareHandler[irq_number]= (IRQ_s*) request_pages(sizeof(IRQ_s), NOT_ADD_LIST);
             shareHandler[irq_number]->next = NULL;
             shareHandler[irq_number]->IRQ_func = func;
        }
        else {
            while(tmpHandler->next!=NULL) tmpHandler = tmpHandler->next;
            tmpHandler->next = (IRQ_s*) request_pages(sizeof(IRQ_s), NOT_ADD_LIST);
            tmpHandler = tmpHandler->next;
            tmpHandler->next = NULL;
            tmpHandler->IRQ_func = func;
            #ifdef DEBUG
    //         printf("
            #endif
        }
    }
    else return; 
}

