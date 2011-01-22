/***************************************************************************
 *            ordered_array.c
 *
 *  Sun 18 07 08 07:47:17 2007
 *  Copyright  2008  Ivan Gualandri
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

#include <ordered_array.h>
#include <kheap.h>
#include <stdio.h>
#include <heap.h>
#include <string.h>


short int standard_lessthan (type_t a, type_t b){
	if(a<b) return 1;
	else return 0;
	return 0;
}

/**
  * Create a new  ordered_array
  * @author Ivan Gualandri
  * @version 1.0
  * @param start Start address of our new array 
  * @param size Max size of array
  * @return Pointer to a new ordered_array_t
  */
ordered_array_t new_array(void* start, unsigned int size, lessthan_predicate_t predicate){
    ordered_array_t tmp_array;
    int i=0;
    tmp_array.array = (type_t*)start;    
    tmp_array.size_max = size;
    tmp_array.size = 0;	    
    tmp_array.less_than = predicate;    
    printf("SizeMax: %d\n", tmp_array.size_max);
	while(i < tmp_array.size_max){
		tmp_array.array = 0x0;
		i++;
	}
    //memset(tmp_array.array, 0, tmp_array.size_max);
    return tmp_array;
}

/**
  * Insert an element into ordered_array
  * @author Ivan Gualandri
  * @version 1.0
  * @param elem Element to be inserted
  * @param array Array where we insert our new element
  * @return void
  */
void insert_array(type_t elem, ordered_array_t* t_array){
    int i;
    i=0;
    //Qui va un ciclo tipo: che termina o quando l'elemento va inserito in coda, o quando trova
    //la sua posizione dove mettersi. && t_array->t_array[i].size>=elem->size Questa e' l'idea, ma va 
    //sistemato il codice.
    while(i<t_array->size && header_t_less_than(t_array->array[i], elem)){				
        i++;
	}
    if(i==t_array->size){
		/*header_t* hel;
		printf("i = t_arraysize\n");*/
		t_array->array[t_array->size] = elem;
		t_array->size++;		
		//hel = (header_t*) t_array->array[0];
		//printf("array: %u\n",t_array->array[0]);				
	}
    else {
		//printf("insert in the middle\n");
		type_t tmp = t_array->array[i];
		t_array->array[i] = elem;
		while(i < t_array->size){
			i++;
			type_t tmp2 = t_array->array[i];			
			t_array->array[i] = tmp;
			tmp = tmp2;			
		}
		t_array->size++;
	}    
    //print_array(t_array);
}
/**
  * Get an element from ordered_array
  * @author Ivan Gualandri
  * @version 1.0
  * @param index index of element we want to read
  * @param array Array where we read requested element
  * @return type_t Element required if available, otherwise PANIC.
  */
type_t get_array(int index, ordered_array_t* t_array){
	if(index < t_array->size){
		//printf("Value: %x ", (type_t*)t_array->array[index]);
		return t_array->array[index];
	}
	else {		
		printf("get_array: PANIC");
		while(1);
	}
}

/**
  * Remove an element from ordered_array
  * @author Ivan Gualandri
  * @version 1.0
  * @param index index of element we want to read
  * @param array Array where we remove requested element
  * @return void
  */
void remove_array(int index, ordered_array_t* t_array){	
	while(index < t_array->size){
		t_array->array[index] = t_array->array[index+1];
		index++;
	}
	t_array->size--;
}

void print_array(ordered_array_t* array){
	int i=0;	
	while(i<array->size){
		//printf("Val: %d: %x\n", i, array->array[i]);
		i++;
	}
}
