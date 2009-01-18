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

void (*IntTable[IDT_SIZE])();
IDT_Descriptor IDT_Table[IDT_SIZE];

void init_idt(){
    short int i;
    i=0;
    while(i<IDT_SIZE){
        IDT_Table[i].offset_low = 0;
        IDT_Table[i].seg_selector = 0;
        IDT_Table[i].null_par = 0;
        IDT_Table[i].options = 0;
        IDT_Table[i].offset_high = 0;
        i++;
    }
    i=0;
    init_funcTable();
    add_IDTseg(0, INT_0,PRESENT|KERNEL, 0x8);
    add_IDTseg(1, INT_1,PRESENT|KERNEL, 0x8);
    add_IDTseg(2, INT_2,PRESENT|KERNEL, 0x8);
    add_IDTseg(3, INT_3,PRESENT|KERNEL, 0x8);
    add_IDTseg(4, INT_4,PRESENT|KERNEL, 0x8);
    add_IDTseg(5, INT_5,PRESENT|KERNEL, 0x8);
    add_IDTseg(6, INT_6,PRESENT|KERNEL, 0x8);
    add_IDTseg(7, INT_7,PRESENT|KERNEL, 0x8);
    add_IDTseg(8, INT_8,PRESENT|KERNEL, 0x8);
    add_IDTseg(9, INT_9,PRESENT|KERNEL, 0x8);
    add_IDTseg(10, INT_10,PRESENT|KERNEL, 0x8);
    add_IDTseg(11, INT_11,PRESENT|KERNEL, 0x8);
    add_IDTseg(12, INT_12,PRESENT|KERNEL, 0x8);
    add_IDTseg(13, INT_13,PRESENT|KERNEL, 0x8);
    add_IDTseg(14, INT_14,PRESENT|KERNEL, 0x8);
    add_IDTseg(15, INT_15,PRESENT|KERNEL, 0x8);
    add_IDTseg(16, INT_16,PRESENT|KERNEL, 0x8);
    add_IDTseg(17, INT_17,PRESENT|KERNEL, 0x8);
    add_IDTseg(18, INT_18,PRESENT|KERNEL, 0x8);
    add_IDTseg(19, INT_19,PRESENT|KERNEL, 0x8);
    add_IDTseg(80, INT_80,PRESENT|USER, 0x8);
    i=20;
    while(i<32){
    //    if(i==8) add_IDTseg(i, INT_8);
          add_IDTseg(i, IntTable[i], PRESENT|KERNEL, 0x8);
        i++;
    }
    set_idtr(IDT_Table, IDT_SIZE);
    //_kputs("IDT Initialization code goes Here\n");
}

/**
 * @author Ivan Gualandri
 * @param short int i  -> indice della IDT
 * @param void (*gestore)() -> puntatore alla funzione che gestira' l'interrupt/Eccezione
 * @param options -> Le opzioni del descrittore (PRESENT,NOTPRESENT,KERNEL,USER)
 * @param seg_sel -> Il selettore del segmento della GDT
 * Questa funzione si occupa di aggiungere un nuovo segmento alla IDT.
 */
void add_IDTseg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel){
    unsigned int indirizzo;
    indirizzo = (unsigned int)gestore;
    IDT_Table[i].offset_low= (indirizzo&0xFFFF);
    IDT_Table[i].null_par=0x00;
    IDT_Table[i].seg_selector = seg_sel;
    IDT_Table[i].options = options|0xE;
    IDT_Table[i].offset_high = indirizzo >> 16;
}

/**
  * @author Ivan Gualandri
  * @param IDT_Descriptor* addr l'indirizzo base della IDT
  * @param unsigned short int limit il limite della IDT.
  *
  * Questa funzione configura il registro IDTR per caricare la IDT in memoria.
  */
void set_idtr(IDT_Descriptor *addr, unsigned short int limit){
    IDT_Register idtr;
    idtr.idt_limit = limit*8;
    idtr.idt_base = (unsigned long)addr;
    __asm__ __volatile__("lidt %0": :"g" (idtr));
}
