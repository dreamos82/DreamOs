/***************************************************************************
 *            kheap.c
 *
 *  Sun 18 07 08 07:47:17 2007
 *  Copyright  2008  Ivan Gualandri
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

#include <kheap.h>
#include <fismem.h>
#include <paging.h>
#include <ordered_array.h>
#include <stddef.h>
#include <stdio.h>

extern unsigned int end;
extern size_t tot_mem;
heap_t *kheap = 0;
unsigned int address_cur = (unsigned int) &end;

void* kmalloc(unsigned int size){
/** Questa funzione andra' modificata con l'evoluzione del paging.
  */
    unsigned int temp;
    
    temp = address_cur;
    address_cur+=size;
    return (void *) temp;
}

/**
  * Build a new heap
  * @author Ivan Gualandri
  * @version 1.0
  * @param start Indirizzo di inizio del nostro heap
  * @param end Indirizzo di fine dell'heap
  * @param size grandezza massima dell'heap
  * @return Pointer to a new heap
  */
heap_t* make_heap(unsigned int start, unsigned int end, unsigned int size){
    heap_t* new_heap;
    heap_node_t* first_node;
    new_heap = (heap_t*)kmalloc(sizeof(heap_t));
    first_node = (heap_node_t*)kmalloc(sizeof(heap_node_t));
    first_node->start_address = (unsigned int)&end;
    first_node->size = size;
    new_heap->max_size = tot_mem-(unsigned int) &end;
    new_heap->free_list = first_node;
    new_heap->used_list = NULL;
    printf("First heap created...\n");   
    printf("Size: %d - Tot mem: %d - Address: %x\n", (new_heap->free_list)->size, tot_mem, new_heap);    
    return (heap_t*) new_heap;
}

void* alloc(unsigned int size, heap_t *cur_heap){    
    heap_node_t* cur_list;
    cur_list = cur_heap->free_list;
    while(cur_list!=NULL) {
        printf("All right let's go\n");
        cur_list = cur_list->next;
    }
}

/**Queste funzioni sono messe solo per alcune prove verranno eliminate entro le prossime revisioni*/
void try_alloc(){
    alloc(4, kheap);
}
