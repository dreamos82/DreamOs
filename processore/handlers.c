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
void _globalException(int n, int error)
{
  switch (n) {

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
    page_fault_handler (error);
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
    _kputs ("Simd Floating Point Exception\n");
    break;

    default:
    _kputs ("Unknown exception\n");
    break;

  }
}

void _irqinterrupt(){
    int irqn;
    irqn = get_current_irq();  
    IRQ_s* tmpHandler; 
    if(irqn>=0) {
		if(irqn==2) {
			outportb(SLAVE_PORT,GET_IRR_STATUS);
			irqn = inportb(SLAVE_PORT);
			irqn = 8 + find_first_bit(irqn);			
		}
        tmpHandler = shareHandler[irqn];		
		if(tmpHandler!=0) {
	    	tmpHandler->IRQ_func();
	    	#ifdef DEBUG
	    		printf("2 - IRQ_func: %d, %d\n", tmpHandler->IRQ_func, tmpHandler);
	    	#endif
	    	while(tmpHandler->next!=NULL) {
	      		tmpHandler = tmpHandler->next;                           
	      		#ifdef DEBUG
	      			printf("1 - IRQ_func (_prova): %d, %d\n", tmpHandler->IRQ_func, tmpHandler);
	      		#endif
	      		if(tmpHandler!=0) tmpHandler->IRQ_func();
	    	}
	  } else printf("irqn: %d\n", irqn);
    }
    else printf("IRQ N: %d E' arrivato qualcosa che non so gestire ", irqn);
    if(irqn<=8 && irqn!=2) outportb(MASTER_PORT, EOI);
    else if(irqn<=16 || irqn==2){	  
      outportb(SLAVE_PORT, EOI);
      outportb(MASTER_PORT, EOI);
    }
}

void _int_rsv(){
	_kputs("Eccezione Riservata - PANIC\n");
	while(1);
}


