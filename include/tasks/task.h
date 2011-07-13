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

#define MAX_TASKS 10

typedef unsigned int pid_t;

/*!  \enum task_state
     \brief I possibili stati di un processo
 */
typedef enum {
	DEAD,
	READY,
	SLEEPING
} task_state;

typedef struct {	
        unsigned int edi;   //+0
        unsigned int esi;   //+1
        unsigned int ebp;  //+2
        unsigned int null;  //+3
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
typedef struct {
	pid_t pid;	
	char *name;
	void (*start_function)();
	task_state state;
	task_register_t *registers;
	unsigned int eip;
	unsigned int esp;
	unsigned int pdir;
	unsigned int ptable;
} task_t;

extern task_t task_list[MAX_TASKS];
extern pid_t current_pid;

void tasks_init();
void new_task(char*, void (*start_function)());
void add_task(unsigned int, task_t);

unsigned int request_pid();

#endif
