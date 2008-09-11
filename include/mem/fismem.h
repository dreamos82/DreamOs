/***************************************************************************
 *            fismem.h
 *
 *  Sun Apr  1 00:16:45 2007
 *  Copyright  2007  IvanGualandri
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
 
#ifndef _FISMEM_H
#define _FISMEM_H
#include <stddef.h>

#define BASE_MEM 0x100000

#define OFFSET_MEM 0x400   //L'offset e' di 1k

#define VALUE_MEM 0xAA55AA55  //Il primo valore che provo a scrivere
#define ANTI_VALUE_MEM ~0xAA55AA55 //Il secondo valore che provo a scrivere

#define BITMAP_SIZE 0x8000  //Number of Elements of Bitmap

#define ADD_LIST 1
#define NOT_ADD_LIST 0

typedef struct {
    unsigned int mem_map[BITMAP_SIZE];
}mem_struct;

/*! Contain start address and number of contiguos pages, useful
 for memory deallocation*/
typedef struct {
    void* inizio; /*!< start address of allocated space*/
    unsigned int num_pagine;/*!< Number of pages allocated*/
}mem_size;

/*! Contain info of allocated memory, for deallocation
 */
typedef struct {
    mem_size pages_info[510];/*!< @see mem_size */
    size_t next_free; /*!< next element of pages_info free*/
    struct mem_area* next; /*!< next element of list available free*/
}mem_area;

typedef mem_area *mem_area_pointer;
extern size_t tot_mem;
extern mem_struct mem_bitmap;

void init_mem();
void* request_pages(int,int);
int release_pages(void *);
size_t calcola_memoria();
size_t get_memsize();
size_t get_numpages();
size_t get_bmpelements();
int add_memarea_element(size_t, int);
void set_memorysize(unsigned int);

void *fis_malloc(const size_t);
void fis_free(void *);
#endif
