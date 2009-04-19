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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
    temp->status = BUDDY_BUSY;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

unsigned int alloc_buddy(int size, buddy_t* tmp_buddy)
{
    buddy_t* cur_buddy;    
    int new_size;
    cur_buddy = tmp_buddy;
    if(size>1){
        new_size = cur_buddy->size/2;
        if(tmp_buddy->left==NULL && tmp_buddy->right==NULL){
            new_size = tmp_buddy->size/2;
            printf("Ok\n");             
            tmp_buddy->left = create_buddy(new_size);
            if(new_size/2>=size) alloc_buddy(size,cur_buddy->left);
            else tmp_buddy->status == BUDDY_BUSY;
        }
        else if(tmp_buddy->left!=NULL && tmp_buddy->status==BUDDY_FREE){            
            alloc_buddy(size, tmp_buddy->left);
        } else if(tmp_buddy->right!=NULL && tmp_buddy->status==BUDDY_FREE){
            alloc_buddy(size, tmp_buddy->right);
        }
        else{
            printf("Se sono nell'else allora devo proseguire nella navigazione\n");            
            alloc_buddy(size/2,tmp_buddy);
        }      
    }
//     else printf("Otherwise ok\n");
}

buddy_t* create_buddy(int size){    
    buddy_t* new_buddy;
    new_buddy = (buddy_t*) kmalloc(sizeof(buddy_t));
    new_buddy->status = BUDDY_FREE;
    new_buddy->left = NULL;    
    new_buddy->right = NULL;
    new_buddy->size = size;
    printf("Creating buddy at address: 0x%x with buddy size: %d\n", new_buddy, new_buddy->size);
    return new_buddy;
}
