/***************************************************************************
 *            shell.c
 *
 *  Sun Apr  1 00:16:03 2007
 *  Copyright  2007  Osiris
 *  Email	jeek69 [at] katamail dot com
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
#include <kheap.h>
#include <buddy.h>
#include <version.h>

extern buddy_t* kbuddy;


void shell(int argc, char *argv[])
{
	unsigned char cmd[256];
	unsigned char string[256];
	int flag = 1;
	char *str1, *user = kmalloc(24);
	memset(user, 0, strlen(user));

	printf("[?] Enter your username: ");
	scanf ("%s",user);

	while (1)
	{
		if (!(_kstrncmp(user, "", 1) ) )
		{
			printf("[?] Enter your username: ");
			scanf ("%s",user);
			printf("[x] Please, insert your username :)\n");
		}
		else
			break;	
	}
	
	_kclear();
	aalogo();
	printf("\n\n\n\n");

        shell_mess = 7;
        argc=1;

	for (;;)
	{
	    printf("%s~# ",user);
	    scanf("%s",cmd);

	    // Inizio funzione argc e argv --->

	   while (1)
	   {
		if (flag) {
			str1 = strtok(cmd, " ");
			flag = 0;
		} 
		else 
		{
			str1 = strtok(NULL, " ");
		}

		if (str1 == NULL)
		{
			break;
		}

		argv[argc] = (char *)kmalloc(strlen(str1) + 1); // Qui ho usato kmalloc() perchè non c'è malloc() 

		strncpy(argv[argc], str1, strlen(str1));
		argc++;
	    }
	    // fine argomentazione.. facile no ? :) --> Osiris r0x :P
	    // Si, d'accordo, ora dobbiamo includerlo in una libreria.. :)


		if (!(_kstrncmp(cmd,"help",4) ) )
		{
			printf("Available command: \n");
			help();
		}

		else if (!(_kstrncmp(cmd, "echo", 4) ) )
		{
			strncpy(string,cmd,strlen(cmd));
			memmove(string, string+5, strlen(string));
			printf("%s\n",string);
			memset(string+5, 0, strlen(string));
		}

		else if (!(_kstrncmp(cmd,"poweroff",8)))
		{
			printf("Poweroff..\n");
			poweroff();
		}
		
		else if (!(_kstrncmp(cmd, "clear", 5)))
		{
			_kclear();
		}

		else if (!(_kstrncmp(cmd, "uname",5)))
		{
			if (argv[2] != " ")
			{
				memmove(argv[2], argv[2]+6, strlen(argv[2]));
			}

			if (!(_kstrncmp(argv[2], "-a", 2)) || !(_kstrncmp(argv[2], "--all", 5)))
       			{
				printf("%s %s.%s.%s%s #1 beta CEST 2008 %s\n",NAME,VERSION,PATCHLEVEL,REV_NUM,EXTRAVERSION,cpu_vendor);
			}

			else if (!(_kstrncmp(argv[2], "-r", 2)) || !(_kstrncmp(argv[2], "--rev", 5)))
       			{
			 	printf("%s.%s.%s%s\n",VERSION,PATCHLEVEL,REV_NUM,EXTRAVERSION);
			}

			else if (!(_kstrncmp(argv[2], "-h", 2) ) || !(_kstrncmp(argv[2], "--help", 6)))
       			{
			 	uname_help();
			}
				
			else if (!(_kstrncmp(argv[2], "-i", 2)) || !(_kstrncmp(argv[2], "--info", 6)))
			{
				info();
			}

			if (!(_kstrncmp(argv[2], '/0', 1)))
			{
				printf("%s\n"
				       "For more info about this tool, please do 'uname --help'\n",NAME);
			}


		}

		else if (!(_kstrncmp(cmd,"answer",6)))
      		{
          		  printf("42\n");
			  memset(cmd, 0, strlen(cmd));
        	}

        else if (!(_kstrncmp(cmd,"kmalloc",7)))
        {
            printf("kmalloc try: ...\n");
            int *b;
            b = (int) kmalloc(10*sizeof(int));
            int i = 0;
            while(i<10){
                b[i] = i*2;
                i++;
            }
            i=0;
            while(i<10) {
                printf("b[%d] = %d\n",i, b[i]);
                i++;
            }
            printf("Address of a: %d\n", b);

        }

        else if (!(_kstrncmp(cmd,"do_fault",8)))
	{  
             printf ("Genero un pagefault...\n");
             char *prova;
             prova = 0xa0000000;
             *prova = 10;
        }

        else if (!(_kstrncmp(cmd,"try_heap",8))){
            try_alloc();
        }

        else if (!(_kstrncmp(cmd,"try_buddy",9))){
             printf("L'indirizzo di kbuddy e': 0x%x\n", kbuddy);
             alloc_buddy(16, kbuddy);
             printf("New allocation\n\n");
             alloc_buddy(8, kbuddy);
        }

        else if (!(_kstrncmp(cmd,"aalogo",6))) 
		aalogo();
        
        else if (strlen(cmd)>0)
	{
            printf("Unknown command: %s\n", cmd);
            memset(cmd, 0, strlen(cmd));
        }

	memset(string+5, 0, strlen(string));
	memset(cmd, 0, strlen(cmd));
	memset(argv[2], 0, strlen(argv[2]));

	}
}

void aalogo() {
_kcolor (4);
printf("\t\t ____           _____          _____ _____\n");
printf("\t\t|    \\ ___ ___ |  _  | _______|     |   __|\n");
printf("\t\t|  |  |  _| -_|| |_| ||       |  |  |__   |\n");
printf("\t\t|____/|_| |___||_| |_||_|\134_/|_|_____|_____|\n");
_kcolor (6);
printf("\t\tRevision: \"%s\"\n",REV_NUM);
_kcolor(7);
logo();
}

void logo()
{
	_kcolor (5);
	printf("\n");
	printf("\t\t\t The Dream Operating System \n"
	       "\t\t           v0.01%s pre-alpha      \n\n"

	  "\t\tHave you ever dreammed an operation system \n"
	  "\t\tof your Dream? I think yep, becuse You can \n"
	              "\t\t\t\tChange Happens!        \n",REV_NUM);
	
	printf("\n\n\n\n");
	_kcolor(7);
}

void uname_help() 
{
	//_kcolor (5);
	printf("Uname function allow you to see the kernel and system information.\n");
	printf("Function avaibles:\n");
	//_kcolor (2);
	printf("1) '-a'   - Kernel version and processor type\n"
  	       "2) '-r'   - Only the kernel version\n"
  	       "3) '-i'   - All info of system and kernel\n");	
	_kcolor (7);
}

void help()
{
	printf("help      - See the 'help' list to learn the DreamOS command now avaible\n"
	       "poweroff  - Turn off the machine\n"
               "kmalloc   - Test a basic kmalloc function\n"
               "do_fault  - Test a page_fault (WARNING: This hang the OS)\n"
               "aalogo    - Show an ascii art logo\n"
               "uname     - Print kernel version, try uname --help for more info\n"
               "try_buddy - Try buddy mmu\n"
               "try_heap  - Try heap mmu\n"
	       "echo      - Print some lines of text\n");
}

void poweroff()
{
	/*asm(	"movl %0, %%eax\n"
		"int $0xff\n"
		: : "g"(1)); // valore di enum*/
    asm("sti");
    printf("E' ora possibile spegnere il computer.\n");
    while(1);
}

void info()
{
	printf("\n:==========: :System info: :==========:\n\n");
	
	// Kernel info
	printf( "Version: '%s'\n"
		"Patchlevel: '%s'\n"
		"Extraversion: '%s'\n"
		"Name: '%s'\n"
		"Revision: '%s'\n",VERSION,PATCHLEVEL,EXTRAVERSION,NAME,REV_NUM);

	// CPU Info
	_kputs (LNG_CPU);
        _kcolor (4);
        _kgoto (61, _kgetline());
        _kputs (cpu_vendor);
        _kcolor(7);
	printf("\n");

	// Memory RAM Info
        printf("Memory RAM: ");
	_kgoto(60, _kgetline());
	printf(" %d Kb\n", get_memsize()/1024);

	// Memory free RAM Info
	printf(LNG_FREERAM);
        _kgoto(60, _kgetline());
        printf(" %d Kb\n", get_numpages());
  
	printf("\n");

	// Bitmap Info
        printf("Number bitmap's elements: ");
	_kgoto(60, _kgetline());
	printf(" %d", get_bmpelements());
        _kgoto(60, _kgetline());

	// Mem_area Info
        printf("\nSize of mem_area: ");
	_kgoto(60, _kgetline());	
	printf(" %d\n", sizeof(mem_area));

	// Page Dir Info
        printf("Page Dir Entry n.0 is: ");
	_kgoto(60, _kgetline());
	printf(" %d\n", get_pagedir_entry(0));
        
	// Page Table Info
	printf("Page Table Entry n.4 in Page dir 0 is: ");
	_kgoto(60, _kgetline());	
	printf(" %d\n", get_pagetable_entry(0,4));

	printf("\n:==========: :===========: :==========:\n\n");
}

