/*
 * Dreamos
 * task.h
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
  
  
#ifndef _TASK__H
#define _TASK__H

#define MAX_TASKS 20
#define STACK_SIZE 0x1000 //Dimensione dello stack
#define NAME_LENGTH 30 //Task name length

typedef unsigned int pid_t;

/*!  \enum task_state
     \brief I possibili stati di un processo
 */
typedef enum {
	DEAD,
	READY,
	SLEEPING,
	NEW	
} task_state;

typedef struct {	
        unsigned int edi;   //+0
        unsigned int esi;   //+1
        unsigned int ebp;  //+2
        unsigned int esp;  //+3 (could be null)
        unsigned int ebx;  //+4
        unsigned int edx;  //+5
        unsigned int ecx;  //+6
        unsigned int eax;  //+7       
        unsigned int eip;  //+8
        unsigned int cs;   //+9
        unsigned int eflags;  //+10
        unsigned int end;    		
} task_register_t;


/*!  \struct task_t
     \brief Main Task Structure
 */
typedef struct task_t{
	pid_t pid;	
	char name[NAME_LENGTH];
	void (*start_function)();
	task_state status;
	task_register_t *registers;
	unsigned int cur_quants;
	unsigned int eip;
	unsigned int esp;
	unsigned int pdir;
	unsigned int ptable;
	struct task_t *next;
}task_t;

/*!  \struct task_list_t
     \brief Task List Structure
 */
typedef struct {
	//task_t list[MAX_TASKS];
	//int cur_task;
	task_t *head;
	task_t *tail;
	task_t *current;
} task_list_t;

/*!  \struct task_vector_t
     \brief Task Vector Structure
 */

typedef struct {
	task_t task_vector[MAX_TASKS];
} task_vector_t;

//extern task_t task_list[MAX_TASKS];
extern task_list_t task_list;
extern pid_t current_pid;

void tasks_init();
void enqueue_task(pid_t, task_t*);
task_t* get_task(pid_t);

pid_t new_task(char*, void (*start_function)());
unsigned int request_pid();
task_t* dequeue_task();

int isEmpty();
int getTaskListSize();

void test_tasklist();
#endif
