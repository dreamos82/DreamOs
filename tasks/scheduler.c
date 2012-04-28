/*
 * Dreamos
 * scheduler.c
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
  * Prima versione: 14/12/2011
  */

#include <scheduler.h>
#include <stdio.h>
#include <task.h>
#include <stddef.h>
#include <debug.h>
#include <paging.h>

task_list_t task_list;

unsigned char active;

void init_scheduler(){
	active=FALSE;
}

void schedule(unsigned int *stack){
	asm("cli");
	if(active == TRUE){
	  task_t* cur_task = dequeue_task();
	  if(cur_task != NULL){	    
	    dbg_bochs_print("@@@@@@@@@@@@@@@");
	    dbg_bochs_print(cur_task->name);
	    dbg_bochs_print("\n");
	    cur_task->esp=stack;
	    enqueue_task(cur_task->pid, cur_task);
	  }
	}
	active = FALSE;
	return;
	asm("sti");
}

void preSchedule(){
	active = TRUE;
}

void idle()
{		
	dbg_bochs_print("idle\n");	
    while(1){
		dbg_bochs_print("===IDLE===\n");
	}
	for(;;) {dbg_bochs_print("===IDLE===\n");}
}

void suicide()
{	
	task_list.head->status = DEAD;
	dbg_bochs_print("suicide\n");
    while(TRUE);
}

