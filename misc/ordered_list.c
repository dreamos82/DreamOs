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

/** 
  * Insert a new element in the list
  * @author shainer
  * @version 0.2
  * @param new_node New element
  * @param list Destination list
  * @return none
  **/
void insert_list(heap_node_t *new_node, heap_node_t **ulist)
{
  /* First node */
  if(!(*ulist)) {
    (*ulist)=new_node;
  } else {
    heap_node_t *swap = *ulist;
    heap_node_t *previous = swap;

    /* Sorting by start address */
    while (swap->start_address < new_node->start_address) {
      previous = swap;
      swap = (heap_node_t*)swap->next;
      if (!swap) break;
    }

    if (previous == swap) { /* new_node is to become the new first one */
      *ulist = new_node;
      new_node->next = swap;
    } else { /* new_node is in the middle of the list */
      previous->next = new_node;
      new_node->next = swap;
    }             
  }
}

