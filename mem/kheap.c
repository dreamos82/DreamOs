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
#define KHEAP_LIST_ADDRESS 0xC0000000

// #define DEBUG 1
extern unsigned int end;
extern size_t tot_mem;
heap_t *kheap;
heap_t *kheap_tmp;
unsigned int address_cur = (unsigned int) &end;
unsigned int node_address;

void* kmalloc(unsigned int size)
{
   unsigned int temp;
   if(kheap!=0) {
     if (size>0) return (void *) alloc(size, kheap);
   }
   else {            
     temp = address_cur;
     address_cur+=size;
   }
     return (void *) temp;
}

/* Test procedure ("try_heap" shell command") */
void try_alloc()
{
   
    printf("try_alloc(): Used list address: %d, Value: %d\n", &(kheap->used_list), kheap->used_list);
    alloc(50, kheap);
    alloc(60, kheap);
    alloc(50, kheap);
    alloc(100, kheap);
    alloc(6, kheap);    
    printf("Used\n");
    print_heap_list (kheap->used_list);
    printf("Free\n");
    print_heap_list (kheap->free_list);
    printf("Free nodes\n");
    print_heap_list(kheap->free_nodes);
}

/**
  * Build a new heap
  * @author Ivan Gualandri
  * @version 1.0
  * @param start Heap start address
  * @param end Heap end address
  * @param size Heap maximum size
  * @return Pointer to a new heap
  */
heap_t* make_heap(unsigned int size)
{
    heap_t* new_heap;
    heap_node_t* first_node;

    new_heap = (heap_t*)KHEAP_LIST_ADDRESS;
    node_address = KHEAP_LIST_ADDRESS + sizeof(heap_t);
//     new_heap = (heap_t*)kmalloc(sizeof(heap_t));
//     first_node = (heap_node_t*)alloc_node();
    first_node = (heap_node_t*)node_address;
    node_address = node_address + sizeof(heap_node_t);
    first_node->start_address = (unsigned int)&end;
    first_node->size = size;
    first_node->next = NULL;

    new_heap->free_nodes = NULL;
    new_heap->max_size = tot_mem-(unsigned int) &end;
    new_heap->free_list = first_node;
    new_heap->used_list = NULL;
    new_heap->free_nodes = NULL;
    printf("  First heap created...\n");   
    printf("  Size: %d\n"
	   "  Tot mem: %d\n"
	   "  Start address: %x\n", (new_heap->free_list)->size, tot_mem, new_heap);    
	set_pagetable_entry_ric(1019, 512 ,0xFEE00000, SUPERVISOR|PD_PRESENT|WRITE, 0);
    return (heap_t*) new_heap;
}
    
/**
  * Allocation
  * @author Ivan Gualandri
  * @version 1.0
  * @param size Size of the memory to be allocated
  * @param cur_heap Current heap
  * @return The start address of the new allocated memory (or NULL if no memory can be allocated)
  */
void *alloc(unsigned int size, heap_t *cur_heap)
{    
    heap_node_t* new_node = NULL;
    heap_node_t* free_heap_list = cur_heap->free_list;    
    heap_node_t* prev_node;
    #ifdef DEBUG
    printf("----\n");
    printf("Required Size: %d ", size);    
    #endif
    prev_node = free_heap_list;

    /* Look for a free block of memory in the heap's free memory list */    
    while(free_heap_list) {
      if(free_heap_list->size >= size) {   
        #ifdef DEBUG
        printf("Available_pages: %d\n", free_heap_list->size / 4096);
        #endif
        /*Se lo spazio disponibile e' maggiore di quello richiesto*/
        if(free_heap_list->size > size){
            #ifdef DEBUG
            printf("Node should be splitted\n");
            #endif
            new_node = (heap_node_t*)alloc_node();            
            new_node->start_address = free_heap_list->start_address;            
            new_node->next = NULL;
            new_node->size = size;
            insert_list (new_node, &(cur_heap->used_list));
            free_heap_list->size = (free_heap_list->size) - (size);
            free_heap_list->start_address = free_heap_list->start_address + (size);
            #ifdef DEBUG
            printf("New_node -> Size: %d, start_address: %d\n", new_node->size, new_node->start_address);
            #endif
        }
        /*Se lo spazio richiesto e' uguale a quello disponibile*/         
        else if(free_heap_list->size == size){
            if(prev_node == free_heap_list) {                
                kheap->free_list = (heap_node_t*)free_heap_list->next;
            }            
            else prev_node->next = free_heap_list->next;
            insert_list (free_heap_list, &(cur_heap->used_list));
            return (void *)free_heap_list->start_address;            
        }        
        #ifdef DEBUG        
        printf("free_heap_list -> Actual size: %d, start_address: %d\n", free_heap_list->size, free_heap_list->start_address);
        printf("----\n");
        #endif
        break;
      }
      else {                
        prev_node = free_heap_list;
        free_heap_list = (heap_node_t*)free_heap_list->next;
      }      
   }
   #ifdef DEBUG
   printf("Prev_node: %d \n", prev_node->start_address);
   printf("New Address: %d ", new_node->start_address);    
   #endif   
   return (void *)new_node->start_address;
}

/**
  * Allocation of a new node for kheap
  * @author Ivan Gualandri
  * @version 1.0
  * @return The start address of the new allocated node
  */
heap_node_t* alloc_node(){
    unsigned int temp;    
   heap_node_t *new_address;
    if(kheap->free_nodes!=NULL){      
      new_address = kheap->free_nodes;
      kheap->free_nodes = new_address->next;
      #ifdef DEBUG
      printf("riciclo un nodo\n");
      #endif
      return new_address;
    }
    else {
      temp = node_address;
      node_address = node_address + sizeof(heap_node_t);
      return (heap_node_t*) temp;
    }
}

/**
  * Free a block of memory
  * @author shainer
  * @version 1.0
  * @param Start_address of the block
  * @return none
  */
void free (void *location)
{
  heap_node_t *busy = kheap->used_list;
  heap_node_t *prev = busy;
//   heap_node_t *free = kheap->free_list;
  heap_node_t *n2;

	//if ((unsigned int)location % 4 != 0) return -1;
	//     printf ("Indirizzo non allineato a 4kb\n");

  while (busy) {
    if (busy->start_address == (unsigned int)location) {
      if (prev == busy) { /* node is the first */
        kheap->used_list = (heap_node_t*)busy->next;
      } else
        prev->next = busy->next;
        
      insert_list (busy, &(kheap->free_list)); // insert node in the free_list

      /* 
       * Deallocation finished, starting merge...
       * I need "busy", the next (n2) and the previous (prev) node
       */
      n2 = kheap->free_list;
      prev = kheap->free_list;
      while (n2 != busy) {
        prev = n2;
        n2 = (heap_node_t*)n2->next;
      }
      n2 = (heap_node_t*)busy->next;
      #ifdef DEBUG
      printf ("--Pre merge--\n");
      printf ("Prev address: %d, size: %d\n", prev->start_address, prev->size);
      printf ("Busy address: %d, size: %d\n", busy->start_address, busy->size);      
      if (n2)
        printf ("N2 address: %d, size %d\n\n", n2->start_address, n2->size);
      #endif
      /* Merge busy into prev */
      if (prev != busy && (prev->start_address + prev->size) == busy->start_address) {
        #ifdef DEBUG
        printf ("Backward merge\n");
        #endif
        prev->size += busy->size;
        prev->next = n2;
	    free_node(busy);
// 	insert_list (busy, &(kheap->free_nodes));
      }
      /* Merge n2 into busy */
      if (n2 && (busy->start_address + busy->size) == n2->start_address) {
        #ifdef DEBUG
        printf ("Forward merge\n");
        #endif
        busy->size += n2->size;
        busy->next = n2->next;
// 	insert_list (n2, &(kheap->free_nodes));

    	free_node(n2);
      }

      break;
    }
    //printf("busy: %d - free: %d\n", busy->start_address, location );
    prev = busy;
    busy = (heap_node_t*)busy->next;
  }

//     printf ("Address not found in list\n");
}

/**
  * Print a heap list
  * @author shainer
  * @version 1.0
  * @param list List to be printed
  */
void print_heap_list (heap_node_t *list)
{
   int count=0;   
   while (list) {
     printf ("%d) Node_address: %d, Current->start_address: %d size: %d\n", count++, list,list->start_address, list->size);     
     list = (heap_node_t*)list->next;
   }
   printf ("\n");
}

/**
  * Free a memory info node.
  * @author finarfin
  * @version 1.0
  * @param node Node in free_list or used_list to be freed 
  */

void free_node(heap_node_t* toadd){    
    toadd->start_address = NULL;
    toadd->size = 0;    
    toadd->next = kheap->free_nodes;
    kheap->free_nodes = toadd;        
}
