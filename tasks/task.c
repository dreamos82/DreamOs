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
#include <debug.h>

pid_t current_pid; 
int cur_free_index; 
task_list_t task_list;
task_array_t task_array;

void tasks_init(){
	asm("cli");	
	printf("Init tasks");
	dbg_bochs_print("Task array setup");
	int i=0;
	while(i<MAX_TASKS){
	  task_array.list[i].status = DEAD;
	  i++;
	}
	task_array.cur_task = 0;
	task_array.size = 0;
	task_list.head = NULL;
	task_list.tail = NULL;
	task_list.size = 0;
	current_pid = 0;
	asm("sti");
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
  n_task->next=NULL;
  if(task_list.tail == NULL){
    task_list.head = n_task;
    task_list.tail = task_list.head;    
  } else {
    task_list.head->next=n_task;    
    task_list.head = n_task;    
  }
}

/**
 * Retrieve  the task given the pid
 * @author Ivan Gualandri
 * @version 1.0
 * @param pid task pid
 */
task_t* get_task(){	
	return (task_t *) task_list.tail;
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
	//new_task->pdir = 0;
	//new_task->ptable = 0;
	enqueue_task(new_task->pid, new_task);
	//(task_list.current)->cur_quants = MAX_TICKS;			
	asm("sti");
	return new_pid;
}

/**
 * That function get the next task on the queue
 * @author Ivan Gualandri
 * @version 1.0
 * @return Get the next task on the queue
 */
task_t* dequeue_task(){
	if(task_list.head==NULL){
	  return NULL;
	} else {
	  task_t* _task;
	  _task = task_list.tail; 
	  task_list.tail=_task->next;
	  return _task;
	}
	return;
}

int isEmpty(){
  if(task_array.size==0)
    return FALSE;
  else 
    return TRUE;
}

/**
 * That function get the size of the tasklist. 
 * @author Ivan Gualandri
 * @version 1.0
 * @return Task List size
 */
int getTaskListSize(){
	task_t *local_task;
	if(task_list.tail == NULL) {
	  return -1;
	}
	local_task=task_list.tail;
	int i = 0;
	while(local_task!=NULL){
	  i++;
	  local_task=local_task->next;
	}
	//if(task_list.current!=NULL) i++;	
	return i;
}

void test_dequeue(){
	task_t* _task =  dequeue_task();
}

void test_tasklist(){
	asm("cli;");
	task_t* local_task;
	local_task = (task_t*) task_list.tail;
	printf("Name\tPid\n");
	while(local_task!=NULL){
	  printf("%s\t%d\n", local_task->name, local_task->pid);
	  local_task=local_task->next;
	}
	printf("Size: %d\n", getTaskListSize());
 	asm("sti;");
}
