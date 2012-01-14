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
#include <scheduler.h>

pid_t current_pid; 
int cur_free_index; 
task_list_t task_list;

void tasks_init(){
	//int i=0;
	printf("Init tasks");
	task_list.head = NULL;
	task_list.tail = NULL;			
	task_list.current = NULL;
	current_pid = 0;
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
void enqueue_task(pid_t pid, task_t* n_task){		
	if(task_list.head == NULL){
		//dbg_bochs_print("TaskList NULL\n");
		task_list.head = n_task;						
	} else {
		//dbg_bochs_print("TaskList Not Null\n");
		(task_list.tail)->next = (task_t*)n_task;				
	}
	task_list.tail = n_task;
	n_task=NULL;
}

/**
 * Retrieve  the task given the pid
 * @author Ivan Gualandri
 * @version 1.0
 * @param pid task pid
 */
task_t* get_task(pid_t pid){
	//Placeholtder for get_task
	printf("PlaceHolder for get_task\n");
    //task_t *_task = task_list.head;
	return (task_t *) NULL;
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
	task_t *new_task;
	table_address_t local_table;
	unsigned int new_pid = request_pid();	
	new_task = (task_t*)kmalloc(sizeof(task_t)); 	
	strcpy(new_task->name, task_name);
	new_task->next = NULL;
	new_task->start_function = start_function;
	new_task->cur_quants=0;
	new_task->pid = new_pid;
	new_task->eip = (unsigned int)start_function;
	new_task->esp = (unsigned int)kmalloc(STACK_SIZE) + STACK_SIZE-100;
	new_task->status = NEW;
	new_task->registers = (task_register_t*)new_task->esp;
	new_tss(new_task->registers, start_function);
	local_table = map_kernel();
	new_task->pdir = local_table.page_dir;
	new_task->ptable = local_table.page_table;
	enqueue_task(new_task->pid, new_task);
	if(task_list.current==NULL){
		//dbg_bochs_print("Current==NULL\n");
		task_list.current=new_task;
	} else {
		//dbg_bochs_print("Current!=NULL\n");
		(task_list.current)->cur_quants = MAX_TICKS;
	}
	asm("sti");
	return new_pid;
}

task_t* dequeue_task(){
    //pid_t pid; 	
	task_t* _task; 
	_task = (task_t*)task_list.head;
	task_list.head = (task_t*)_task->next;	
	return _task;
}

int isEmpty(){
	if(task_list.head == task_list.tail) return TRUE;
	else return FALSE;
}

/**
 * That function get the size of the tasklist. 
 * @author Ivan Gualandri
 * @version 1.0
 * @return Task List size
 */
int getTaskListSize(){
	task_t *_task = task_list.head;
	int i = 0;
	while(_task != task_list.tail){
		_task = (task_t*)_task->next;
		i++;
	}
	return i+1;
}

task_t* test_dequeue(){
	task_t* _task =  dequeue_task();
	return _task;
}

void test_tasklist(){
	asm("cli;");
	task_t* local_task;
	int i = 0;
	local_task = (task_t*) task_list.head;
	printf("PID\tName\n");	
	printf("%d\t%s\n", local_task->pid, local_task->name);
	local_task = (task_t*)local_task->next;		
	while(local_task!=task_list.tail && local_task!=NULL){
		//dbg_bochs_print("ps inside\n");
		printf("%d\t%s\n", local_task->pid, local_task->name);
		local_task = (task_t*)local_task->next;		
		i++;
	}	
	local_task=(task_t*)task_list.current;
	printf("%d\t%s\n", local_task->pid, local_task->name);
	asm("sti;");
	printf("TaskList size: %d\n", i);
}
