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

/**Please remember that header->size include also sizeof header + sizeof footer*/
#include <heap.h>
#include <stdio.h>
#include <ordered_array.h>
#include <kheap.h>
#include <stddef.h>

extern unsigned int end;
extern unsigned int address_cur;
new_heap_t *n_heap;


void init_newmem(){
	n_heap =0;
	n_heap = (new_heap_t*)new_heap(HEAP_START_ADDRESS, HEAP_INDEX_SIZE);	
}

short int header_t_less_than(void *a,void *b){
	return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

new_heap_t *new_heap(unsigned int start, unsigned int size){
	/*Actually we need to use kmalloc!*/
	new_heap_t* t_heap =(new_heap_t*) new_malloc(sizeof(new_heap_t));		
	if(start%0x1000 == 0){		
		header_t *first_hole = (header_t*)start;		
		t_heap->index = new_array((void*)start,size, &header_t_less_than);
		t_heap->start_address = start;
		t_heap->end_address = start + size;
		t_heap->max_address = 0xDFFFF000; /**Da controllare max address*/					
		first_hole->size = t_heap->end_address - start;		
		first_hole->magic = HEAP_MAGIC;		
		first_hole->is_hole = 1; /*TRUE*/			
		insert_array((void*) first_hole, &t_heap->index);
		//printf("First hole: %u size: %u", t_heap->start_address, t_heap->end_address);
		//get_array(0, &t_heap->index);
		//locate_smallest_hole(size, PAGE_ALIGNED,&t_heap->index);
		return t_heap;
	}
	else printf("ERROR\n");
	return 0;
}

unsigned int new_malloc(unsigned int size){
	if(n_heap==0) {
		//I have no heap defined, i continue to use address_cur.
		unsigned int tmp;
		//printf("No heap defined starting %x...\n", address_cur);
		tmp = address_cur; 
		address_cur += size;
		printf("New address: %x\n", address_cur);
		return tmp;
	}
	else {
		//If i have a heap, then i have vm management enabled, and i can call alloc...
		void* new_address = new_alloc(size, PAGE_ALIGNED, n_heap);	
		//&printf("Heap defined\n");
		printf("Start Address: %x End Address: %x\n", n_heap->start_address, n_heap->end_address);
		//printf("Address_cur value: %x\n", address_cur);
		return (unsigned int)new_address;
	}
	return 0;
}

void *new_alloc(unsigned int size, unsigned short int p_aligned, new_heap_t* t_heap){
	//#define MEMDEBUG 1
	#ifdef  MEMDEBUG
	unsigned int real_size = size +sizeof(header_t) + sizeof(footer_t);	
	//printf("Size of:\n\theader_t: %d\n\tfooter_t: %d\n\treal_size: %d\n\tsize: %d\n", sizeof(header_t), sizeof(footer_t), real_size, size);	
	printf("Real size: %d\n", real_size);
	unsigned int min_index = locate_smallest_hole(real_size, PAGE_ALIGNED, t_heap);
	printf("Real size: %d - min_index: %d\n", real_size, min_index);
	if(min_index == -1 ){
		/**No hole with the requested size found, asking more space for heap*/		
		unsigned int old_len = t_heap->end_address - t_heap->start_address;		
		unsigned int old_end_address = t_heap->end_address;			
		/**expand the current heap*/
		printf("Old len value: %x - ", old_len);
		expand(old_len+real_size, t_heap);
		unsigned int new_len = t_heap->end_address - t_heap->start_address;		
		printf("New len value: %x\n", old_len);		
		min_index=0;
		unsigned int latest_i = -1;
		unsigned int val = 0x0;
		/**Find the latest hole*/
		while (min_index < t_heap->index.size){
			unsigned int temp = get_array(min_index, &t_heap->index);
			if(temp > val){
				val = temp;
				latest_i = min_index;
			}
			min_index++;
		}
		if(latest_i == -1){
			/**We need to create a new hole*/
			header_t *header = (header_t *)old_end_address;
			header->magic = HEAP_MAGIC;
			header->size = new_len - old_len;
			header->is_hole = HEAP_HOLE;
			footer_t *footer = (footer_t *)(old_end_address + header->size - sizeof(footer_t));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
			insert_array((void_t*)header, &t_heap->index);
		}
		else {
			/**We need to adjust the latest hole*/
			header_t* header = get_array(min_index, &t_heap->index);
			header->size += new_len - old_len;
			/**We need a new footer*/
			footer_t* footer = (footer_t*) (header + header->size - sizeof(footer_t));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
		}
		printf("Recursing!!! \n");
		return new_alloc(size, p_aligned, t_heap);
	}
	else {
		header_t *header = get_array(min_index, &t_heap->index);
		unsigned int hole_address = (unsigned int)header;
		unsigned int hole_size = header->size;
		//printf("HOLE FOUND\n");
		printf(" In new_alloc: Header: 0x%x\t Real Size: 0x%x\n", header->magic, real_size);
		if(hole_size - real_size < sizeof(header_t) + sizeof(footer_t)){
			/*We can't split the hole!*/
			//printf("A good step forward");
			size = hole_size + real_size;
			real_size = hole_size;			
		}
		else remove_array(min_index, &t_heap->index);
		//Temporaneamente commentato!	
		//#ifdef DEBUG
		header_t *block_header  = (header_t *)hole_address;
		block_header->magic = HEAP_MAGIC;
		block_header->size = real_size;
		block_header->is_hole= HEAP_BLOCK;
		footer_t *block_footer = (footer_t *)(hole_address + size + sizeof(header_t));
		block_footer->magic = HEAP_MAGIC;
		block_footer->header = block_header;
		printf("Block_header: %x - End of block_footer: %x\n", (unsigned int)block_header, (unsigned int)block_footer);
		//#endif
		if(hole_size - real_size >0){
			/*We need to add a new hole. The new_hole address is given by:
			 * the current hole address + size + sizeof(header_t) + sizeof(footer_t) 
			 **/			
			header_t *head_hole = (header_t *)(hole_address + sizeof(header_t) + size + sizeof(footer_t));
			printf("Add a new hole! New address: %x\n", (unsigned int) head_hole);			
			head_hole->magic = HEAP_MAGIC;
			head_hole->is_hole = HEAP_HOLE;
			head_hole->size = hole_size - real_size;
			footer_t *footer_hole = (footer_t*)(hole_address + head_hole->size - sizeof(footer_t));
			footer_hole->magic = HEAP_MAGIC;
			footer_hole->header = head_hole;
			insert_array((void *)head_hole, &t_heap->index);			
		}	
		return (void *)((unsigned int)block_header+sizeof(header_t));
		//get_array(0, &t_heap->index);
	}	
	#endif
	return NULL;
}

void expand(unsigned int new_size, new_heap_t *t_heap){
	//unsigned int old_size = t_heap->end_address - t_heap->start_address; 
	/** First of all: Align the space required*/
	if((new_size&0xFFFFF000) != 0){
		new_size&=0xFFFFF000;
		new_size+=0x1000;
	}	
	/**We have pages created when #PF occurs, so we only need to update 
	 * t_heap->end_address*/
	t_heap->end_address = t_heap->start_address + new_size;		
}

unsigned int contract(unsigned int new_size, new_heap_t *t_heap){
	//printf("PlaceHolder for new function contract\n");
	if(new_size&0x1000){
        new_size &= 0x1000;
        new_size += 0x1000;
    }    
    if(new_size < HEAP_MIN_SIZE) new_size = HEAP_MIN_SIZE;
	t_heap->end_address = t_heap->start_address + new_size;
	return new_size;
}

short int locate_smallest_hole(unsigned int size, unsigned short int p_align, new_heap_t* in_heap){
	unsigned int index = 0; 
	//printf("Size index: %d\n", in_heap->index.size);
	while(index < in_heap->index.size){
		unsigned int h_size;
		header_t *header = (header_t *)get_array(index, &in_heap->index);
		printf("Header: 0x%x\tSize: 0x%x\n", header->magic, header->size);
		h_size = header->size;
		if(h_size >= size)
			break;
		index++;
	}
	if(index >= in_heap->index.size)
		return -1;
	else 
		return index;
}

void new_free(void *address, new_heap_t* t_heap){	
	if(address==0){
		//printf("DoNothing\n");
		return;
	}
	header_t* header = (header_t*)((unsigned int)address - sizeof(header_t));	
	footer_t* footer = (footer_t*)((unsigned int)header + header->size - sizeof(footer_t));	
	/**Set header as a hole*/
	header->is_hole = HEAP_HOLE;
	char to_add = 1;
	/**Unify left*/
	/**Test if on the left i have a hole*/
	footer_t* left_footer = (footer_t*) ((unsigned int) header - sizeof(footer_t));
	printf("Address of header: %x - Footer: %x Left: %x\n", header, footer, footer->magic);
	if(left_footer->magic == HEAP_MAGIC && left_footer->header->is_hole == HEAP_HOLE){
		printf("unify left\n");
		/**I have found a hole on the left of my current header i need to save the size of
		 * my current block*/
		unsigned int cur_block_size = header->size;
		/**Now i update my header pointer to the new hole found*/
		header = left_footer->header;
		header->size = header->size + cur_block_size;
		/**Now i update the address of the header into footer*/
		footer->header = header;
		to_add = 0;
	}
	header_t* right_header = (header_t*) ((unsigned int)footer + sizeof(footer_t));
	if(right_header->magic == HEAP_MAGIC && right_header->is_hole == HEAP_HOLE){
		unsigned int i = 0;
		printf("unify right\n");
		/**First: update the size of the new header*/
		header->size = header->size + right_header->size;
		left_footer = (footer_t*) ((unsigned int) right_header + right_header->size - sizeof(footer_t));
		footer = left_footer;
		while((i < t_heap->index.size) && get_array(i, &t_heap->index) != (void_t*)right_header)			
			i++;
		if(i<t_heap->index.size) remove_array(i, &t_heap->index);		
	}
	/**We must check if we aren't at the end of memory*/
	if(((unsigned int)footer + sizeof(footer_t)) == t_heap->end_address){
		unsigned int old_size = t_heap->end_address - t_heap->start_address;
		unsigned int new_size = contract((unsigned int)header - t_heap->start_address, t_heap);
		/**We need to resize?*/
		printf("Test\n");
		if(header->size - (old_size-new_size) > 0){
			footer = (footer_t *) (unsigned int)header + header->size - sizeof(footer_t);
			footer->magic = HEAP_MAGIC;
			footer->header = header;
		}
		else {
			unsigned int i = 0;
			while(i<t_heap->index.size && get_array(i, &t_heap->index)!=(void_t*)right_header)
				i++;
			if(i < t_heap->index.size)
				remove_array(i, &t_heap->index);
		}
	}
	if(to_add == 1) {
		printf("And then\n");
			insert_array((void_t*) header, &t_heap->index);
	}	
}
