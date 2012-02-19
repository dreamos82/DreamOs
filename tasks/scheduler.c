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
	if(active == TRUE){
		dbg_bochs_print((const unsigned char*)"Scheduler called\n");
		task_t* cur_task;
		task_t* next_task;				
		if(task_list.size>1){
			cur_task = (task_t*)task_list.current;
			next_task = dequeue_task();
			dbg_bochs_print("OK\n");			
			cur_task->esp = *stack;
			enqueue_task(cur_task->pid, cur_task);				
			dbg_bochs_print(next_task->name);		
			dbg_bochs_print("-");
			dbg_bochs_print(cur_task->name);		
			dbg_bochs_print("\n");
			while(next_task->status!= READY && next_task->status != NEW){
				dbg_bochs_print((const unsigned char*)"Not READY or NEW");
				enqueue_task(next_task->pid, next_task);
				next_task=dequeue_task();		
				//printf("Stack: %d", stack);			
			}
			//Switch context logic goes here.		
			task_list.current = next_task;
			*stack = next_task->esp;			
			}
			//load_pdbr(next_task->pdir);		
		active = FALSE;
	}	
	return;
}

void preSchedule(){
	active = TRUE;
}

void idle()
{		
	dbg_bochs_print("idle\n");	
    while(TRUE){
		dbg_bochs_print("===IDLE===\n");
	}
}

void suicide()
{	
	dbg_bochs_print("suicide\n");
    while(TRUE);
}

