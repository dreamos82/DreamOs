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

#include <handlers.h>
#include <idt.h>
#include <pic8259.h>
#include <video.h>
#include <io.h>
#include <stdio.h>
#include <keyboard.h>
#include <video.h>
#include <paging.h>

// #define DEBUG 1
void (*IntTable[IDT_SIZE])();
IRQ_s *shareHandler[16];
// IRQ_s *tmpHandler;
// IRQ_s shareHandler[16];
/** @author Ivan Gualandri
 *  @return none
 * Questa funzione si occupa di inizializzare la tabella di funzioni che gestiscono le 
 * interruzioni ed eccezioni.
 */
void init_funcTable(){
    int i;
    i=0;
    while(i<IDT_SIZE){
        if(i>19 && i<32) IntTable[i] = _int_rsv;
        else if(i>31 && i<48) add_Interrupt(i, _irqinterrupt);
        else IntTable[i] = _globalException;
        i++;
    }
}

/**
  * @author Ivan Gualandri
  * @return None
  * @param i posizione all'interno del vettore
  * @param func funzione da aggiungere
  *
  * Questa funzione aggiunge un handler di interruzione alla tabella per che contiene le funzioni di gestione chiamate dalle eccezioni/interruzioni della IDT
  */
void add_Interrupt(int i, void (*func)()){
	IntTable[i] = func;
}
/**
  * @author Ivan Gualandri
  * @version 1.0
  *
  * Questa funzione gestira a livello centralizzato le varie eccezione
  */
void _globalException(int n, int error){
	if(n==DIVIDE_ERROR)_kputs("Divide Error\n");
	else if(n==DEBUG_EXC) _kputs("Debug Exception\n");
	else if(n==NMI_INTERRUPT) _kputs("Nmi Exception\n");
	else if(n==OVERFLOW) _kputs("OverFlow Exception\n");
	else if(n==BOUND_RANGE_EXCEED) _kputs("Bound Exception\n");
	else if(n==DEV_NOT_AVL) _kputs("Device Not Available Exception\n");
	else if(n==COPROC_SEG_OVERRUN) _kputs("CoProcessor Segment Overrun\n");
	else if(n==BREAKPOINT) _kputs("BreakPoint\n");
	else if(n==INVALID_TSS) _kputs("Invalid TSS\n");
	else if(n==SEGMENT_NOT_PRESENT) _kputs("Segment Not Present\n");
	else if(n==STACK_SEGMENT_FAULT) _kputs("Stack Segment Fault Exception\n");
	else if(n==GENERAL_PROTECTION) _kputs("General Protection Exception\n");
    else if(n==INVALID_OPCODE) _kputs("Invalid Opcode Exception\n");
    else if(n==BOUND_RANGE_EXCEED) _kputs("Bound exception\n");
    else if(n==PAGE_FAULT) page_fault_handler (error);    
    else if(n==INT_RSV) _kputs("Intel Reserved\n");
    else if(n==FLOATING_POINT_ERR) _kputs("Floating Point Exception\n");
    else if(n==ALIGNMENT_CHECK) _kputs("Alignment Check Exception\n");             
    else if(n==MACHINE_CHECK) _kputs("Machine Check Exception\n");
    else if(n==DOUBLE_FAULT) {
        _kputs("DoubleFault Exception\n");
        _kputs("Ottimo direi\n");
    }
    else if(n==SIMD_FP_EXC) _kputs("Simd Floating Point Exception\n");
    else _kputs("Cose non buone avvengono\n");
}

void _irqinterrupt(){
    int irqn;
    irqn = get_current_irq();  
    IRQ_s* tmpHandler; 
    if(irqn>0) {
      switch(irqn){
	case 1: 
	  PIT_handler();
	  break;
	case 2:
	  keyboard_isr();
	  break;
      }
//         tmpHandler = shareHandler[irqn-1];
//         tmpHandler->IRQ_func();
//         #ifdef DEBUG
//         printf("2 - IRQ_func: %d, %d\n", tmpHandler->IRQ_func, tmpHandler);
//         #endif
//         while(tmpHandler->next!=NULL) {
//             tmpHandler = tmpHandler->next;                           
//             #ifdef DEBUG
//             printf("1 - IRQ_func (_prova): %d, %d\n", tmpHandler->IRQ_func, tmpHandler);
//             #endif
//             tmpHandler->IRQ_func();
//         }
    }
//  else printf("IRQ N: %d E' arrivato qualcosa che non so gestire ", irqn);
    if(irqn<=8) outportb(0x20, MASTER_PORT);
    else if(irqn<=16){
      outportb(0x20, SLAVE_PORT);
      outportb(0x20, MASTER_PORT);
    }
}

void _int_rsv(){
	_kputs("Eccezione Riservata - PANIC\n");
	while(1);
}

/*void _prova(){
    #ifdef DEBUG
    _kputs("Shared 1\n");
    #endif    
}*/

