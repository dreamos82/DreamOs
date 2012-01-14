/*
 * Dreamos
 * tss.c
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
 /*
  * Autore Ivan Gualandri
  * Prima versione: 20/06/2011
  */
  

#include <tss.h>
#include <task.h>
#include <stddef.h>
#include <scheduler.h>

void new_tss(task_register_t* tss, void (*func)()){
    tss->eax=0;
    //can't be NULL; set 0 instead
    tss->esp=0;
    tss->ebx=0;
    tss->ecx=0;
    tss->edx=0;
    tss->edi =0;
    tss->esi =0;
    tss->cs = 8;
    tss->eip = (unsigned)func;
    tss->eflags = 0x202;
    tss->end = (unsigned) suicide;
    //tss->fine = (unsigned)end; //per metterci il suicide	
	return;
}
