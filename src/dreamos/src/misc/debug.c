/***************************************************************************
 *            debug.c
 *
 *  Sat Mar 31 07:47:17 2007
 *  Copyright  2007  Theking0 - finarfin
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


#include <debug.h>
#include <stdio.h>

#define SERIAL_BOCHS    (0x00E9)
#define SERIAL_COM1     (0x03F8)

void dbg_print(const char * msg, ...)
{
    // Define a buffer for the formatted string.
    char formatted[1024];
    // --------------------------------
    // Stage 1: FORMAT
    // Start variabile argument's list.
    va_list ap;
    va_start (ap, msg);
    // Format the message.
    vsprintf(formatted, msg, ap);
    // End the list of arguments.
    va_end (ap);
    // --------------------------------
    // Stage 2: SEND
    register int it;
    for (it = 0; formatted[it] != 0; ++it)
    {
#if     defined(BOCHS_DEBUG)
        outportb(SERIAL_BOCHS, formatted[it]);
#elif   defined(QEMU_DEBUG)
        outportb(SERIAL_COM1, formatted[it]);
#endif
    }
    // --------------------------------
}


void print_reg(register_t * reg)
{
    dbg_print("Registers dump:\n");
    dbg_print("eax 0x%-9x ebx 0x%-9x ecx 0x%-9x edx    0x%-9x\n",
              reg->eax, reg->ebx, reg->ecx, reg->edx);
    dbg_print("edi 0x%-9x esi 0x%-9x ebp 0x%-9x esp    0x%-9x\n",
              reg->edi, reg->esi, reg->ebp, reg->esp);
    dbg_print("eip 0x%-9x cs  0x%-9x ss  0x%-9x eflags 0x%-9x useresp 0x%-9x\n",
              reg->eip, reg->ss, reg->eflags, reg->useresp);
}

void print_reg16(register16_t * reg)
{
    dbg_print("Registers dump:\n");
    dbg_print("ax 0x%x bx 0x%x cx 0x%x dx 0x%x\n", reg->ax, reg->bx, reg->cx,
              reg->dx);
    dbg_print("di 0x%x si 0x%x bp 0x%x sp 0x%x\n", reg->di, reg->si, reg->bp,
              reg->sp);
    dbg_print("ds 0x%x es 0x%x fs 0x%x gs 0x%x ss 0x%x eflags 0x%x\n",
              reg->ds, reg->es, reg->fs, reg->gs, reg->ss, reg->eflags);
}
