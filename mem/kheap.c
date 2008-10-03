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
#include <ordered_list.h>
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
    first_node->next = NULL;
    new_heap->max_size = tot_mem-(unsigned int) &end;
    new_heap->free_list = first_node;
    new_heap->used_list = NULL;
    printf("First heap created...\n");   
    printf("Size: %d - Tot mem: %d - Address: %x\n", (new_heap->free_list)->size, tot_mem, new_heap);    
    return (heap_t*) new_heap;
}

void* alloc(unsigned int size, heap_t *cur_heap){    
    int n_pages;
    n_pages = size/4096;
    if(size%4096!=0) n_pages++;
    printf("Numero pagine da allocare: %d, %d\n", n_pages, size);
    heap_node_t* cur_list;    
    printf("Cur_heap->max_size %d\n", cur_heap->max_size);
    cur_list = cur_heap->free_list;
    printf("Arrivo qua\n");
    /*Vado alla ricerca di una locazione di memoria libera*/
    while(cur_list!=NULL) {
        if(cur_list->size > size){
            heap_node_t* new_node;

            new_node = (heap_node_t*)kmalloc(sizeof(heap_node_t));
            new_node->start_address = cur_list->start_address;
            new_node->next = NULL;
            new_node->size = n_pages*0x1000;
//             new_node->next= cur_heap->used_list; 
//             cur_heap->used_list = new_node;
            /*Inserisci il nodo in testa*/
            printf("New_node: Size: %d, start_address: %d\n", new_node->size, new_node->start_address);
            printf("Cur_list Actual size: %d, start_address: %d\n", cur_list->size, cur_list->start_address);
            cur_list->size = (cur_list->size) - (n_pages*0x1000);
            cur_list->start_address = cur_list->start_address - (n_pages*0x1000);
            printf("New_size: %d, new_start_address %d\n", cur_list->size, cur_list->start_address);      

             insert_list(new_node,&(cur_heap->used_list));                            
        }
        else printf("No good\n");
        printf("The size of that node is: %d\n", cur_list->size);
        printf("Value: %d\n", cur_list->next); 
        cur_list = cur_list->next;
    }
}

/**Queste funzioni sono messe solo per alcune prove verranno eliminate entro le prossime revisioni*/
void try_alloc(){
    alloc(5000, kheap);
    alloc(60, kheap);    
}
