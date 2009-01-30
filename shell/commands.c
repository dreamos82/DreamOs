#include <commands.h>

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
	 "\t\t           v%s.%s%s      \n\n"
	 "\t\t\t\t Welcome to DreamOS\n"
	 "\t\t Where dreams don't become Reality and remain dreams. \n"
	 "\t\t\tR.I.P - Rest in peace with dreamos ^_^        \n",VERSION, PATCHLEVEL, EXTRAVERSION);
	
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
	 "echo      - Print some lines of text\n");
}

void echo()
{
  int c;

    for (c=1; c<argc; c++)
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
  int *b;
  int i=0;

  printf("Kmalloc try: ...\n");
  b = (int *)kmalloc(10 * sizeof(int));
  printf("Address obtained: %d\n", b);
  
  while(i<10) {
    b[i] = i*2;
    i++;
  }
  i=0;

  while(i<10) {
    printf("b[%d] = %d\n",i, b[i]);
    i++;
  }

  printf("Address of b: %d\n", b);
  printf("Navigating used list...\n");
  print_heap_list (kheap->used_list);
  free (b);
}

void do_fault()
{
  int *prova;

  printf ("Genero un pagefault scrivendo 10 nella locazione 0xa0000000...\n");
  prova = (int *)0xa0000000;
  *prova = 10;

  printf ("Contenuto della locazione 0xa0000000 dopo l'intervento dell'handler: %d\n", *prova);
}

void uname()
{
  if (!(strcmp(argv[1], "-a")) || !(strcmp(argv[1], "--all")))
    printf("%s %s.%s%s #1 CEST 2009 %s\n", NAME, VERSION, PATCHLEVEL, EXTRAVERSION, cpu_vendor);
  else if (!(strcmp(argv[1], "-r")) || !(strcmp(argv[1], "--rev")))
    printf("%s.%s%s\n", VERSION, PATCHLEVEL, EXTRAVERSION);
  else if (!(strcmp(argv[1], "-h") ) || !(strcmp(argv[1], "--help")))
    uname_help();
  else if (!(strcmp(argv[1], "-i")) || !(strcmp(argv[1], "--info")))
    uname_info();
  else
    printf("%s. For more info about this tool, please do 'uname --help'\n",NAME);
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
          "Finarfin - Ivan\n\n\n"            
         );

  _kcolor('\011');
  _kputs("Contributors:\n");
  _kcolor('\012');
  _kputs("Osiris\n"
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

void cpuid(void)
{
  struct cpuinfo_generic *sinfo = get_cpuid();

  printf ("----- CPUID Information -----\n");
  printf ("Vendor: %s\n", sinfo->cpu_vendor);
  printf ("Type: %s\n", sinfo->cpu_type);
  free (sinfo);
}

void answer(void)
{
  printf("42\n");
}

