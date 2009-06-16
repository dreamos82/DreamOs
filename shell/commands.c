#include <commands.h>
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
#include <version.h>
#include <cpuid.h>
#include <clock.h>
#include <sys/utsname.h>
#include <mouse.h>

void aalogo()
{
  _kcolor (4);
  printf("\t\t ____           _____          _____ _____\n");
  printf("\t\t|    \\ ___ ___ |  _  | _______|     |   __|\n");
  printf("\t\t|  |  |  _| -_|| |_| ||       |  |  |__   |\n");
  printf("\t\t|____/|_| |___||_| |_||_|\134_/|_|_____|_____|\n");
  _kcolor (6);
  printf("\t\tVersion: \"%s %s.%s%s\"\n", NAME, VERSION, PATCHLEVEL, EXTRAVERSION);
  _kcolor(7);
  logo();
}

void logo()
{
  _kcolor (5);
  printf("\n");
  printf("\t\t\t The Dream Operating System \n"
	 "\t\t           v%s.%s%s %s      \n\n"
	 "\t\t\t   Welcome to DreamOS\n"
	 "\t  Where dreams don't become Reality and remain dreams. \n"
	 "\t          R.I.P - Rest in peace with dreamos ^_^        \n", 
	 VERSION, PATCHLEVEL, EXTRAVERSION, REV_NUM);
	
  printf("\n\n\n\n");
  _kcolor(7);
}

void help()
{
  printf ("Available commands:\n");
  printf("help      - See the 'help' list to learn the DreamOS commands now available\n"
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
	 "date      - Show date and time\n"
	 "echo      - Print some lines of text\n"
	 "modprobe  - Tool to load and kill modules\n");
}

void echo()
{
    int c;
    for (c = 1; c < argc; c++)
        printf ("%s ", argv[c]);
    printf ("\n");
}

void poweroff()
{
  printf ("Poweroff...\n");
  
  asm("sti");
  printf("E' ora possibile spegnere il computer.\n");
  while(1);
}

void kmalloc_try()
{
  int *b, *c, *d;
  int i = 0;

  printf("Kmalloc try: ... ");
  //print_heap_list (kheap->free_list);
  b = (int *)kmalloc(15 * sizeof(int));
  c = (int *)kmalloc(10 * sizeof(int));
  d = (int *)kmalloc(15 * sizeof(int));
  printf("Address obtained: %d %d %d\n", b, c, d);
  
  while(i < 15) {
    b[i] = i*2;
    if(i < 10) c[i] = i*3;
    d[i] = i*2;
    i++;
  }
  i = 0;

  while(i < 15) {
    printf("b[%d] = %d d[%d] = %d ",i, b[i], i,d[i]);
    if(i < 10) printf("c[%d] = %d\n", i, c[i]);
    else printf("\n");
    i++;
  }
  printf("Navigating used list...\n");
  print_heap_list (kheap->used_list);
  free (b);
  free (c);
  free (d);
}

void do_fault()
{
  int *prova;

  printf ("Genero un pagefault scrivendo 10 nella locazione 0xa0000000...\n");
  prova = (int *)0xa0000000;
  *prova = 10;

  printf ("Contenuto della locazione 0xa0000000 dopo l'intervento dell'handler: %d\n", *prova);
}

void uname_cmd()
{
  struct utsname *infos;
  infos = kmalloc(sizeof(struct utsname *));
  uname(infos);
  if (!(strcmp(argv[1], "-a")) || !(strcmp(argv[1], "--all")))
    printf("%s %s.%s%s Rev: %s #1 CEST 2009 %s\n", infos->sysname, infos->version, infos->release, EXTRAVERSION,REV_NUM, cpu_vendor);
  else if (!(strcmp(argv[1], "-r")) || !(strcmp(argv[1], "--rev")))
    printf("%s.%s%s\n", infos->version, infos->release, EXTRAVERSION);
  else if (!(strcmp(argv[1], "-h") ) || !(strcmp(argv[1], "--help")))
    uname_help();
  else if (!(strcmp(argv[1], "-i")) || !(strcmp(argv[1], "--info")))
    uname_info();
  else
    printf("%s. For more info about this tool, please do 'uname --help'\n",infos->sysname);
  free(infos);
}

void uname_help() 
{
  printf("Uname function allow you to see the kernel and system information.\n");
  printf("Function avaibles:\n");
  printf("1) '-a'   - Kernel version and processor type\n"
  	 "2) '-r'   - Only the kernel version\n"
  	 "3) '-i'   - All info of system and kernel\n");	
}

void uname_info()
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

void printmem(void)
{
  print_heap_list(kheap->used_list);
}

void credits(void)
{
  _kcolor('\011');
  _kputs("DreamOS Credits\n\n");                
  _kputs("Main Developers:\n");
  _kcolor('\012');
   printf("Shainer - Lisa\n"
	  "Osiris\n"
          "Finarfin - Ivan\n\n\n"            
         );

  _kcolor('\011');
  _kputs("Contributors:\n");
  _kcolor('\012');
  _kputs("vinc94\n"
	  "tk0\n"
	  "DT\n"
         "Celeron\n"
	 "Hamcha\n"
         "m0nt0\n"
         "and many others (3 or 4 :P)\n\n");
  _kcolor('\007');
}

void sleep_cmd(void)
{
  int s;

  if (argv[1]) {
    s = atoi(argv[1]);
    if (s != -1)
      sleep (s);
  } else printf ("Missing operand\n");
}

void cpuid_help()
{
  printf ("CPUID help message\n"
          "-v : shows verbose CPUID information\n");
}

/*
 * Call CPUID command and display information
 */
void cpuid(void)
{

  /* List of features */
  const char *ecx_features[ECX_FLAGS_SIZE] = {
      "SSE3", "Reserved", "Reserved", "Monitor/MWAIT", "CPL Debug Store", 
      "Virtual Machine", "Safer Mode", "Enhanced Intel SpeedStep Technology", 
      "Thermal Monitor 2", "SSSE3", "L1 Context ID", "Reserved", "Reserved", 
      "CMPXCHG16B", "xTPR Update Control", "Perfmon and Debug Capability", 
      "Reserved", "Reserved", "DCA", "SSE4.1", "SSE4.2", "Reserved", "Reserved", "POPCNT" 
  };
  const char *edx_features[EDX_FLAGS_SIZE] = { 
      "x87 FPU", "Virtual 8086 Mode", "Debugging Extensions", "Page Size Extensions",
      "Time Stamp Counter", "RDMSR and WRMSR", "Physical Address Extensions", 
      "Machine Check Exception", "CMPXCHG8B", "APIC On-chip", "Reserved", 
      "SYSENTER and SYSEXIT", "Memory Type Range Registers", "PTE Global Bit", 
      "Machine Check Architecture", "Conditional Move Instructions", 
      "Page Attribute Table", "36-bit Page Size", "Processor Serial Number", 
      "Reserved", "Debug Store", "Thermal Monitor and Clock Facilities", "Intel MMX",
      "FXSAVE and FXRSTOR", "SSE", "SSE2", "Self Snoop", "Multi-Threading", "TTC", 
      "Reserved", "Pending Break Enable" 
  };
  
  int i;
  int verbose = 0;

  /* Examine possible options */
  if (argv[1] != NULL) {
    if (strcmp (argv[1], "-v") == 0)
      verbose=1;
    else {
      printf ("Unknown option %s\n", argv[1]);
      cpuid_help();
      return;
    }
  }

  printf ("----- CPUID Information -----\n");
  if (strcmp(sinfo->brand_string, "Reserved") != 0)
    printf ("%s\n", sinfo->brand_string);
  printf ("Vendor: %s\n", sinfo->cpu_vendor);
  printf ("Type: %s, Family: %x, Model: %x\n", sinfo->cpu_type, sinfo->cpu_family, sinfo->cpu_model);
  printf ("Apic ID: %d\n", sinfo->apic_id);

  if (verbose==1) {
    printf ("\n--- Supported features ---\n");
    for (i=0; i<ECX_FLAGS_SIZE; i++) {
      if (sinfo->cpuid_ecx_flags[i] == 1)
        printf ("%s\n", ecx_features[i]);
    }
    for (i=0; i<EDX_FLAGS_SIZE; i++) {
      if (sinfo->cpuid_edx_flags[i] == 1)
        printf ("%s\n", edx_features[i]);
    }
    printf ("--------------------------\n");
  }
}

void answer(void)
{
  printf("42\n");
}

void modprobe(void)
{
  if (argc < 2)
      printf("No module inserted or bad usage! Type modprobe --help for the usage.\n");
  
  else
  {
      if ( argv[2] != NULL && (_kstrncmp (argv[1], "-r", 2) == 0) && 
	  (_kstrncmp (argv[2], "mouse", 5) == 0) )
      {
	  printf("Removing %s..\n", argv[2]);
	  mouse_dead();
      }
      /* per ora lo impostiamo così, static,
      tanto non abbiamo nessun altro modulo, per ora.. */
      else if (_kstrncmp (argv[1], "mouse", 5) == 0)
      {
	 /* enabling mouse */
	 mouse_init();
       }

      else if (_kstrncmp (argv[1], "--help", 6) == 0)
      {
	  printf("---------------------------------------------------\n"
		 "Module tool to load and kill modules\n"
		 "Simple to use, just type:\n"
		 "\n"
		 "Usage: %s -<options> module_name\n"
		 "\t-> %s module_name     - to load module\n"
		 "\t-> %s -r module_name  - to kill module\n"
		  "---------------------------------------------------\n"
		  , argv[0], argv[0], argv[0]);
      }

      else
	 if ( (_kstrncmp (argv[1], "-r", 2) == 0) && (_kstrncmp (argv[2], "mouse", 5) == -1) )
	 {   printf("FATAL: Module %s not found.\n", argv[2]); }
	
	 else
	     printf("FATAL: Module %s not found.\n", argv[1]);

   }


}



