/***************************************************************************
 *            buddy.c
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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <buddy.h>
#include <paging.h>
#include <kheap.h>
#include <stdio.h>
#include <stddef.h>

extern unsigned int end;
extern unsigned int address_cur;
buddy_t* kbuddy;
buddy_t* new_buddy(){
    buddy_t* temp;
    temp = (buddy_t*) kmalloc(sizeof(buddy_t));
    printf("end: %x | address_cur: %x\n", &end, address_cur);
    temp->start_address = address_cur;
    temp->size = 65536; //da cambiare
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}