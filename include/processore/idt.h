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
#ifndef _IDT_H
#define _IDT_H 

/*! \def IDT_SIZE
    \brief Dimensione della IDT = 256
*/

#define IDT_SIZE 256

/*!  \struct idt_desc
     \brief Struttura dati che rappresenta un descrittore della IDT               
 */
typedef struct idt_desc {
    unsigned short int offset_low;/**< il puntatore all'istruzione*/
    unsigned short int seg_selector;/**< seleziona il segmento codice dalla GDT (nel nostro caso primo quindi 0x8, si specifica l'offset)*/
    unsigned char null_par;/**< e' un campo che deve essere sempre impostato a 0.*/
    unsigned char options;/**< Le opzioni del selettore dela IDT*/
    unsigned short int offset_high;/**< il puntatore all'istruzione*/
} __attribute__((packed)) IDT_Descriptor;/**< */

/*!  \struct idt_r
     \brief Struttura dati che serve a caricare la IDT nel IDTR
 */
typedef struct idt_r{
    unsigned short int idt_limit;/**< la dimensione della IDT (in numero di entry)*/
    unsigned int idt_base;/**< l'indirizzo iniziale della IDT*/
} __attribute__((packed)) IDT_Register;

void init_idt();

void add_IDTseg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel);
void set_idtr(IDT_Descriptor *, unsigned short int);

void INT_0(); /**< Eccezione numero 0 - #DE Divide Error*/
void INT_1(); /**< Eccezione numero 1 - #DB Debug*/
void INT_2(); /**< Eccezione numero 2 - Non Mascable Interrupt*/
void INT_3(); /**< Eccezione numero 3 - #BP Breakpoint*/
void INT_4(); /**< Eccezione numero 4 - #OF Overflow*/
void INT_5(); /**< Eccezione numero 5 - #BR Bound Range Exception*/
void INT_6(); /**< Eccezione numero 6 - #UD Invalid OpCode Exception*/
void INT_7(); /**< Eccezione numero 7 - #NM Device Not Available*/
void INT_8(); /**< Eccezione numero 8 - #DF Double Fault*/
void INT_9(); /**< Eccezione numero 9 - Coprocessor Segment Overrun*/
void INT_10(); /**< Eccezione numero 10 - #TS Invalid TSS*/
void INT_11(); /**< Eccezione numero 11 - #NP Segment Not Present*/
void INT_12(); /**< Eccezione numero 12 - #SS Stack Segment Fault*/
void INT_13(); /**< Eccezione numero 13 - #GP General Protection*/
void INT_14(); /**< Eccezione numero 14 - #PF Page Fault*/
void INT_15(); /**< Eccezione numero 15 - #XX Reserverd*/
void INT_16(); /**< Eccezione numero 16 - #MF Floating Point */
void INT_17(); /**< Eccezione numero 17 - #AC Alignment Check*/
void INT_18(); /**< Eccezione numero 18 - #MC Machine Check*/
void INT_19(); /**< Eccezione numero 19 - #XF Streaming SIMD Exception*/
void INT_32();
void INT_33();
void INT_34();
void INT_35();
void INT_36();
void INT_37();
void INT_38();
void INT_39();
void INT_40();
void INT_41();
void INT_42();
void INT_43();
void INT_44();
void INT_45();
void INT_46();
void INT_47();
void INT_48();
void INT_80();
#endif
