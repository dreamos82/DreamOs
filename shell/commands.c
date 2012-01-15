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
#include <vfs.h>
#include <testing.h>
#include <fcntl.h>
#include <stat.h>
#include <task.h>

int argc;
char **argv;

void aalogo()
{
  _kcolor (BRIGHT_GREEN);
  printf("\t\t ____           _____          _____ _____\n");
  printf("\t\t|    \\ ___ ___ |  _  | _______|     |   __|\n");
  printf("\t\t|  |  |  _| -_|| |_| ||       |  |  |__   |\n");
  printf("\t\t|____/|_| |___||_| |_||_|\134_/|_|_____|_____|\n");
  _kcolor (WHITE);
  printf("\t\tVersion: \"%s %s.%s%s\"\n", NAME, VERSION, PATCHLEVEL, EXTRAVERSION);
  //_kcolor(WHITE);
  logo();
}

void logo()
{
  _kcolor (BRIGHT_BLUE);
  printf("\n");
  printf("\t\t\t The Dream Operating System \n"
	 "\t\t           v%s.%s%s %s      \n\n"
	 "\t\t\t   Welcome to DreamOS\n"
	 "\t  Where dreams don't become Reality and remain dreams. \n"
	 "\t          R.I.P - Rest in peace with dreamos ^_^        \n", 
	 VERSION, PATCHLEVEL, EXTRAVERSION, REV_NUM);
	
  printf("\n\n\n\n");
  _kcolor(WHITE);
}

void help()
{
  printf ("Available commands:\n");
  int i=0;
  while(i<19)
  {
	printf("%s   %s\n", shell_cmd[i].cmdname, shell_cmd[i].cmddesc);
	i++;
  }
}

/*void help()
{
  printf ("Available commands:\n");
  printf(
	"help          - See the 'help' list to learn the DreamOS commands now available\n"
	"clear         - Clear the screen\n"
	"poweroff      - Turn off the machine\n"
	"aalogo        - Show an ascii art logo\n"
	"uname         - Print kernel version, try uname --help for more info\n"
	"credits       - Show DreamOS credits\n"
	"sleep         - pause DreamOS for a particular number of seconds\n"
	"cpuid         - Show cpu identification informations\n"
	"date          - Show date and time\n"
	"echo          - Print some lines of text\n"
	"drv_load      - Tool to load and kill drivers\n"
	"ls            - Tool for listing dir - not complete-\n"
	"cd            - Change dir - not complete-\n"
	"whoami        - Show the current user name\n"
	"tester        - Try some functions, 'tester --help' for more info'\n"
	"pwd           - Print current working directory\n"
	"more		   - read content of a file\n"
	"newfile	   - create a newfile\n"
	);
}*/


void echo()
{
	int i = argc;
	int j = 0;
    if ( argc == 1 ) {
         printf("");
    } else {
		while ( --i > 0 ) {
		printf("%s ",argv[++j]);
		}
	}
    printf("\n");
}

void poweroff()
{
  printf ("Poweroff...\n");
  
  asm("cli");
  printf("Now you can shut down the PC.\n");
  while(1);
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
  printf("1) -a   - Kernel version and processor type\n"
  	   "2) -r   - Only the kernel version\n"
	   "3) -i   - All info of system and kernel\n");	
}

void uname_info()
{
  printf("\n:==========: :System info: :==========:\n\n");
	
  // Kernel info
  printf( "Version: %s\n"
	  "Patchlevel: %s\n"
	  "Extraversion: %s\n"
	  "Name: %s\n"
	  "Revision: %s\n",VERSION,PATCHLEVEL,EXTRAVERSION,NAME,REV_NUM);

  // CPU Info
  _kputs (LNG_CPU);
  _kcolor (BRIGHT_RED);
  _kgoto (61, _kgetline());
  _kputs (cpu_vendor);
  _kcolor(WHITE);
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

void credits(void)
{
  _kcolor(BRIGHT_BLUE);
  _kputs("DreamOS Credits\n\n");                
  _kputs("Main Developers:\n");
  _kcolor(GREEN);
   printf(  "Finarfin - Ivan (Founder)\n\n");
  _kcolor(BRIGHT_BLUE);
  _kputs("Developers:\n");
  _kcolor(GREEN);
  _kputs("Lord Osiris - Diego Stamigni \n\n");
  _kcolor(BRIGHT_BLUE);
  _kputs("Contributors:\n");
  _kcolor(GREEN);
  _kputs("N3m3s1s\n"
			"Blackz\n"
		    "vinc94\n"
	  		"tk0\n"
	  		"DT\n"
         	"Celeron\n"
	 		"Hamcha\n"
         	"m0nt0\n"			
         	"and many others (3 or 4 :P)\n\n");
  _kcolor(WHITE);
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

  printf ("----- CPU ID Information -----\n");
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
    printf ("---------------------------\n");
  }
}

void answer(void)
{
  printf("42\n");
}

void  drv_load(void)
{
  if (argc < 2)
      printf("No driver inserted or bad usage! Type %s --help for the usage.\n", argv[0]);
  
  else
  { 
      if ( (_kstrncmp (argv[1], "-r", 2) == 0) ) 
	  { 
		  if ( ( argv[2] != NULL ) ) 
		  {
			  if (_kstrncmp (argv[2], "mouse", 5) == 0)  { 
		 	 	 printf("Disattivamento %s in corso..\n", argv[2]);
		 	  	 mouse_dead();
			  }
      		  else 
				  printf("FATAL: Driver %s not found.\n", argv[2]); 
		  }
		  else 
			  printf("Warning, no driver name inserted!\n"); 
	  }
      // per ora lo impostiamo così, static,
      // tanto non abbiamo nessun altro modulo, per ora..
      else if (_kstrncmp (argv[1], "mouse", 5) == 0)
      {
	 	 // enabling mouse
	  	 mouse_init();
      }

      else if ( (_kstrncmp (argv[1], "--help", 6) == 0) || (_kstrncmp (argv[1], "-h", 2) == 0) )
      {
	  printf("---------------------------------------------------\n"
		 "Driver tool to load and kill driver\n"
		 "Simple to use, just type:\n"
		 "\n"
		 "Usage: %s -<options> driver_name\n"
		 "\t-> %s module_name     - to load driver\n"
		 "\t-> %s -r module_name  - to kill driver\n"
		  "---------------------------------------------------\n"
		  , argv[0], argv[0], argv[0]);
      }

     else
	{
	 	if ( (_kstrncmp (argv[1], "-r", 2) == 0) && (_kstrncmp (argv[2], "mouse", 5) == -1) )
			printf("FATAL: Driver %s not found.\n", argv[2]); 
	
		else
			printf("FATAL: Driver %s not found.\n", argv[1]);
	 }

   }
	
}

void ls() {
	int i=0, j=0;
	int flag = 0;
	DIR *dirp;
	while ( strcmp(mountpoint_list[i].mountpoint, "") ) {
         	j++;
		i++;
	}
	i = 0;		
	dirp = opendir(current_user.cur_path);
	if(dirp!=NULL){
		int tot = 0;
		if(argc > 1 && strcmp(argv[1], "-l") == 0) flag =1;
		/*while(i<j){
			if(strcmp(mountpoint_list[i].mountpoint, current_user.cur_path)){
				
			}
			i++;
		}*/
		struct dirent* cur_dir_entry;
		cur_dir_entry = readdir(dirp);		
		while(cur_dir_entry!=NULL){			
			//struct stat 			
			if(cur_dir_entry->d_type == FS_MOUNTPOINT) _kcolor(BRIGHT_GREEN);
			if(flag==1){
				struct stat tmp_stat;
				if(stat(cur_dir_entry->d_name, &tmp_stat)!=-1){
					printf("uid=%d(%s) - size:  %d ", tmp_stat.st_uid, current_user.username, tmp_stat.st_size);
					tot = tot + tmp_stat.st_size;
				}
			}
			printf("%s\n", cur_dir_entry->d_name);
			_kcolor(WHITE);			
			cur_dir_entry = readdir(dirp);
		}
		closedir(dirp);
		printf("\n");
		if(flag==1)printf("Total: %d byte\n", tot);
	} else {
		while( i < j ) {
			if (argc == 1) {
				_kcolor(BRIGHT_BLUE);
				printf("%s   ", mountpoint_list[i].mountpoint);
			}
			else {
				if  ( (_kstrncmp(argv[1], "-l", 2) ) == 0 )
					printf("uid=%d(%s), gid=%d(%s) - ", 
							mountpoint_list[i].uid,
							current_user.username,
							mountpoint_list[i].gid, 
							current_user.username);
					_kcolor(BRIGHT_BLUE);
					printf("%s\n", mountpoint_list[i].mountpoint);
					_kcolor(WHITE);
			}
			i++; 
		}
		_kcolor(WHITE);
		printf(" -->   Total: %d\n", j);
	}
}

void more(){
	if(argc==1) 
		printf("Usage:\n\t more filename\nfor read a file\n");
	else {
		if(argc<3) {
			int i;
			char buf;
			//printf("File to open: %s\n", argv[1]);
			i = open(argv[1], O_RDONLY, 42);
			if(i>-1) {
				int j=0;		
				while(read(i, &buf, 1)!= (int) NULL) {
					putchar(buf);			
					j++;
				}	
				putchar('\n');			
				close(i);
			}		
		}
		else printf("too many arguments\n");
	}
}

void cd( ){
	//char *relpath;		
	char abspath[CURPATH_LEN];
	DIR *dirp=NULL;	
	if(argc != 2) {
		printf("Bad usage. Try 'ls -l' and then 'cd dir'.\n");
		return;
	} else {
		int i=0;
		int rel_size __attribute__((unused))= 0;		
		memset(abspath, '\0', CURPATH_LEN);				
		if(argv[1][0] == '/') {
			i = get_mountpoint_id(argv[1]);		
			strcpy(abspath, argv[1]);
			//printf("abspath: %s\n", abspath);
			dirp=opendir(argv[1]);
		}
		else if(!strncmp(argv[1], "..", 2)) {
			printf(".. option %s\n", argv[1]);
			return;
		}
		else if(argv[1][0]=='.') {
			//printf(". option\n");			
			if(strlen(argv[1]) == 1) return;
			else printf("str_len: %d\n", strlen(argv[1]));
		}		
		else {			
			int abs_size = 0;			
			abs_size = strlen(current_user.cur_path);
			strcpy(abspath, current_user.cur_path);
			if(abspath[abs_size-1] == '/')
				strncat(abspath, argv[1], strlen(argv[1]));
			else {
				strncat(abspath, "/", strlen(argv[1]));
				strncat(abspath, argv[1], strlen(argv[1]));
			}	
			dirp=opendir(abspath);			
		}		
		if(dirp!=NULL){			
			closedir(dirp);
		}		
		rel_size = strlen(argv[1]) - strlen(mountpoint_list[i].mountpoint);
		if(i == -1) {			
			printf("cd: %s: No such file or directory\n", argv[1]);
			return;
		}
		else {
			strcpy(current_user.cur_path, abspath);
		}
		/*if(rel_size >0){
			relpath = get_rel_path(i, argv[1]);
			free(relpath);
		}*/
	}
}

void whoami(){
	printf("%s\n", current_user.username);
}

void tester(){
	int i = 0;
	struct devel testing[MAX_TEST] = { 
					{ "try_kmalloc", try_kmalloc },
					{ "try_strtok", try_strtok },
					{ "do_fault", do_fault },
					{ "try_printmem", try_printmem },
					{ "try_ocreat", try_ocreat },
					{ "try_open", try_open},					
					{ "try_syscall", try_syscall},
					{ "show_fd", show_fd},
					{ "test_stat", test_stat},
					{ "try_check", try_check},
					{ "try_shadow", try_shadow},
					{ "try_mapaddress", try_mapaddress},
					{ "try_tasksetup", try_tasksetup},
					{ "try_tasklist", test_tasklist},
					{ "--help", help_tester },
					};
	if (argc != 2) {
		printf ("Bad usage. Try '%s --help' for more info about the usage.\n", argv[0]);
		return;
	} else { 
		for ( i = 0 ; i <= MAX_TEST ; i++) {
			if ( (strcmp(argv[1], testing[i].cmd_testname) ) == (int) NULL ) {
				 (testing[i].func)();
				break;
			} 
		}
		if ( i > MAX_TEST ) {
			printf("Error: %s not found.\n", argv[1]);
			//return;
		}
	}
}

void pwd(){
	printf("%s\n", current_user.cur_path );
}

void newfile(){
	if(argc!=2){
		printf("Bad usage. Try newfile --help for more informations\n");
	}
	else {
		if(strcmp(argv[1], "--help")==0) {
			printf("newfile FILENAME - Make a new file, and prompt for it's content\n");
		}
		else {
			int fd;			
			fd = open(argv[1], O_RDONLY);
			if(fd>=0) printf("-ERROR: %s File already exist\n", argv[1]);
			else {
				char text[256];
				printf("Filename: %s\n", argv[1]);
				close(fd);
				fd = open(argv[1], O_RDWR|O_CREAT|O_APPEND);		
				if(fd>=0){ 
					printf("Type your text here, actually only one line available!!\n");
					scanf("%s", text);
					write(fd,text, strlen(text));
					if(close(fd)==-1) printf("something went wrong\n");
				}
			}
		}
	}
}

void ps(){
	//printf("Stay tuned\n");
	test_tasklist();
}

