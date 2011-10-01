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
#include <kheap.h>
#include <tss.h>
#include <task_utils.h>

pid_t current_pid; 
int cur_free_index; 
task_t task_list[MAX_TASKS];

void tasks_init(){		
	current_pid = 0;
	current_free_index=0;	
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

/**
 * That function add a task to the current task list 
 * @author Ivan Gualandri
 * @version 1.0
 */
void add_task(pid_t pid, task_t* cur_task){
	task_list[pid] = *cur_task;
}

/**
 * Retrieve  the task given the pid
 * @author Ivan Gualandri
 * @version 1.0
 * @param pid task pid
 */
task_t get_task(pid_t pid){
	return task_list[pid];	
}

/**
 * Create a new task 
 * @author Ivan Gualandri
 * @version 1.0
 * @param task_name The name of the task
 * @param start_function the entry point of the task.
 */
pid_t new_task(char *task_name, void (*start_function)()){
	asm("cli");
	task_t new_task;
	table_address_t local_table;
	unsigned int new_pid = request_pid();	
	strcpy(new_task.name, task_name);
	new_task.start_function = start_function;
	new_task.pid = new_pid;			
	new_task.eip = (unsigned int)start_function;	
	new_task.esp = (unsigned int) kmalloc(STACK_SIZE) + STACK_SIZE - 100;
	new_task.state = READY;
	new_task.registers = (task_register_t*)new_task.esp;
	new_tss(new_task.registers, start_function);
	local_table = map_kernel();
	new_task.pdir = local_table.page_dir;
	new_task.ptable = local_table.page_table;
	add_task(new_task.pid, &new_task);	
	asm("sti");
	return new_pid;
}

