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

int cur_fd;
file_descriptor_t fd_list[_SC_OPEN_MAX];
struct mountpoint_t mountpoint_list[MAX_MOUNTPOINT];

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
	va_list ap;
	va_start(ap, oflags);
	
	prova = va_arg(ap, int);
	//printf("Magic value: %d e il path: %s e cur_fd: %d\n", prova,path,cur_fd);
	if(cur_fd >= _SC_OPEN_MAX) cur_fd=0;
	else {		
		mpid = get_mountpoint_id(path);
		fd_list[cur_fd].mountpoint_id = mpid;				
		path = get_rel_path(mpid, path);		
		if(mountpoint_list[fd_list[cur_fd].mountpoint_id].operations.open != NULL){
			fd_list[cur_fd].fs_spec_id = (int) mountpoint_list[fd_list[cur_fd].mountpoint_id].operations.open(path, oflags);
			//printf("Mpoint id: %d %s fs_spec_fd: %d\n", fd_list[cur_fd].mountpoint_id, path, fd_list[cur_fd].fs_spec_id);			
		}
		else {
			printf("No OPEN services found here\n");		
			va_end(ap);
			return -1;
		}
	}	
	va_end(ap)
	return cur_fd++;
}

