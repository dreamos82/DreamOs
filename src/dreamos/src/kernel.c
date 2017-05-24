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
#include <process.h>
#include <scheduler.h>
#include <vm.h>
#include <descriptor_tables.h>
#include "debug.h"

#ifdef BOCHS_DEBUG

#include <debug.h>

#endif

unsigned int * current_page_table;
extern unsigned int end;
char * module_start;
unsigned int module_end;
elf_t kernel_elf;

asmlinkage void _start(multiboot_info_t * boot_informations)
{
    main_loop(boot_informations);
    while (1);
}

int main_loop(multiboot_info_t * boot_informations)
{
    // Initialize the video.
    video_init();

    // -------------------------------------------------------------------------
    // Show DreamOs version.
    video_set_color(BRIGHT_GREEN);
    video_puts(DREAMOS_VER);
    video_set_color(WHITE);
    video_puts(LNG_SITE);
    video_set_color(BRIGHT_BLUE);
    video_puts(SITEURL);
    video_puts("\n");
    video_set_color(WHITE);
    video_puts("\n");

    // -------------------------------------------------------------------------
    // Initialize the system calls.
    video_puts(LNG_INIT_SYSCALL);
    syscall_init();
    video_print_ok();

    // -------------------------------------------------------------------------
    module_start = (char *) *((unsigned int *) boot_informations->mods_addr);
    module_end = *((unsigned int *) (boot_informations->mods_addr + 4));

    // -------------------------------------------------------------------------
    // Initialize paging.
    video_puts(LNG_INIT_MEMORY);
    kernel_init_paging(boot_informations->mem_upper);
    video_print_ok();

    // -------------------------------------------------------------------------
    // Set the GDT, IDT and IRQ.
    video_puts(LNG_INIT_GDT);
    init_gdt();
    video_print_ok();
    video_puts(LNG_INIT_IDT);
    init_idt();
    video_print_ok();
    video_puts(LNG_INIT_IRQ);
    video_print_ok();
    pic8259_init_irq();

    // -------------------------------------------------------------------------
    // Initialize the virtual memory.
    video_puts(LNG_INIT_VM);
    kernel_init_vm();
    video_print_ok();
    // Initialize the heap.
    video_puts(LNG_INIT_HEAP);
    kernel_init_heap();
    video_print_ok();
    // Initialize the memory.
    kernel_map_memory(boot_informations);
    printf(" * Memory (lower) : %lu kb \n", boot_informations->mem_lower);
    printf(" * Memory (upper) : %lu Mb \n", boot_informations->mem_upper / 1024);

    // -------------------------------------------------------------------------
    // Alloc and fill CPUID structure.
    video_puts(LNG_INIT_CPUID);
    get_cpuid(&sinfo);
    video_print_ok();

    // -------------------------------------------------------------------------
    // Initialize the filesystem.
    video_puts(LNG_INIT_FS);
    vfs_init();
    video_print_ok();
    initfs_init();
    if (boot_informations->mods_count > 0)
    {
        printf("Found n. %lu Modules\n", boot_informations->mods_count);
    }
    // Get the kernel image from the boot info
    kernel_elf = elf_from_multiboot(boot_informations);

    // -------------------------------------------------------------------------
    // Initialize the scheduler.
    video_puts(LNG_INIT_SCHEDULER);
    kernel_initialize_scheduler();
    video_print_ok();
    // We disable floppy driver motor
    fdc_disable_motor();

    // -------------------------------------------------------------------------
    // Initialize the shell.
    video_puts(LNG_INIT_SHELL);
    video_print_ok();
    kernel_create_process(shell, "Shell", "Shell", 0);
    return 0;
}
