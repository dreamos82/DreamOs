/*  DreamOS
    shell.c
    This file is part of DreamOSr.
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
*  Shell Coded by
*	Osiris 
*
*   jeek69 [at] katamail dot com
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
//#include <stdlib.h>


extern buddy_t* kbuddy;
void logo()
{
	printf("\n\n\t\t-------------------------- \n"
	       "\t\tThe Dream Operating System \n"
	       "\t\t      v0.01 pre-alpha      \n"
	       "\t\t-------------------------- \n");
	printf("\n\n\n\n\n\n");
}

void help()
{
	printf("help      - See the 'help' list to learn the DreamOS command now avaible\n"
	       "poweroff  - Turn off the machine\n"
	       "info      - See the system info about Memory and other stuffs like that\n"
               "kmalloc   - Test a basic kmalloc function\n"
               "do_fault  - Test a page_fault (WARNING: This hang the OS)\n"
               "aalogo    - Show an ascii art logo\n"
               "uname     - Print kernel version\n"
               "try_buddy - Try buddy mmu\n"
               "try_heap - Try heap mmu\n"
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

void shell(int argc, char *argv[])
{
	unsigned char cmd[256];
	//char *cmd=kmalloc(sizeof(cmd));
	//char *string=kmalloc(sizeof(string));
	unsigned char string[256];
	int a = 1, flag = 1;
	char *str1;

	printf("[?] Enter your username: ");
    char user[24];
    memset(user, 0, 24);   
	scanf ("%s",user);

	printf("\n\n\n\n\n\n");
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


		if (!(_kstrncmp(cmd,"help",4) ) )
		{
			printf("Available command: \n");
			help();
			cmd[a]=NULL;
			memset(cmd, 0, strlen(cmd));
		}

		if (!(_kstrncmp(cmd, "echo", 4) ) )
		{
			strncpy(string,cmd,strlen(cmd));
			memmove(string, string+5, strlen(string));
			printf("%s\n",string);
			memset(string+5, 0, strlen(string));
			memset(cmd, 0, strlen(cmd));
		}

		else if (!(_kstrncmp(cmd,"poweroff",8)))
		{
			printf("Poweroff..\n");
			poweroff();
			cmd[a]=NULL;
			memset(cmd, 0, strlen(cmd));
		}
		
		else if (!(_kstrncmp(cmd, "clear", 5)))
		{
			_kclear();
			cmd[a]=NULL;
			memset(cmd, 0, strlen(cmd));
		}

		else if (!(_kstrncmp(cmd, "uname",5)))
		{
			//if (_kstrncmp(argv[2], NULL,0) == -1)
			//{
				memmove(argv[2], argv[2]+6, strlen(argv[2]));
				//printf("%s\n", argv[2]);
			//}

			if (!(_kstrncmp(argv[2], "-a", 2)))
       			{
			printf("%s %s.%s.%s%s #1 beta CEST 2008 %s\n",NAME,VERSION,PATCHLEVEL,REV_NUM,EXTRAVERSION,cpu_vendor);
			}
			else { printf("%s\n", NAME); }
			memset(cmd, 0, strlen(cmd));

		}

		else if (!(_kstrncmp(cmd,"info",4)))
		{
			info();
			cmd[a]=NULL;
			memset(cmd, 0, strlen(cmd));
		}
		else if (!(_kstrncmp(cmd,"answer",6)))
      		  {
          		  printf("42\n");
          		  cmd[a]=NULL;
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
            cmd[a]=NULL;
        	memset(cmd, 0, strlen(cmd));

        }
        else if (!(_kstrncmp(cmd,"do_fault",8))){  
            char *prova;
            prova = 0xa0000000;
            *prova = 10;
            cmd[a]=NULL;
    	    memset(cmd, 0, strlen(cmd));
        }
        else if (!(_kstrncmp(cmd,"try_heap",8))){
            try_alloc();
            memset(cmd, 0, strlen(cmd));
        }
        else if (!(_kstrncmp(cmd,"try_buddy",9))){
             printf("L'indirizzo di kbuddy e': 0x%x\n", kbuddy);
             alloc_buddy(16, kbuddy);
             printf("New allocation\n\n");
             alloc_buddy(8, kbuddy);
            cmd[a] = NULL;
	        memset(cmd, 0, strlen(cmd));
        }
        else if (!(_kstrncmp(cmd,"aalogo",6))) aalogo();        
        else if(strlen(cmd)>0){
            printf("Unknown command: %s\n", cmd);
            memset(cmd, 0, strlen(cmd));
        }
        cmd[a]=NULL;
	memset(cmd, 0, strlen(cmd));

	}
}

void aalogo() {
printf("\t ____                     _____ _____\n");
printf("\t |    \\ ___ ___ ___ _____|     |   __|\n");
printf("\t |  |  |  _| -_| = |     |  |  |__   |\n");
printf("\t |____/|_| |___|__||_|_|_|_____|_____|\n");
printf("\t -.rev: \"%s\"\n",REV_NUM);
logo();
}


