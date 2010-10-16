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


void init_newmem(){
	n_heap = (new_heap_t*)new_heap(HEAP_START_ADDRESS, HEAP_INDEX_SIZE);
	//printf("Start_address: %u End Address: %u\n", n_heap->start_address, n_heap->end_address);		
}

static short int header_t_less_than(void *a,void *b){
	return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

new_heap_t *new_heap(unsigned int start, unsigned int size){
	new_heap_t* t_heap =(new_heap_t*) kmalloc(sizeof(new_heap_t));	
	if(start%0x1000 == 0){		
		header_t *first_hole = (header_t*)start;		
		t_heap->index = new_array((void*)start,size, &header_t_less_than);
		t_heap->start_address = start;
		t_heap->end_address = start + size;			
		//printf("Making first Hole\n");	
		//This should be inserted into the ordered array.	
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
		//I have no heap defined, i continue to use address_cur.
		unsigned int tmp;
		printf("No heap defined starting from: %d and %x...\n", &end, address_cur);
		tmp = address_cur; 
		address_cur = address_cur+size;
		printf("New address: %x\n", tmp);
		return tmp;
	}
	else {
		//If i have a heap, then i have vm management enabled, and i can call alloc...
		void* new_address = new_alloc(size, 1, n_heap);
		printf("Heap defined\n");
		printf("Start Address: %u End Address: %u\n", n_heap->start_address, n_heap->end_address);
		if(n_heap->start_address==0) printf("erroso\n");
		return 0;
	}
	return 0;
}

void *new_alloc(unsigned int size, unsigned short int p_aligned, new_heap_t *t_heap){
	printf("Step by step\n");
}
