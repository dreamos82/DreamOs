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

#define FILENAME_LENGTH 64
#define MAX_FILES 32

struct initrd_t{
	int nfiles;
};

struct initrd_file_t{
	int magic;
	char fileName[FILENAME_LENGTH];
	int  uid;
	unsigned int offset;
	unsigned int length;
};

typedef struct initrd_t initrd_t;

extern initrd_t fs_specs;
void dummy();
int initfs_init();
DIR *initfs_opendir(const char *);
int initfs_open(const char *, int, ...);
ssize_t initfs_read(int, void *, size_t);
#endif
