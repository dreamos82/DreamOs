/*
 * Dreamos
 * gdt.h
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
 /*
  * Autore Ivan Gualandri
  * Prima versione: 27/10/2003
  * Contiene le definizioni di alcuni tipi di dato :D
  */
#ifndef _GDT_H
#define _GDT_H
#include <stddef.h>

#define GDT_SIZE 8192

//For opt_2 argument
#define PRESENT 0x80
//#define GRANULARITY 0x40

//For opt_1 argument
#define GRANULARITY 0x80
#define NOT_PRESENT 0x00
#define KERNEL 0x00
#define RING1 0x01
#define RING2 0x02
#define USER 0x03
#define CODE 0x10
#define MEMORY 0x00
#define DATA 0x10

//For Data Type Option
#define R_ONLY 0x00
#define R_ONLY_A 0x01
#define R_WRITE 0x02
#define R_WRITE_A 0x03

/*!  \struct gdt_desc
     \brief Struttura dati che rappresenta un descrittore della GDT                             
 */
typedef struct gdt_desc {
	unsigned short int segment_limit_low; /**<Primi 16 bit del limite (20 bit in totale)*/
	unsigned short int segment_base_low; /**<Primi 2 byte della base del segmento*/
	unsigned char base_mid; /**<base del segmento*/
	unsigned char options_1; /**<Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit)*/
	unsigned char options_2; /**<SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)*/
	unsigned char base_high; /**<Ultimi 8 bit del base address*/
} __attribute__((packed)) GDT_Descriptor;

/*!  \struct gdt_r
     \brief Struttura dati che serve a caricare la IDT nel IDTR
 */
typedef struct gdt_r {
	unsigned short int gdt_limit;/**< la dimensione della GDT (in numero di entry)*/
	unsigned int gdt_base;/**< l'indirizzo iniziale della IDT*/
} __attribute__((packed)) GDT_Register;

void set_gdtr(GDT_Descriptor *, unsigned short int, int, int);
void add_GDTseg(int, unsigned int, unsigned int,unsigned char, unsigned char);
void init_gdt();

#endif
