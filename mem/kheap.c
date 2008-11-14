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
heap_t *kheap;
unsigned int address_cur = (unsigned int) &end;
unsigned int node_address;

void* kmalloc(unsigned int size)
{
    unsigned int temp;
    
    temp = address_cur;
    address_cur+=size;
    return (void *) temp;
}

/* Test procedure ("try_heap" shell command") */
void try_alloc()
{
   
    printf("try_alloc(): Used list address: %d, Value: %d\n", &(kheap->used_list), kheap->used_list);
    alloc(5000, kheap);
    alloc(60, kheap);
    alloc(50, kheap);
    alloc(100, kheap);

    print_heap_list (kheap->used_list);
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
heap_t* make_heap(unsigned int start, unsigned int end, unsigned int size)
{
    heap_t* new_heap;
    heap_node_t* first_node;

    new_heap = 0xC0000000;
    node_address = 0xC0000000 + sizeof(heap_t);
//     new_heap = (heap_t*)kmalloc(sizeof(heap_t));

    first_node = (heap_node_t*)alloc_node();
    first_node->start_address = (unsigned int)&end;
    first_node->size = size;
    first_node->next = NULL;

    new_heap->max_size = tot_mem-(unsigned int) &end;
    new_heap->free_list = first_node;
    new_heap->used_list = NULL;

    printf("First heap created...\n");   
    printf("Size: %d - Tot mem: %d - Start address: %x\n", (new_heap->free_list)->size, tot_mem, new_heap);    
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
    int n_pages = size / 4096;
    int aval_pages;
    heap_node_t* new_node = NULL;
    heap_node_t* free_heap_list = cur_heap->free_list;    
    heap_node_t* prev_node;
    if(size%4096 !=0)
      n_pages++;
    printf("----\n");
    printf("Number of pages: %d ", n_pages);
    
    prev_node = free_heap_list;

    /* Look for a free block of memory in the heap's free memory list */    
    while(free_heap_list) {
      if(free_heap_list->size >= size) {
        aval_pages = free_heap_list->size/4096;
        if(free_heap_list->size%4096 !=0)
            aval_pages++;
        printf("Available_pages: %d\n", aval_pages);
        if(aval_pages > n_pages){
            printf("Node should be splitted\n");
            new_node = (heap_node_t*)alloc_node();
            new_node->start_address = free_heap_list->start_address;
            new_node->next = NULL;
            new_node->size = n_pages*0x1000;
            insert_list (new_node, &(cur_heap->used_list));
            free_heap_list->size = (free_heap_list->size) - (n_pages*0x1000);
            free_heap_list->start_address = free_heap_list->start_address + (n_pages*0x1000);        
        }
        else if(aval_pages == n_pages){
            insert_list (free_heap_list, &(cur_heap->used_list));
            prev_node->next = free_heap_list->next;
            printf("\n");    
        }

        printf("New_node -> Size: %d, start_address: %d\n", new_node->size, new_node->start_address);
        printf("free_heap_list -> Actual size: %d, start_address: %d\n", free_heap_list->size, free_heap_list->start_address);
        printf("----\n");
        
        break;
      }
      else {
        prev_node = free_heap_list;
        free_heap_list = free_heap_list->next;
        }      
   }
   printf("Prev_node: %d \n", prev_node->start_address);
   return (void *)new_node;
}

/**
  * Allocation of a new node for kheap
  * @author Ivan Gualandri
  * @version 1.0
  * @return The start address of the new allocated node
  */
heap_node_t* alloc_node(){
    unsigned int temp;
    temp = node_address;
    node_address = node_address + sizeof(heap_node_t);
    return (heap_node_t*) temp;
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
  heap_node_t *free = kheap->free_list;
  heap_node_t *n2;

  if ((unsigned int)location % 4 != 0)
    printf ("Indirizzo non allineato a 4kb\n");

  while (busy) {
    if (busy->start_address == location) {
      if (prev == busy) { /* node is the first */
        kheap->used_list = busy->next;
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
        n2 = n2->next;
      }
      n2 = busy->next;

      printf ("--Pre merge--\n");
      printf ("Prev address: %d, size: %d\n", prev->start_address, prev->size);
      printf ("Busy address: %d, size: %d\n", busy->start_address, busy->size);
      if (n2)
        printf ("N2 address: %d, size %d\n\n", n2->start_address, n2->size);

      /* Merge busy into prev */
      if (prev != busy && (prev->start_address + prev->size) == busy->start_address) {
        printf ("Backward merge\n");
        prev->size += busy->size;
        prev->next = n2;
      }
      /* Merge n2 into busy */
      if (n2 && (busy->start_address + busy->size) == n2->start_address) {
        printf ("Forward merge\n");
        busy->size += n2->size;
        busy->next = n2->next;
      }

      break;
    }
    prev = busy;
    busy = busy->next;
  }

  if (!busy)
    printf ("Address not found in list\n");

  printf("Navigating used list...\n");
  print_heap_list (kheap->used_list);
  printf("Navigating free list...\n");
  print_heap_list (kheap->free_list);
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
     printf ("%d) Current->start_address: %d\n", count++, list->start_address);
     list = list->next;
   }
   printf ("\n");
}

