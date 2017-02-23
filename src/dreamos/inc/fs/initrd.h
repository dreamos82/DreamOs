/*
 * Dreamos
 * initrd.h
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

#ifndef INITRD_H
#define INITRD_H

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILENAME_LENGTH 64
#define MAX_FILES 32
#define MAX_INITRD_DESCRIPTORS _SC_OPEN_MAX

/*! \struct initrd_t
    \brief Contiene il numero dei files contenuti nel filesystem initrd.
 */
struct initrd_t
{
    int nfiles; /*!< Numero Files letti*/
};

/*! \struct initrd_file_t 
    \brief Contiene le informazioni relative ai singoli files contenuti in initrd
 */
struct initrd_file_t
{
    int magic; /*!< Numero usato come delimitatore da settare a 0xBF*/
    char fileName[FILENAME_LENGTH]; /*!< Nome del file*/
    short int file_type;
    int uid; /*!< User id del proprietario del file */
    unsigned int offset; /*!< indirizzo relativo di partenza */
    unsigned int length; /*!< Dimensione del File */
};

/*! \struct initrd_fd 
    \brief File Descriptor relativo ai files aperti.
 */
struct initrd_fd
{
    int file_descriptor; /*!< id del file aperto all'interno del file system (posizione nel vettore dei files */
    int cur_pos; /*!< Posizione attuale all'interno del file, per operazioni di lettura/scrittura*/
};

typedef struct initrd_t initrd_t;
typedef struct initrd_file_t initrd_file_t;
typedef struct initrd_fd initrd_fd;

extern initrd_t * fs_specs;
extern initrd_file_t * fs_headers;
extern unsigned int fs_end;

void dummy();

int initfs_init();

DIR * initfs_opendir(const char *);

struct dirent * initrd_readdir(DIR *);

int initfs_open(const char *, int, ...);

ssize_t initfs_read(int, char *, size_t);

int initrd_stat(char *, struct stat *);

ssize_t initrd_write(int, const void *, size_t);

int initrd_close(int);

#endif
