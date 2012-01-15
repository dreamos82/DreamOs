/*
 * Dreamos
 * fcntl.c
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
 
#include <vfs.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifdef LEGACY
#include <kheap.h>
#endif
#ifdef LATEST
#include <heap.h>
#endif
#include <shell.h>
#include <stdio.h>
#include <stdio.h>

int cur_fd;

/**
  * @author Ivan Gualandri
  * @param char* path percorso del file da aprire
  * @param int oflags parametri di apertura
  *
  * Dato un path viene aperto se presente, e si torna il numero di descrittore che lo contiene
  * @todo Inserire gestione flags
  */
int open(const char *path, int oflags,  ...){
	int mpid;
	int ret_fd;
	int error __attribute__((unused))= 0;
	char *newpath;
	va_list ap __attribute__((unused));
	va_start(ap, oflags);
	ret_fd = 0;		
	newpath = kmalloc(CURPATH_LEN * sizeof(char));
	memset(newpath, '\0', CURPATH_LEN);
	cur_fd=0;
	if(cur_fd == _SC_OPEN_MAX) cur_fd = 0;			

	while(fd_list[cur_fd].mountpoint_id != -1 && cur_fd < _SC_OPEN_MAX){
		//printf("%d %d\n", cur_fd, fd_list[cur_fd].mountpoint_id);		
		cur_fd++;
	}
	if(cur_fd == _SC_OPEN_MAX) {
		printf("No more file descriptors available\n");
		return -1;
	}
	strcpy(newpath, path);	
	//printf("Path: %s %s\n", path, newpath);	
	error = get_abs_path((char*) newpath);
	//printf("After get_abs: %s %s\n", newpath, current_user.cur_path);	
   	 mpid = get_mountpoint_id((char*) newpath);		
	//printf("Cur_fd: %d\n",cur_fd);
	if(mpid >-1) {
		fd_list[cur_fd].mountpoint_id = mpid;				
		newpath = get_rel_path(mpid, (char *)newpath);		
	} else {
		printf("That path doesn't exist\n");
		va_end(ap);
		return -1;
	}
	if( mpid > -1 && mountpoint_list[fd_list[cur_fd].mountpoint_id].operations.open != NULL){
			fd_list[cur_fd].fs_spec_id = (int) mountpoint_list[fd_list[cur_fd].mountpoint_id].operations.open(newpath, oflags);
		if(fd_list[cur_fd].fs_spec_id == -1){
			printf("No file's Found\n");
			va_end(ap);
			return -1;
		}
	}
	else {
		if(mpid>-1) printf("No OPEN services found here\n");					
		va_end(ap);
		return -1;
	}
	va_end(ap)	
	fd_list[cur_fd].offset = 0;
	fd_list[cur_fd].flags_mask = oflags;
	ret_fd = cur_fd;
	cur_fd++;
	free(newpath);
	return ret_fd;
}

