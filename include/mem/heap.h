/***************************************************************************
 *            heap.h
 *
 *  Sun 8 08 10 
 *  Copyright  2010  Ivan Gualandri
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

#ifndef __HEAP_H
#define __HEAP_H

#define HEAP_START_ADDRESS 0xC0000000 /**Indirizzo iniziale*/
#define HEAP_START_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MIN_SIZE 0x70000
#define HEAP_MAGIC 0x12893457
#define PAGE_ALIGNED 1
#define HEAP_HOLE 1
#define HEAP_BLOCK 0

#include <ordered_array.h>

/*! \struct heap_t 
    \brief Contiene le informazioni relative all'heap corrente DA TERMINARE
 */
typedef struct {
	ordered_array_t index; /*!< Indice degli holes */
	unsigned int start_address; /*!< Indirizzo di inizio dell'heap */
	unsigned int end_address; /*!< Indirizzo di fine dell'heap */
	unsigned int max_address; /*!< Indirizzo massimo dove possiamo espandere l'heap */
}new_heap_t;

/*! \struct header_t 
    \brief Contiene le informazioni relative alle locazioni di memoria 
 */
typedef struct {
	unsigned int magic; /*!< For header identification*/	
	short int is_hole; /*!< 1 se si tratta di un hole 0 se si tratta di un block*/
	int size;  /*!< Total size of the block (IMPORTANT: including header and footer)*/
}__attribute__((packed)) header_t;

/*! \struct footer_t 
    \brief Contiene altre informazioni relative alle locazioni di memoria 
 */
typedef struct {
	unsigned int magic; /*!< For footer identification */
	header_t *header;/*!< Pointer to the block header*/
} footer_t;


new_heap_t *new_heap(unsigned int, unsigned int);

void *new_alloc(unsigned int, unsigned short int, new_heap_t*);
void expand(unsigned int, new_heap_t*);
void new_free();
void init_newmem();

unsigned int new_malloc(unsigned int);
unsigned int contract(unsigned int, new_heap_t*);
short int locate_smallest_hole(unsigned int, unsigned short int, new_heap_t*);
short int header_t_less_than(void *,void *);

void n_free(unsigned int);
#ifdef OLD_MEM_DISABLED
void* kmalloc(unsigned int);
void kfree(unsigned int);
#endif

#endif
