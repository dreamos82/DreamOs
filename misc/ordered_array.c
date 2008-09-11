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

/**
  * Crea un nuovo ordered_array
  * @author Ivan Gualandri
  * @version 1.0
  * @param start Indirizzo di inizio del nostro array 
  * @param size grandezza massima dell'array
  * @return Pointer to a new ordered_array_t
  */
ordered_array_t new_array(void_t address,unsigned int start, unsigned int size){
    int i;
    i=0;
    printf("Make array code goes here...\n");        
    ordered_array_t tmp_array;
    tmp_array.size_max = size;
    tmp_array.size = 0;
    tmp_array.array = (void_t)start;    
    /* Questa parte e' memsettbile dopo che sara' fatta */
    while(i<size){
        tmp_array.array[i]= (void_t) 0x0;
        i++;
    }
    return tmp_array;
}

void insert_array(void_t elem, ordered_array_t* array){
    int i;
    i=0;
    //Qui va un ciclo tipo: che termina o quando l'elemento va inserito in coda, o quando trova
    //la sua posizione dove mettersi. && array->array[i].size>=elem->size Questa e' l'idea, ma va 
    //sistemato il codice.
    while(i<array->size)
        i++;
    if(i==array->size) array->array[++i] = elem;
}
