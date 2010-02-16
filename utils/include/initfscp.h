//      initfscp.h
//      
//      Copyright 2009  Ivan Gualandri 
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#ifndef _INITFS_CP_
#define _INITFS_CP_

#define FILENAME_LENGTH 64
#define MAX_FILES 32
#define INITFSCP_VER "0.2.1"

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_MOUNTPOINT 0x03

struct initrd_file_t{
	int magic;
	char fileName[FILENAME_LENGTH];
	short int file_type;
	int  uid;
	unsigned int offset;
	unsigned int length;
};

typedef struct initrd_file_t initrd_file_t;
void usage(char *);
void version(char *);
#endif
