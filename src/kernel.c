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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <elf.h>
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
#include <io.h>
#include <keyboard.h>
#include <paging.h>
#include <use.h>
#include <shell.h>
#include <syscall.h>
#include <kheap.h>
#include <vfs.h>
#include <initrd.h>
#include <keyboard.h>
#include <fdc.h>
#include <thread.h>
#include <scheduler.h>
#include <testing.h>
#include <paging.h>
#include <vm.h>

#ifdef BOCHS_DEBUG
#include <debug.h>
#endif

unsigned int *current_page_table;
extern unsigned int end;
multiboot_info_t *boot_informations;
char *module_start;
unsigned int module_end;
elf_t kernel_elf;

asmlinkage void _start(struct multiboot_info *boot_info)
{
    boot_informations = boot_info;
	main_loop(boot_info);
	while(1);
}

int main_loop(struct multiboot_info *boot_info)
{
    _kclear();
    syscall_init();
    module_start = (char*) *((unsigned int*)boot_info->mods_addr);
	module_end = *((unsigned int*)(boot_info->mods_addr+4));
    _kcolor(BRIGHT_GREEN);
    _kputs(DREAMOS_VER);
    _kcolor(WHITE);
    _kputs(LNG_SITE);
    _kcolor(BRIGHT_BLUE);
    _kputs(SITEURL);
    _kputs("\n");
    _kcolor(WHITE);
    _kputs("\n");
    _kputs(LNG_GDT);
    init_gdt();
    _kprintOK();
    outportb(0xFF, MASTER_PORT_1);
    outportb(0xFF, SLAVE_PORT_1);
    _kputs(LNG_IDT);
    init_idt();
	asm volatile("int $0x3");
    _kprintOK();
	kernel_init_paging(boot_info->mem_upper);
	_kprintOK();
    _kputs(LNG_PIC8259);
    init_IRQ();
    _kprintOK();
	kernel_init_vm();
	kernel_init_heap();
	kernel_map_memory(boot_info);
    _kprintOK();
    printf("Memory (upper) amount-> %d Mb \n", boot_info->mem_upper/1024);
    printf("Memory (lower) amount-> %d kb \n", boot_info->mem_lower);

    /** Alloc and fill CPUID structure */
    sinfo = kmalloc(sizeof(struct cpuinfo_generic));
    get_cpuid (sinfo);
    vfs_init();
    initfs_init();
	if(boot_info->mods_count > 0)
		printf("Found n. %d Modules\n", boot_info->mods_count);
    printf("\n");

	// Get the kernel image from the boot info
	kernel_elf = elf_from_multiboot(boot_info);

	asm volatile ("sti");
	thread_t* scheduler_thread = kernel_init_threading();
    kernel_init_scheduler(scheduler_thread);
    _kprintOK();
    printf(LNG_PIT8253);
    printf("----\n");
    printf(LNG_SHELL);
    _kprintOK();
	printf("[+] Address: 0x%x\n", &end);
	printf("\n");

#ifdef BOCHS_DEBUG
	dbg_bochs_print("DreamOS Debug String for Bochs\n");
#endif
	configure_PIT ();
    //We disable floppy driver motor
    fdc_disable_motor();

	kernel_create_thread(shell, "shell", 0);
    return 0;
}

