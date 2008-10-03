/***************************************************************************
 *            ordered_list.c
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

#include <ordered_list.h>
#include <kheap.h>
#include <stdio.h>
#include <stddef.h>

void insert_list(heap_node_t* new_node, heap_node_t** list){
    if(*list==NULL){
        printf("\n---Insert new node---\n");
        *list=new_node;
        printf("Provo: %d---\n", (*list)->size);
    }
    else {
        printf("List->size= %d\n", (*list)->size);
        printf("Piano piano vado avanti\n");
    }
    printf("The current size: %d\n", new_node->size);
    printf("I will insert into the list\n");
}
