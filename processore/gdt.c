/*
 * Dreamos
 * gdt.c
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

#include <gdt.h>
#include <video.h>

GDT_Descriptor Gdt_Table[GDT_SIZE];
int current_pos;

void init_gdt(){
	int i;
	current_pos = 0;
	i=0;
	/*Pulisco il vettore della gdt*/
	while(i < GDT_SIZE) {
		Gdt_Table[i].segment_limit_low = 0;
		Gdt_Table[i].segment_base_low = 0; 
		Gdt_Table[i].base_mid = 0;
		Gdt_Table[i].options_1 = 0;
		Gdt_Table[i].options_2 = 0;
		Gdt_Table[i].base_high = 0;
		i++;
	}
	/*Le condizione minime per il funzionamento sono 3 descrittori: il primo nullo
	 *il secondo per l'area codice e il terzo per l'area dati.
	 *il descrittore nullo deve stare nella posizione 0
	 */
	add_GDTseg(0,0,0,0,0);
	add_GDTseg(1,0x00000000,0x000FFFFF, PRESENT|KERNEL|CODE|0x0A,GRANULARITY);
	add_GDTseg(2,0x00000000,0x000FFFFF, PRESENT|KERNEL|DATA|0x02,GRANULARITY);
	set_gdtr(Gdt_Table, 0xFFFFF, 1, 2);	
}
/**
  * @author Ivan Gualandri
  * @version 1.0
  * @param pos indica la posizione all'interno della IDT
  * @param base indirizzo di memoria da cui inizia il segmento che stiamo definendo
  * @param limit indirizzo di memoria che delimita il segmento
  * @param opt1 in ordine: Type (4bit) - S (1) bit -DPL (2 bit) - P(1 bit)
  * @param opt2 in ordine: SegLimit_hi(4 bit) AVL(1 bit) L(1 bit) D/B(1 bit) G(1bit)
*/
void add_GDTseg(int pos,unsigned int base, unsigned int limit, unsigned char opt1, unsigned char opt2){
	unsigned int tmpbase, tmplimit;
	tmpbase = base;
	tmplimit = limit;
	Gdt_Table[pos].segment_limit_low = limit & 0xFFFF;
	Gdt_Table[pos].segment_base_low = base & 0xFFFF; //Primi 32byte
	tmpbase = base >>16;
	Gdt_Table[pos].base_mid = tmpbase & 0xFF;
	Gdt_Table[pos].options_1= opt1;
	tmplimit= limit >> 16;
	tmplimit &=0xf;
	Gdt_Table[pos].options_2= GRANULARITY|0x40|tmplimit;
	tmpbase = base >>24;
	Gdt_Table[pos].base_high = tmpbase & 0xFF;
}

void set_gdtr(GDT_Descriptor *addr, unsigned short int limit, int code, int data){
	GDT_Register gdtreg;
	gdtreg.gdt_limit = limit;
	gdtreg.gdt_base = (unsigned long)addr;
	code = code<<3;
	data = data <<3;
	__asm__ __volatile__ ("lgdt %0\n"
				"pushl %1\n"
				"pushl $CSpoint\n"
				"lret\n"
				"CSpoint:\n"
				"mov %2,%%eax\n"
				"mov %%ax,%%ds\n"
				"mov %%ax,%%es\n"
				"mov %%ax,%%fs\n"
				"mov %%ax,%%gs\n"
				"mov %%ax,%%ss\n"
				: 
				: "g"  (gdtreg), "r" (code), "r" (data));
}
