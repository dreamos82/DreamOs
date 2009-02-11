/*
 * Dreamos
 * paging.h
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
 /*
  * Autore Ivan Gualandri
  * Prima versione: 29/10/2007
  * Contiene le definizioni di alcuni tipi di dato :D
  */

#ifndef PAGING_H
#define PAGING_H

#define PD_LIMIT 0x400
#define PT_LIMIT 0x400

#define PD_PRESENT 0x01 

#define WRITE 0x02
#define READ 0

#define SUPERVISOR 0
#define P_USER 0x04

#define WT_ENABLE 0x08

#define CACHE_DISABLE 0x10
#define ACCESSED 0x20

#define DIRTY 0x01

#define PF_MASK 0x03

typedef unsigned int pde_t; /**< Tipo dati per indicare una page dir entry */
typedef unsigned int pte_t; /**< Tipo dati per indicare una page table entry */

/*!  \struct pdbr_t
     \brief Struttura dati che contiene le informazioni sulla page dir, da caricare nel pdbr
 */
typedef struct pg_dir {
    unsigned int base_pd:20; /**< Indirizzo dove inizia la page dir (I primi 20 bit)*/
    unsigned int reserved_hi:7;
    unsigned int pcd:1;
    unsigned int pwt:1;
    unsigned int reserved_low:3;
}__attribute__((packed)) pdbr_t;

extern unsigned int *current_page_dir;
extern unsigned int *current_page_table;
void init_paging();
unsigned int* create_pageDir();
unsigned int* create_pageTable();

void set_pagedir_entry(int , unsigned int, unsigned char, unsigned char);
void set_pagetable_entry(int , unsigned int, unsigned char, unsigned char);
void set_pagedir_entry_ric(int , unsigned int, unsigned char, unsigned char);
void set_pagetable_entry_ric(int, int , unsigned int, unsigned char, unsigned char);

unsigned int get_pagedir_entry(int);
unsigned int get_pagetable_entry(int,int);

void load_pdbr(unsigned int);
void map_kernel(pde_t);
void page_fault_handler(int);

#endif
