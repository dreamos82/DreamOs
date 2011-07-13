/*
 * Dreamos
 * task.c
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
  
#include <task.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

pid_t current_pid; 
task_t task_list[MAX_TASKS];

void tasks_init(){		
	current_pid = 0;	
	int i=0;
	while(i<MAX_TASKS){
		task_list[i].pid = 0;
		task_list[i].pdir = 0x0;
		task_list[i].state = DEAD;
		task_list[i].start_function = NULL;
		task_list[i].esp = 0x0;
		task_list[i].pdir = 0x0;
		task_list[i].ptable = 0x0;
		i++;
	}
	printf("Init Tasks");	
	//_kprintOk();
	return;
}
/**TODO: add pid number check*/
unsigned int request_pid(){
	return current_pid++;
}

void new_task(char *task_name, void (*start_function)()){
	asm("cli");
	task_t new_task;
	unsigned int new_pid = request_pid();
	strcpy(new_task.name, task_name);
	new_task.pid = new_pid;			
	new_task.eip = (unsigned int)start_function;	
	asm("sti");
	return;
}

