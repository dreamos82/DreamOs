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
#include <shell.h>

unsigned int *current_page_table;
extern unsigned int end;
// multiboot_info_t *boot_informations;
asmlinkage void _start(struct multiboot_info *boot_info){
//     boot_informations = boot_info;    
	main_loop(boot_info);
	while(1);
}

int main_loop(struct multiboot_info *boot_info)
{    

    _kclear();
    syscall_init();
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
    init_mem();    
    _kputs(LNG_PIC8259);
    init_IRQ();
    asm("sti");
    _kprintOK();   
    printf("End: %d\n", end);
    init_paging();    

    printf(LNG_PIT8253);
    //configure_PIT ();
    _kprintOK();

    /*asm ("movl $0, %eax\n"
     "movl $37, %ebx\n"
     "int $80");    */
    printf("Memory (upper) amount-> %d kb \n", boot_info->mem_upper);
    get_cpuid();    
    printf("\n");    
    printf("----\n");
    printf("Loading the shell..\n");
    printf("[+] Loading complete!!\n\n");    
    printf("End: %d Address: 0x%x\n", end, &end);

    shell();
}
