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

pid_t current_pid; 
task_t task_list[MAX_TASKS];

void tasks_init(){	
	current_pid = 0;	
	int i=0;
	while(i<MAX_TASKS){
		task_list[i].pid = 0;
		task_list[i].pdir = 0x0;
		i++;
	}
	printf("Init Tasks");	
	//_kprintOk();
	return;
}

unsigned int request_pid(){
	return current_pid++;
}

void new_task(){
	
}

