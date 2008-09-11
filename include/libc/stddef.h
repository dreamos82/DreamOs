/*
 * Dreamos
 * stddef.h
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

#ifndef __STDDEF__H
#define __STDDEF__H

#define NULL 0

#define KERNEL_SIZE 0x200

typedef unsigned int size_t;
typedef unsigned long long qword;
typedef unsigned short int ushort;
typedef unsigned char byte;

#endif
