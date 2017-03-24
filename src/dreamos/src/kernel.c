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
#include <kernel.h>
#include <video.h>
#include <pic8259.h>
#include <gdt.h>
#include <idt.h>
#include <cpuid.h>
#include <stdio.h>
#include <port_io.h>
#include <paging.h>
#include <language.h>
#include <shell.h>
#include <syscall.h>
#include <kheap.h>
#include <vfs.h>
#include <initrd.h>
#include <fdc.h>
#include <thread.h>
#include <scheduler.h>
#include <descriptor_tables.h>
#include <fismem.h>


#ifdef BOCHS_DEBUG

#include <debug.h>

#endif

unsigned int * current_page_table;
extern unsigned int end;
multiboot_info_t * boot_informations;
char * module_start;
unsigned int module_end;
elf_t kernel_elf;

asmlinkage void _start(struct multiboot_info * boot_info)
{
    boot_informations = boot_info;
    main_loop(boot_info);
    while (1);
}

int main_loop(struct multiboot_info * boot_info)
{
    // -------------------------------------------------------------------------
    // Initialize the system calls.
    _kclear();
    syscall_init();
    module_start = (char *) *((unsigned int *) boot_info->mods_addr);
    module_end = *((unsigned int *) (boot_info->mods_addr + 4));

    // -------------------------------------------------------------------------
    // Show DreamOs version.
    _kcolor(BRIGHT_GREEN);
    _kputs(DREAMOS_VER);
    _kcolor(WHITE);
    _kputs(LNG_SITE);
    _kcolor(BRIGHT_BLUE);
    _kputs(SITEURL);
    _kputs("\n");
    _kcolor(WHITE);
    _kputs("\n");

    // -------------------------------------------------------------------------
    // Set the GDT.
    _kputs(LNG_GDT);
    init_gdt();
    _kprintOK();

    outportb(0xFF, MASTER_PORT_1);
    outportb(0xFF, SLAVE_PORT_1);

    // -------------------------------------------------------------------------
    __asm__ __volatile__("cli;");
    // Set the IDT.
    _kputs(LNG_IDT);
    init_idt();
    _kprintOK();
    // Initialize the IRQ.
    printf(LNG_PIC8259"\n");
    irq_init();
    _kprintOK();
    // Initialize the memory.
    printf(LNG_INIT_MEMORY);
    set_memorysize((boot_info->mem_upper+boot_info->mem_lower)*1024);
    init_mem();
    _kprintOK();
    __asm__ __volatile__("sti;");
    printf("Initialize paging...");
    init_paging();
    _kprintOK();
    printf(" * Memory (upper) : %lu Mb \n", boot_info->mem_upper / 1024);
    printf(" * Memory (lower) : %lu kb \n", boot_info->mem_lower);

    // -------------------------------------------------------------------------
    // Alloc and fill CPUID structure.
    printf(LNG_INIT_CPUID);
    sinfo = kmalloc(sizeof(struct cpuinfo_generic));
    get_cpuid(sinfo);
    _kprintOK();

    // -------------------------------------------------------------------------
    // Initialize the filesystem.
    printf(LNG_INIT_FS);
    vfs_init();
    _kprintOK();
    initfs_init();

    // -------------------------------------------------------------------------
    if (boot_info->mods_count > 0)
    {
        printf("Found n. %lu Modules\n", boot_info->mods_count);
    }
    // Get the kernel image from the boot info
    kernel_elf = elf_from_multiboot(boot_info);

    __asm__ __volatile__("sti;");

    // -------------------------------------------------------------------------
    // Initialize the scheduler.
    printf(LNG_SCHEDULER);
    kernel_initialize_scheduler();
    _kprintOK();

    //printf(LNG_PIT8253);
    //printf("----\n");
    //
    //printf("[+] Address: 0x%x\n", &end);
    //printf("\n");

#ifdef BOCHS_DEBUG
    dbg_print("DreamOS Debug String for Bochs\n");
#endif

    // We disable floppy driver motor
    fdc_disable_motor();

    // -------------------------------------------------------------------------
    // Initialize the shell.
    printf(LNG_SHELL);
    _kprintOK();
    kernel_create_thread(shell, "Shell", "Shell", 0);
    return 0;
}

