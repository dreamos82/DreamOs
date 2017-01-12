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
#include <8253.h>
#include <bitops.h>
#include <paging.h>

//IRQ_s *shareHandler[IRQ_NUM];
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
void irq_init(){
    int i;
    asm("cli");
// Inizializzo i 2 processori pic con ICw1 ICW2 ICW3 e ICW4
    outportb(MASTER_PORT,ICW_1);
    outportb(SLAVE_PORT, ICW_1);

    outportb(MASTER_PORT_1, ICW_2_M);
    outportb(SLAVE_PORT_1,  ICW_2_S);

    outportb(MASTER_PORT_1, ICW_3_M);
    outportb(SLAVE_PORT_1,  ICW_3_S) ;

    outportb(MASTER_PORT_1, ICW_4);
    outportb(SLAVE_PORT_1,  ICW_4);

    master_cur_mask = 0xFF;
    slave_cur_mask = 0xFF;

    outportb(MASTER_PORT_1, 0xFF);
	outportb(SLAVE_PORT_1,  0xFF);
		
    //outportb (0xFC, MASTER_PORT_1);
    irq_enable(KEYBOARD);    
    irq_enable(TIMER);
    irq_enable(TO_SLAVE_PIC);
          
    irq_setup();
    
        i=0;
    while(i<IRQ_NUM){
        shareHandler[i] = NULL;
        i++;
    }
    irq_add_handler(1, keyboard_isr);
    irq_add_handler(0, timer_pit_handler);
    asm("sti");                  
}

void irq_setup(){
    kernel_add_idt_seg(32, INT_32,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(33, INT_33,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(34, INT_34,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(35, INT_35,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(36, INT_37,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(38, INT_38,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(39, INT_39,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(40, INT_40,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(41, INT_41,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(42, INT_42,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(43, INT_43,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(44, INT_44,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(45, INT_45,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(46, INT_46,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(47, INT_47,PRESENT|KERNEL, 0x8);
    kernel_add_idt_seg(48, INT_48,PRESENT|KERNEL, 0x8);
}

/** This function, enable irqs on the pic.
  * @author Ivan Gualandri
  * @version 1.0
  * @param irq number of irq to enable.
  * @return 0 if all OK, -1 on errors
  * This function provide a tool for enabling irq from the pic processor. 
  */
int irq_enable (IRQ_t irq){
    byte cur_mask;
    byte new_mask;
    if(irq>=0 && irq<15) {
        if(irq<8){
            new_mask = ~(1<<irq);
            cur_mask = inportb(MASTER_PORT_1);
            outportb(MASTER_PORT_1, (new_mask&cur_mask));
            master_cur_mask=(new_mask&cur_mask);
            }
        else{
            irq = irq-8;
            new_mask = ~(1<<irq);
            cur_mask = inportb(SLAVE_PORT_1);
            outportb(SLAVE_PORT_1, (new_mask&cur_mask));
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
int irq_disable(IRQ_t irq){
    byte cur_mask;
    if(irq<15){
        if(irq<8){
            cur_mask = inportb(MASTER_PORT_1);
            cur_mask |= (1<< irq);
            outportb(MASTER_PORT_1, cur_mask&0xFF);
        }
        else {
            irq = irq-8;
            cur_mask = inportb(SLAVE_PORT_1);
            cur_mask |= (1<< irq);
            outportb(SLAVE_PORT_1, cur_mask&0xFF);
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
int irq_get_current(){
    int cur_irq;
    outportb(MASTER_PORT,GET_IRR_STATUS);
    cur_irq = inportb(MASTER_PORT);
    if(cur_irq == 4) {		
      outportb(SLAVE_PORT, GET_IRR_STATUS);
      cur_irq = inportb(SLAVE_PORT);
	  //printf("Slave irq number: %d\n", cur_irq);
	  return 8 + find_first_bit(cur_irq);
    }
    return find_first_bit(cur_irq);
}

/** This Function add an IRQ Handler to the givent irq number
  * @author Ivan Gualandri
  * @version 1.0
  * @param irq_number number of IRQ to serve (from 0 to 16)
  * @param func function to add
  * @return  None
  **/
void irq_add_handler(int irq_number, void (*func)()){
    if(irq_number<16){
        IRQ_s *tmpHandler;
        tmpHandler = shareHandler[irq_number];
        if(shareHandler[irq_number]==NULL){
             shareHandler[irq_number]= (IRQ_s*) kernel_alloc_page ();
             shareHandler[irq_number]->next = NULL;
             shareHandler[irq_number]->IRQ_func = func;
        }
        else {
            while(tmpHandler->next!=NULL) tmpHandler = tmpHandler->next;
            tmpHandler->next = (IRQ_s*) kernel_alloc_page ();
            tmpHandler = tmpHandler->next;
            tmpHandler->next = NULL;
            tmpHandler->IRQ_func = func;
        }
    }
    else return; 
}

