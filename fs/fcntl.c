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
#include <kheap.h>
#include <shell.h>

int cur_fd;
file_descriptor_t fd_list[_SC_OPEN_MAX];
struct mountpoint_t mountpoint_list[MAX_MOUNTPOINT];
userenv_t current_user;

/**
  * @author Ivan Gualandri
  * @param char* path percorso del file da aprire
  * @param int oflags parametri di apertura
  *
  * Dato un path viene aperto se presente, e si torna il numero di descrittore che lo contiene
  * @todo Inserire gestione flags
  */
int open(const char *path, int oflags,  ...){
	int prova;
	int mpid;
	int ret_fd;
	va_list ap;
	va_start(ap, oflags);
	ret_fd = 0;	
	while(ret_fd < _SC_OPEN_MAX){
		//if(fd_list[ret_fd].mountpoint_id == -1) printf("%d ", ret_fd);		
		ret_fd++;
	}
	prova = va_arg(ap, int);
	if(cur_fd == _SC_OPEN_MAX) cur_fd = 0;
		//printf("Cur_fd reset\n");
		//printf("No more file descriptors available\n");
		//return -1;
	while(fd_list[cur_fd].mountpoint_id != -1 && cur_fd < _SC_OPEN_MAX){
		cur_fd++;
	}
	if(cur_fd == _SC_OPEN_MAX) {
		printf("No more file descriptors available\n");
		return -1;
	}
	path=get_abs_path((char*) path);
    mpid = get_mountpoint_id((char*) path);		
	//printf("Cur_fd: %d\n",cur_fd);
	if(mpid >-1) {
		fd_list[cur_fd].mountpoint_id = mpid;				
		path = get_rel_path(mpid, path);		
	} else {
		printf("That path doesn't exist\n");
		va_end(ap);
		return -1;
	}
	if( mpid > -1 && mountpoint_list[fd_list[cur_fd].mountpoint_id].operations.open != NULL){
			fd_list[cur_fd].fs_spec_id = (int) mountpoint_list[fd_list[cur_fd].mountpoint_id].operations.open(path, oflags);
		if(fd_list[cur_fd].fs_spec_id == -1){
			printf("No file's Found\n");
			va_end(ap);
			return -1;
		}
		//printf("%d---%d---", cur_fd, fd_list[cur_fd].fs_spec_id);
			//printf("Mpoint id: %d %s fs_spec_fd: %d\n", fd_list[cur_fd].mountpoint_id, path, fd_list[cur_fd].fs_spec_id);			
	}
	else {
		if(mpid>-1) printf("No OPEN services found here\n");					
		//fd_list[cur_fd].mountpoint_id = -1; //Controllare se va messa
		va_end(ap);
		return -1;
	}
	va_end(ap)	
	fd_list[cur_fd].offset = 0;
	ret_fd = cur_fd;
	cur_fd++;
	free((void*)path);
	return ret_fd;
}

