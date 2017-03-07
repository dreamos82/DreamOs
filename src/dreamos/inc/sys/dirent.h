/*
 * Dreamos
 * dirent.h
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

#ifndef _DIRENT_H
#define _DIRENT_H

#include <stddef.h>

#define NAME_MAX 30

/*! 
    \struct dirent
    \brief Struttura dati che contiene le entry della cartella.
*/
struct dirent
{
    ino_t d_ino;
    int d_type;
    char d_name[NAME_MAX + 1];
};

/*! 
    \struct DIR
    \brief Struttura dati che contiene le informazioni su una cartella
*/
typedef struct
{
    int handle; /**< Filesystem directory handle*/
    ino_t cur_entry;
    char path[NAME_MAX + 1]; /**< dir path*/
    struct dirent entry; /**< Next directory item*/
} DIR;


int closedir(DIR *);

DIR * opendir(const char *);

struct dirent * readdir(DIR *);

int readdir_r(DIR *, struct dirent *, struct dirent **);

void rewinddir(DIR *);

void seekdir(DIR *, long int);

long int telldir(DIR *);

DIR * fake_opendir(const char *);

#endif
