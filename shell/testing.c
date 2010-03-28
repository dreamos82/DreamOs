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
#include <multiboot.h>
#include <kernel.h>
#include <fcntl.h>
#include <initrd.h>

//multiboot_info_t *boot_informations;
char *module_start;

void try_strtok()
{
	char *s = "Hello World";
	char *p;
	
	printf("Stringa completa: %s\n"
		 "Stringa spezzata: \n" ,s);
	
	p = strtok(s, " ");
	while (p != NULL) {
		printf("%s\n", p);
		p = strtok(NULL, " ");
	}
}

void try_kmalloc()
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

void try_printmem(void)
{
	print_heap_list(kheap->used_list);
}

void help_tester()
{
	printf("Testing functions.. ");
	_kcolor(4);
	printf("Warning: for developers only!\n");
	_kcolor(7);
	printf(
		"  -> try_kmalloc       - Test a basic kmalloc() function\n"
		"  -> do_fault          - Test a page_fault\n"
		"  -> try_strtok        - Test strtok() function in string.h\n"
		"  -> try_printmem      - Print used locations of memory\n"
		"  -> try_module        - Read the content of the memory zone loaded in module, aka initfs \n"
		"  -> try_open          - Function to test open() & stdarg() \n"		
		);	
}

void try_module(){
	initrd_t *fs_head;
	initrd_file_t* headers;
	int i;
	i=0;
	fs_head = (initrd_t *)module_start;
	headers = (initrd_file_t *) (module_start + sizeof(initrd_t));
	while(i<fs_head->nfiles){
		printf("%s\t%d\n", headers[i].fileName, headers[i].length);
		i++;
	}
	printf("Total Files: %d\n", fs_head->nfiles);
	_kputs("\n");
}

void try_open(){
	char appoggio[50];
	char prova;
	int i;
	i=0;	
	while(i<50){
		appoggio[i] = '\0';
		i++;
	}
	printf("Please insert a path: ");
	
	scanf("%s", appoggio);	
	i = open(appoggio, O_RDONLY, 42);
	printf("%d\n", i);
	if(i>-1) {
		int j=0;		
		while(read(i, &prova, 1)!=0) {
			putchar(prova);			
			j++;
		}
		//printf("\n%s\n", prova);
		close(i);
	}
	printf("\n");
}

void try_fsinit(){
	printf("Number of files present: %d\n", initfs_init());
}

void try_syscall(){
	printf("Trying sysputch:\n");
	int i;
	//i='A';
    for(i='A';i<='Z';i++) {
		asm(
			"movl %0, %%ecx\n"
			"movl $0x0, %%eax\n"
			"int $80\n"
			: : "g"(i)	
		);	
	}
	printf("\n");
}
