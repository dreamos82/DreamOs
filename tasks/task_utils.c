/***************************************************************************
 *            task_utils.c
 *
 *  Sat May  7 2011
 *  Copyright  2011  Ivan Gualandri
 *  Email
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <task_utils.h>
#include <fismem.h>
#include <paging.h>
#include <kheap.h>
#include <ordered_list.h>

/**
   * Make a copy of the kernel into a new pagedir/pagetable
   * @author Ivan Gualandri
   * @version 1.0 
   */
unsigned int map_kernel(){
	unsigned int *logic_pd_address = kmalloc(8*4096);
	unsigned int *pagedir;
	unsigned int *pagetable;
	pagedir = create_pageDir();
	pagetable = create_pageTable();
	new_pd_node->start_address = (heap_node_t*) pagedir;
	new_pd_node->size = 8 * 4096;
	
	//set_pagedir_entry_ric(0, (unsigned int)current_page_table, PD_PRESENT|SUPERVISOR|WRITE,0);    
	
	return 0;
}
