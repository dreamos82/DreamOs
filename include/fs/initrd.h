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

#define FILENAME_LENGTH 64

struct initrd_t{
	int nfiles;
};

struct initrd_file_t{
	char fileName[FILENAME_LENGTH];
	int  uid;
	unsigned int offset;
	unsigned int length;
};

void dummy();
DIR *dummy_opendir(const char *);
int dummy_open(const char *, int, ... );
#endif
