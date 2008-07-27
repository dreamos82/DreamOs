/***************************************************************************
 *            buddy.h
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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef BUDDY_H
#define BUDDY_H

/*!  \struct buddy_t
     \brief Struttura dati che mantiene le informazioni su un singolo buddy
 */
typedef struct {
    unsigned int start_address; /**< Indirizzo di inizio del buddy*/
    unsigned int size; /**< Dimensione della memoria da gestire*/
    struct buddy_t* left; /**< Figlio sinistro*/
    struct buddy_t* right; /**< Figlio destro*/
} buddy_t;

buddy_t* new_buddy();

#endif