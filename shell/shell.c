/*  DreamOS
    shell.c
    This file is part of Foobar.
    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
  Shell Coded by
	Osiris 
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
#include <kheap.h>

void logo()
{
	printf("\t-------------------------- \n"
	       "\tThe Dream Operating System \n"
	       "\t      v0.01 pre-alpha      \n"
	       "\t-------------------------- \n");
	printf("\n\n\n");
}

void help()
{
	printf("help     - See the 'help' list to learn the DreamOS command now avaible\n"
	       "poweroff - Turn off the machine\n"
	       "info     - See the system info about Memory and other staffs like that\n"
           "kmalloc  - Test a basic kmalloc function\n"
		);
}

void poweroff()
{
	/*asm(	"movl %0, %%eax\n"
		"int $0xff\n"
		: : "g"(1)); // valore di enum*/
    asm("hlt");
    printf("E' ora possibile spegnere il computer\n");
    while(1);
}

void info()
{
	printf(":==========: System info: :==========:\n\n");
        printf("Memory RAM: %d Mb", get_bmpelements());
        _kputs (LNG_CPU);
        _kcolor (4);
        _kgoto (61, _kgetline());
        _kputs (cpu_vendor);
        _kcolor(7);  
	printf("\n");  
        printf(LNG_FREERAM);
        _kgoto(60, _kgetline());
        printf(" %d kb\n", get_memsize()/1024);
        printf(LNG_FREEPAGE);
        _kgoto(60, _kgetline());
        printf(" %d\n", get_numpages());
        printf(LNG_BITMAP);
	_kgoto(60, _kgetline());
	printf(" %d", get_bmpelements());
        _kgoto(60, _kgetline());
        printf("\nSize of mem_area: %d\n", sizeof(mem_area));
        printf("Page Dir Entry n.0 is: %d\n", get_pagedir_entry(0));
        printf("Page Table Entry n.4 in Page dir 0 is: %d\n", get_pagetable_entry(0,4));
	printf(":==========: :==========: :==========:\n");
}

void shell(void)
{
	unsigned char cmd[256];
	//char *cmd=malloc(256); Dio maiale, Page Fault con il puntatore...
	int a = 1;
	logo();
	for (;;)
	{
		_kputs("root~# ");
	        scanf("%s",cmd);
		if (!(_kstrncmp(cmd,"help",4) ) )
		{
			printf("Available command: \n");
			help();
			cmd[a]=NULL;
		}

		else if (!(_kstrncmp(cmd,"poweroff",8)))
		{
			printf("Poweroff..\n");
			poweroff();
			cmd[a]=NULL;
		}
		
		else if (!(_kstrncmp(cmd,"info",4)))
		{
			info();
			cmd[a]=NULL;
		}
		else if (!(_kstrncmp(cmd,"answer",4)))
        {
            printf("42\n");
            cmd[a]=NULL;
        }
        else if (!(_kstrncmp(cmd,"kmalloc",4)))
        {
            printf("kmalloc try: ...\n");
            int *a;
            a = kmalloc(10*sizeof(int));
            int i = 0;
            while(i<10){
                a[i] = i*2;
                i++;
            }
            i=0;
            while(i<10) {
                printf("a[%d] = %d\n",i, a[i]);
                i++;
            }
            printf("Address of a: %d\n", a);
        }
	}

}

