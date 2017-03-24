/***************************************************************************
 *            fismem.c
 *
 *  Sun Apr  1 00:16:03 2007
 *  Copyright  2007  Ivan Gualandri
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

#include <fismem.h>
#include <video.h>
#include <stdio.h>
#include <stddef.h>
#include <pic8259.h>
#include <debug.h>

size_t tot_mem;
size_t num_pages=0;
size_t bmp_elements;

mem_struct mem_bitmap;


mem_area mem_info; 
mem_area_pointer mem_info_root = (mem_area_pointer)&mem_info;

void init_mem(){
    int i=0;
//     int remainder=0;    
    while(i<BITMAP_SIZE){
        if(i<32)mem_bitmap.mem_map[i] = 0xFFFFFFFF;
        else mem_bitmap.mem_map[i] = 0x00000000;
        i++;
    }
	//mem_bitmap.mem_map[0xf7f0] = 0x00000001;
	//mem_bitmap.mem_map[0x7f60] = 0x00000001;
    i=0;    
    bmp_elements = (tot_mem/4096)/32;    
    //     printf("bmp_elements: %d\n", bmp_elements);
    //Ho allocato il primo elemento di mem_info, spazio dedicato al kernel
    mem_info.pages_info[0].inizio= 0x00000000;
    mem_info.pages_info[0].num_pagine = 512;
    i++;
    mem_info.next_free = 1;
    mem_info.next = NULL;
    while(i < MEMINFO_SIZE) {
        mem_info.pages_info[i].inizio= 0x00000000;
        mem_info.pages_info[i].num_pagine = 0;
        i++;
    }
}

/**
  * @author Ivan Gualandri
  * @version 1.0
  * @param int n_page number of pages required
  * @param int flags ADD_LIST if we want to add current allocations to list for deallocations NOT_ADD_LIST otherwise
  */
void* request_pages(int n_pages, int flags){
    //L'indirizzo preso in input sara' allineato a 4k.
    int n_free, sh;
	size_t indirizzo;
    size_t i=16;
    sh=0;
    n_free=0;
    while(n_free<n_pages){
        if((mem_bitmap.mem_map[i] & 1<<sh)) n_free=0;
        else n_free++;
        sh++;
        if(sh==32 && i<bmp_elements && n_free<n_pages){
            sh=0;
            i++;
        }
		if(i==bmp_elements && i==31) return NULL;	
    }
	indirizzo = ((i*32+sh)-n_free)*4096;
    //Stampe debug - Da eliminare
    #ifdef DEBUG
    dbg_print ("Numero pagine che mi servono: %d\n", n_pages);
    dbg_print ("Sono alla posizione %d\n", sh);
    dbg_print ("Dell'elemento: %d\n", i);
    dbg_print ("Ho ottenuto l'indirizzo: %d\n", (int)indirizzo);
    #endif
    i = (indirizzo/4096)/32;
    sh = (indirizzo/4096)%32;
    n_free=0;
    while(n_free<n_pages){
        mem_bitmap.mem_map[i] = mem_bitmap.mem_map[i] | (1<<sh);
        sh++;
        n_free++;
        if(sh==32 && n_free < n_pages){
            sh=0;
            i++;
        }
    }
    /*Devo aggiungere l'elemento nella lista.*/
    #ifdef DEBUG
    dbg_print ("Valore: %d\n", sh);
    dbg_print (" Chiamo add_memarea_element\n");
    #endif
    if(flags==ADD_LIST)
     add_memarea_element(indirizzo, n_pages);
    #ifdef DEBUG
    dbg_print("Next_free: %d\n", mem_info.next_free);
    dbg_print("%u\n", indirizzo);
    #endif    
    return (void*) indirizzo;
}

int release_pages(void *addr){
    if((int)addr%4096==0){
        //Inizializzazioni
        mem_area_pointer tmp_list;
        int riga, colonna;
        int indirizzo_int, n_pagine;
        int i;
        i=0;
        riga=0;
        colonna=0;        
        n_pagine =0;
        tmp_list = mem_info_root;
        riga = ((int)addr/4096)/32;
        colonna =((int)addr/4096)%32;
        indirizzo_int = (int)addr;
        //Cerca elemento dentro strutturina
        while(i<510){         
            if(indirizzo_int == (int) tmp_list->pages_info[i].inizio  && indirizzo_int>0){
                n_pagine = tmp_list->pages_info[i].num_pagine;                 
                tmp_list->pages_info[i] = tmp_list->pages_info[tmp_list->next_free-1];
                tmp_list->next_free--;                
                break;
            }
            if(i==509 && tmp_list->next!=NULL){
                  tmp_list =(mem_area_pointer) tmp_list->next;
                  i=0;
            }           
            i++;
        }
        //Dopo che ho trovato il numero di pagine occupate, lo elimino        
        i=0;        
        if(n_pagine!=0){
            dbg_print("Valore mem_map: %d ", mem_bitmap.mem_map[riga]);
            while(i<n_pagine){           
                mem_bitmap.mem_map[riga] = mem_bitmap.mem_map[riga]^(1<<colonna);
                if(colonna<32 && i<n_pagine){                
                    colonna++;
                }
                else if(i<n_pagine){
                    riga++;
                    colonna = 0;
                }
                i++;
            }
            dbg_print("dopo pulitura: %d ", mem_bitmap.mem_map[riga]);
            dbg_print("Pagina rilasciata - Prossima pos libera: %d", tmp_list->next_free);
            dbg_print("Ho liberato: %d pagine\n", n_pagine);
        }        
    } else {
        dbg_print("Error, address is: %d\n", addr);
        return -1;
    }
    return 0;
}

int add_memarea_element(size_t start_address, int required_pages){
//Return -1 on error 0 on success
    int posizione;
    posizione = mem_info.next_free;
    if(posizione>0 && posizione<510){
        #ifdef DEBUG
        dbg_print("Posizione <510\n");
        #endif
        mem_info.pages_info[posizione].inizio = (void*)start_address;
        mem_info.pages_info[posizione].num_pagine = required_pages;
        mem_info.next_free++;
        return 0;
    }
     else {
       mem_info.next = request_pages(1, NOT_ADD_LIST);
       mem_area_pointer mem_new = (mem_area_pointer) mem_info.next;
        if(mem_new->next!=NULL){
            mem_new = (mem_area_pointer)mem_info.next;
            #ifdef DEBUG
            dbg_print("Posizione >510\n");
            #endif
            mem_new->pages_info[0].inizio = (void*)start_address;
            mem_new->pages_info[0].num_pagine = required_pages;
            mem_new->next_free = 1;
        }
        else return -1;
    return 0;
    }
    return -1;
}

/**
  * Calcola la quantita' di memoria
  * @author Ivan Gualandri
  * @version 1.0
  * @return size_t Dimensione della memoria.
  * Questa funzione andra'completamente modificata appena si aggiungera il boot con grub.
  */

size_t calcola_memoria()
{
    return tot_mem;
}

/**
  * @author Ivan Gualandri
  * @version 1.0
  * @return Total amount of RAM installed on PC.
  */
size_t get_memsize(){
    return tot_mem;
}

/**
  * Calcola il numero di pagine effettivamente disponibili in ram
  * @author Ivan Gualandri
  * @version 1.0
  * @return size_t Numero pagine da 4k.
  */
size_t get_numpages(){
    num_pages = (tot_mem/1024)/4;
    return num_pages;
}

/**
  * Calcola il numero di elementi della bitmap utilizzabili dalla memoria
  * @author Ivan Gualandri
  * @version 1.0
  * @return size_t Numero di elementi. 
  */
size_t get_bmpelements(){
    bmp_elements = (tot_mem/4096)/32;
    return bmp_elements;
}

/*Da spostare appena termintao il gestore della memoria*/
void *fis_malloc(const size_t size){
    return request_pages(size % 4096 ? size / 4096 + 1 : size / 4096, ADD_LIST);
}

void fis_free(void *address){
    if(release_pages(address)!=0) dbg_print("Error\n");
}


/**
  * Configura una nuova dimensione per la memoria fisica, va chiamata prima di init_mem
  * @author Ivan Gualandri
  * @version 1.0
  * @param new_size La nuova dimensione per la memoria.
  * @return void. 
  */
void set_memorysize(unsigned int new_size){
    tot_mem = new_size;
}
