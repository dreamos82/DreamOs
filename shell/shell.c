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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
#include <version.h>
#include <cpuid.h>
    
extern heap_t *kheap;
int argc;
char **argv;

void options(char *com)
{
  int i=0;
  argc=0;

  for (; *com; com++)
  {
    argv[argc] = (char *)kmalloc(sizeof(char) * 10);
    while (*com != ' ') {
      *(argv[argc] + i) = *com++;
      i++;
    } 
    *(argv[argc] + i) = '\0';
    argc++;
    i=0;
  }
  argv[argc] = '\0';
}

void shell()
{
	char cmd[256];
	char string[256];
	char *user = kmalloc(24);
	memset(user, 0, strlen(user));

	printf("[?] Enter your username: ");
	scanf ("%s",user);

	while (!strlen(user))
	{		
			printf("[?] Enter your username: ");
			scanf ("%s",user);
			printf("[x] Please, insert your username :)\n");
	}
	
	_kclear();
	aalogo();
	printf("\n\n\n\n");

        shell_mess = strlen (user) + 3;
        argc=1;

	for (;;)
	{
	    printf("%s~# ",user);
	    scanf("%s",cmd);
        
            options (cmd);

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
			/*if (argv[2] != " ")
			{
				memmove(argv[2], argv[2]+6, strlen(argv[2]));
			}*/

			if (!(_kstrncmp(argv[1], "-a", 2)) || !(_kstrncmp(argv[1], "--all", 5)))
       		{
				printf("%s %s.%s.%s%s #1 beta CEST 2008 %s\n",NAME,VERSION,PATCHLEVEL,REV_NUM,EXTRAVERSION,cpu_vendor);
			}

			else if (!(_kstrncmp(argv[1], "-r", 2)) || !(_kstrncmp(argv[1], "--rev", 5)))
       		{
			 	printf("%s.%s.%s%s\n",VERSION,PATCHLEVEL,REV_NUM,EXTRAVERSION);
			}

			else if (!(_kstrncmp(argv[1], "-h", 2) ) || !(_kstrncmp(argv[1], "--help", 6)))
       		{
			 	uname_help();
			}
				
			else if (!(_kstrncmp(argv[1], "-i", 2)) || !(_kstrncmp(argv[1], "--info", 6)))
			{
				info();
			}

			if (!(_kstrncmp(argv[1], '\0', 1)))
			{
				printf("%s\n"
				       "For more info about this tool, please do 'uname --help'\n",NAME);
			}


		}

		else if (!(_kstrncmp(cmd, "cpuid",5)))
		{
			struct cpuinfo_generic *sh_info = get_cpuid();

			printf ("----- CPUID Information -----\n");
			printf ("Vendor: %s\n", sh_info->cpu_vendor);
			printf ("Type: %s\n", sh_info->cpu_type);
		}

		else if (!(_kstrncmp(cmd, "answer",6)))
      		{
          		  printf("42\n");			  
        	}
		else if (!(_kstrncmp(cmd, "sleep", 5)))
		{
			int s;

			if (argv[1]) {
			  s = atoi(argv[1]);
			  if (s != -1)
			    sleep (s);
		 	} else printf ("Missing operand\n");
		}

        else if (!(_kstrncmp(cmd,"kmalloc",7)))
        {
            printf("kmalloc try: ...\n");
            int *b;
            b = (int*) kmalloc(10*sizeof(int));
	    printf("Address obtained: %d\n", b);
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
	  printf("Navigating used list...\n");
          print_heap_list (kheap->used_list);
            free (b);

        }

        else if (!(_kstrncmp(cmd,"do_fault",8)))
    	{  
             printf ("Genero un pagefault scrivendo 10 nella locazione 0xa0000000...\n");
             int *prova;
             prova = (int*)0xa0000000;
             *prova = 10;
              printf ("Contenuto della locazione 0xa0000000 dopo l'intervento dell'handler: %d\n", *prova);
        }

        /*else if (!(_kstrncmp(cmd,"try_heap",8))){
            try_alloc();
        }
        
        else if (!(_kstrncmp(cmd,"free",4))) {
          unsigned int ptr = 425548;
          free((unsigned int*)ptr);
          ptr = 437836;
          free((unsigned int*)ptr);
          ptr=446028;
          free((unsigned int*)ptr);
          printf("Navigating used list...\n");
          print_heap_list (kheap->used_list);
          printf("Navigating free list...\n");
          print_heap_list (kheap->free_list);
          printf("Navigating free nodes...\n");
          print_heap_list (kheap->free_nodes);
        }*/

        else if (!(_kstrncmp(cmd,"printmem",8))) print_heap_list(kheap->used_list);        
        else if (!(_kstrncmp(cmd,"aalogo",6))) 
		aalogo();
        
        else if (!(_kstrncmp(cmd,"credits",7)))
            {
                _kcolor('\011');
                _kputs("DreamOS Credits\n\n");                
                _kputs("Main Developers:\n");
                _kcolor('\012');
                printf("Shainer - Lisa\n"
                    "Finarfin - Ivan\n\n\n"            
                );
                _kcolor('\011');
                _kputs("Contributors:\n");
                _kcolor('\012');
                _kputs("Osiris\n"
                    "Celeron\n"
                    "m0nt0\n"
                    "and many others (3 or 4 :P)\n\n");
                _kcolor('\007');
            }
        else if (strlen(cmd)>0)
	    {
            printf("Unknown command: %s\n", cmd);            
        }

	memset(string+5, 0, strlen(string));
	memset(cmd, 0, strlen(cmd));

        for (--argc; argc>=0; argc--) {
	  free (argv[argc]);
        }

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
	  "\t\t\t\t Welcome to DreamOS\n"
	  "\t\t Where dreams don't become Reality and remain dreams. \n"
	              "\t\t\tR.I.P - Rest in peace with dreamos ^_^        \n",REV_NUM);
	
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
           "clear     - Clear the screen\n"
	   "poweroff  - Turn off the machine\n"
           "kmalloc   - Test a basic kmalloc function\n"
           "do_fault  - Test a page_fault\n"
           "aalogo    - Show an ascii art logo\n"
           "uname     - Print kernel version, try uname --help for more info\n"
           "printmem  - Print used locations of memory\n"
           "credits   - Show DreamOS credits\n"
	   "sleep     - pause DreamOS for a particular number of seconds\n"
	   "cpuid     - Show cpu identification informations\n"
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

