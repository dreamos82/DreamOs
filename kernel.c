/***************************************************************************
 *            kernel.c
 *
 *  Sat Mar 31 07:47:17 2007
 *  Copyright  2007  Ivan Gualandri
 *  Email
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include <multiboot.h>
#include <kernel.h>
#include <stddef.h>
#include <video.h>
#include <pic8259.h>
#include <8253.h>
#include <gdt.h>
#include <idt.h>
#include <cpuid.h>
#include <stdio.h>
#include <string.h>
#include <fismem.h>
#include <io.h>
#include <keyboard.h>
#include <paging.h>
#include <use.h>

unsigned int *current_page_table;
multiboot_info_t *boot_informations;
asmlinkage void _start(){
//     boot_informations = boot_info;    
	main_loop();
	while(1);
}

int main_loop()
{
    _kclear();

    _kcolor('\012');
    _kputs(DREAMOS_VER);
    _kcolor('\007');
    _kputs(LNG_SITE);
    _kcolor('\011');
    _kputs(SITEURL);
    _kputs("\n");
    _kcolor('\007');
    _kputs("\n");

    _kputs(LNG_GDT);
    init_gdt();
    _kprintOK();

    outportb(0xFF, MASTER_PORT_1);
    outportb(0xFF, SLAVE_PORT_1);
    _kputs(LNG_IDT);
    asm("cli");   
    init_idt();
    _kprintOK();
    calcola_memoria();
    _kputs(LNG_PIC8259);
    init_IRQ();
    asm("sti");
    _kprintOK();
    init_paging();

    get_cpuid();
    _kputs (LNG_CPU);
    _kcolor (4);
    _kgoto (61, _kgetline());
    _kputs (cpu_vendor);
    _kputs ("\n\n");
    _kcolor(7);    
    printf(LNG_FREERAM);
    _kgoto(60, _kgetline());
    printf(" %d kb\n", get_memsize()/1024);
    printf(LNG_FREEPAGE);
    _kgoto(60, _kgetline());
    printf(" %d\n", get_numpages());
    printf(LNG_BITMAP);
    _kgoto(60, _kgetline());
    printf(" %d\n", get_bmpelements());
    printf("Size of mem_area: %d\n", sizeof(mem_area));
    init_mem();
    asm ("movl $0, %eax\n"
	 "movl $35, %ebx\n"
	 "int $80");

    configure_PIT ();


    printf("\n");
    printf("Page Dir Entry n.0 is: %d\n", get_pagedir_entry(0));
    printf("Page Table Entry n.4 in Page dir 0 is: %d\n", get_pagetable_entry(0,4));
    printf("----\n");

    while(1);
}
