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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <kheap.h>
#include <paging.h>
#include <ordered_array.h>

extern unsigned int end;
heap_t *kheap = 0;
unsigned int address_cur = (unsigned int) &end;

void* kmalloc(unsigned int size){
    unsigned int temp;
    
    temp = address_cur;
    address_cur+=size;
    return (void *) temp;
}
/**
  * Crea un nuov heap
  * @author Ivan Gualandri
  * @version 1.0
  * @param start Indirizzo di inizio del nostro heap
  * @param end Indirizzo di fine dell'heap
  * @param size grandezza massima dell'heap
  * @return Pointer to a new heap
  */
heap_t* make_heap(unsigned int start, unsigned int end, unsigned int size){
    heap_t* new_heap;
    new_heap = (heap_t*)kmalloc(sizeof(heap_t));
    new_heap->hole_index = new_array(10,10,10); //decidere i valori
    new_heap->start_address = start;
    new_heap->end_address = end;
    new_heap->max_size = size;
    return (heap_t*) new_heap;
}