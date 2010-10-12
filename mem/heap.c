/***************************************************************************
 *            heap.c
 *
 *  Sun 8 08 10 
 *  Copyright  2010  Ivan Gualandri
 *  Email finarfin@elenhost.org
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

#include <heap.h>
#include <stdio.h>
#include <ordered_array.h>
#include <kheap.h>

extern unsigned int end;
extern unsigned int address_cur;
new_heap_t *n_heap;
//unsigned int placement_address=(unsigned int) &end;


void init_newmem(){
	//placement_address = address_cur;	
	n_heap = (new_heap_t*)new_heap(HEAP_START_ADDRESS, HEAP_INDEX_SIZE);
}

static short int header_t_less_than(void *a,void *b){
	return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

new_heap_t *new_heap(unsigned int start, unsigned int size){
	new_heap_t* t_heap =(new_heap_t*) new_malloc(sizeof(new_heap_t));	
	if(start%0x1000 == 0){		
		header_t *first_hole = (header_t*)start;
		t_heap->start_address = start;
		t_heap->end_address = start + size;	
		t_heap->index = new_array((void*)start,size, &header_t_less_than);
		printf("Start_address: %u End Address: %u\n", n_heap->start_address, n_heap->end_address);		
		printf("Making first Hole\n");		
		first_hole->size = t_heap->end_address - start;
		first_hole->magic = HEAP_MAGIC;
		first_hole->is_hole = 1; /*TRUE*/
		return t_heap;
	}
	else printf("ERROR\n");
	return 0;
}

unsigned int new_malloc(unsigned int size){
	if(n_heap==0) {
		unsigned int tmp;
		printf("No heap defined starting from: %d and %x...\n", &end, address_cur);
		tmp = address_cur; 
		address_cur = address_cur+size;
		printf("New address: %x\n", tmp);
		return tmp;
	}
	else {
		printf("Heap defined\n");
		printf("Start Address: %u End Address: %u\n", n_heap->start_address, n_heap->end_address);
		return 0;
	}
	return 0;
}
