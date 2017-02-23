/*
 * Dreamos
 * stat.h
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

#ifndef _STAT_H
#define _STAT_H

#include <sys/types.h>

/*!  \struct struct stat
     \brief Struttura dati che contine le informazioni su di un file
 */
struct stat
{
    dev_t st_dev; /**< device id del file*/

    uid_t st_uid; /**< user id del file*/
    gid_t st_gid; /**< group id del file*/

    off_t st_size; /**< Dimensione del file */
};


int stat(const char *, struct stat *);

#endif
