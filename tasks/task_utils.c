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
table_address_t map_kernel(){
	table_address_t logicmemory_root;
	unsigned int *logic_pd_address = kmalloc(8*4096);
	unsigned int *logic_pt_address = kmalloc(8*4096);
	unsigned int *pagedir;
	unsigned int *pagetable;
	unsigned int i=0;	
	unsigned int j=0;
	/** Request physical space for pagedir and pagetable*/ 	
	pagedir = create_pageDir();
	pagetable = create_pageTable();	
	//set_pagedir_entry_ric(1023, fis_address, PD_PRESENT|SUPERVISOR, 0);
	/*new_pd_node->start_address = (heap_node_t*) pagedir;
	new_pd_node->size = 8 * 4096;*/
	/**Map the logic addresses for pagedir and pagetable into phisical address
	 * identified by the variables pagetable and pagedir*/
	while(i < 8){
		map_address(*pagedir, *logic_pd_address);
		map_address(*pagetable, *logic_pt_address);		
		pagedir+=4096;
		pagetable+=4096;
		i++;
	}	
	//set_pagedir_entry_ric(0, (unsigned int)current_page_table, PD_PRESENT|SUPERVISOR|WRITE,0);  
	/**Initialize the pagedir with all 0's and the pagetable with the locations of the kernel*/  
	while(j<PD_LIMIT){
		pagedir[j] = 0x0;
		pagetable[j] = (j*0x1000&0xFFFFF0)|PD_PRESENT|SUPERVISOR|WRITE|0;
		j++;
	}
	/**Add the pagetable to first entry of the pagedir*/
	pagedir[0] = ((unsigned int)pagetable&0xFFFFF000)|PD_PRESENT|SUPERVISOR|WRITE|0;
	pagedir[1023] = ((unsigned int)pagedir&0xFFFFF000)|PD_PRESENT|SUPERVISOR|0;	
	/**Prepare the result and return it*/
	logicmemory_root.page_dir = (unsigned int) pagedir;
	logicmemory_root.page_table = (unsigned int) pagetable;
	return logicmemory_root;
}
