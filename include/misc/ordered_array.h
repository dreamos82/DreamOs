/***************************************************************************
 *            ordered_array.h
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

#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <ordered_array.h>

typedef void* void_t;

/*!  \struct ordered_array_t
     \brief Struttura dati che un vettore ordinato.
 */
typedef struct {
    void_t *array; /**< Il vettore che sara' ordinato*/
    unsigned int size; /**< La dimensione attuale*/
    unsigned int size_max; /**< La dimensione massima*/
}ordered_array_t;

ordered_array_t new_array(void_t, unsigned int, unsigned int);
void insert_array(void_t, ordered_array_t*);

#endif
