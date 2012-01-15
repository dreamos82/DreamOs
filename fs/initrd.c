/*
 * Dreamos
 * initrd.c
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

#include <stdio.h>
#include <initrd.h>
#include <multiboot.h>
#include <unistd.h>
#include <types.h>
#include <string.h>
#include <vfs.h>
#ifdef LEGACY
#include <kheap.h>
#endif
#ifdef LATEST
#include <heap.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <commands.h>

char *module_start;
initrd_t *fs_specs;
initrd_file_t* fs_headers;
initrd_fd ird_descriptors[MAX_INITRD_DESCRIPTORS];
unsigned int fs_end;
unsigned int cur_irdfd;

void dummy(){
	printf("Qui solo per una prova\n");
}	

int initfs_init(){	
	int i=0;
	fs_end = 0;
	fs_specs = (initrd_t *) module_start;
	fs_headers = (initrd_file_t *)(module_start + sizeof(initrd_t));
	while (i<MAX_INITRD_DESCRIPTORS) {
		ird_descriptors[i].file_descriptor = -1;
		ird_descriptors[i].cur_pos = 0;
		i++;
	}
	cur_irdfd = 0;
	//printf("Number of files present: %d\n", fs_specs->nfiles);
	fs_end = fs_headers[(fs_specs->nfiles)-1].offset + fs_headers[(fs_specs->nfiles)-1].length;
	printf("Fs_end: %d\n", fs_end);
	return fs_specs->nfiles;
	
}

DIR *initfs_opendir(const char *path){
	DIR* pdir = NULL;
	initrd_file_t *module_var;
	module_var = fs_headers;
	if(!strcmp(path, "/") || path[0]== '\0'){
		//printf("Directory found\n");
		pdir = kmalloc(sizeof(DIR));
		strcpy(pdir->path, path);
		pdir->handle = 0x01;
		pdir->cur_entry = 0x00;
		pdir->entry.d_ino = 0x00;
		strcpy(pdir->entry.d_name, module_var[0].fileName);
		//printf("First occorrency: %s\n", pdir->entry.d_name);
		return pdir;
	}
	else {
		printf("Sono una dummy_opendir, ti piaccio? path: %s\n", path);	
		return NULL;
	}
}

struct dirent *initrd_readdir(DIR *dirp){
	initrd_file_t *fs_type;	
	int nfiles;	
	nfiles = fs_specs->nfiles;
	//printf("%d nfiles\n", nfiles);
	if(dirp->cur_entry < nfiles){	
		fs_type = (initrd_file_t *)(module_start + sizeof(initrd_t));	
		dirp->entry.d_ino =  dirp->cur_entry;
		dirp->entry.d_type = fs_type[dirp->cur_entry].file_type;
		strcpy(dirp->entry.d_name, fs_type[dirp->cur_entry].fileName);	
		//printf("%s\n", dirp->entry.d_name);
		dirp->cur_entry++;
		//printf("Placeholder for future readdir of initrd Number of files: %d\n", dirp->cur_entry);
		return &(dirp->entry);
	}
	else return NULL;
	return NULL;
}

int initfs_open(const char *path, int flags, ...){
	initrd_file_t *module_var;
	int j = 0;
	int ret_fd __attribute__((unused)) = -1;
	module_var = fs_headers;
	//printf("J vale: %d fs_spec: %d cur_idfd: %d\n", j, fs_specs->nfiles, cur_irdfd);
	if(cur_irdfd >= MAX_INITRD_DESCRIPTORS) {
		int i=0;
		cur_irdfd=0;
		while(ird_descriptors[i].file_descriptor!=-1 && i < MAX_INITRD_DESCRIPTORS) i++;			
		cur_irdfd = i;
		//printf("i: %d\n", i);
	}
	while (j < fs_specs->nfiles) {
		//printf(".");
		if(!strcmp(path, module_var[j].fileName)){
			if(module_var[j].file_type == FS_DIRECTORY || module_var[j].file_type == FS_MOUNTPOINT)
			//Erroneus file type
				return -1;
			ird_descriptors[cur_irdfd].file_descriptor	= j;
			ret_fd = cur_irdfd;				
			//printf("ret_fd: %d --- %d\n", cur_irdfd, j);
			ird_descriptors[cur_irdfd].cur_pos = 0;
			if(flags&O_APPEND) {
				printf("Appendiamoci\n");
				ird_descriptors[cur_irdfd].cur_pos = module_var[j].length;
			}
			//else printf("Pero' non ci appendiamo\n");
			return cur_irdfd++; 
		}
		j++;
	}
	//printf("\n");
	if(flags&O_CREAT) {
		printf("O_CREAT Flag\n");
		if(fs_specs->nfiles < MAX_FILES ){			
			module_var[fs_specs->nfiles].magic=0xBF;
			strcpy(module_var[fs_specs->nfiles].fileName, path);
			module_var[fs_specs->nfiles].file_type = FS_FILE;
			module_var[fs_specs->nfiles].uid = current_user.uid;
			module_var[fs_specs->nfiles].offset = ++fs_end;
			module_var[fs_specs->nfiles].length = 0;
			ird_descriptors[cur_irdfd].file_descriptor	= fs_specs->nfiles;
			ird_descriptors[cur_irdfd].cur_pos = 0;
			fs_specs->nfiles++;				
			return cur_irdfd++; 
		}
		return -1;
	}
	
	//printf("Qua\n");
	return -1;	
}

ssize_t initfs_read(int fildes, char *buf, size_t nbyte){
	char *file_start;
	int lfd, file_size;	
	int j=0;	
	int read_pos = 0;
	lfd = ird_descriptors[fildes].file_descriptor;
	read_pos = ird_descriptors[fildes].cur_pos;
	file_size = fs_headers[lfd].length;
	file_start = (char *) (module_start	+ fs_headers[lfd].offset);
	//printf("cur_pos val: %d\n", ird_descriptors[fildes].cur_pos);
	if(nbyte == 0) return 0;	
	while(j<nbyte && read_pos<file_size){		
		*buf++ = file_start[read_pos];			
		read_pos++;
		j++;
	}			
	ird_descriptors[fildes].cur_pos = read_pos;		
	if(read_pos == file_size) {
		return 0; //Return an EOF
	}
	return nbyte;
}

int initrd_stat(char* path, struct stat *buf){
	int i;
	initrd_file_t *module_var;
	module_var = fs_headers;
	i=0;	
	while(i<MAX_FILES){			
		if(!strcmp(path, module_var[i].fileName)) {			
			buf->st_uid = module_var[i].uid;
			buf->st_size = module_var[i].length;
			break;
		}
		i++;
	}
	//printf("Initrd stat function\n");	
	//buf->st_uid = 33;	
	if(i==MAX_FILES) return -1;
	else return 0;
}

ssize_t initrd_write(int fildes, const void *buf, size_t nbyte){
	char *file_start;
	char *appoggio;
	appoggio = (char *) kmalloc(strlen(buf)*sizeof(char));
	strcpy(appoggio, buf);
	unsigned int lfd = 0;
	int i = 0;
	lfd = ird_descriptors[fildes].file_descriptor;
	printf("Please wait, im writing the world...\n");
	printf("And the world begun with those words: %s and his mark his: %d\n", appoggio, lfd);
	file_start = (char *) (module_start	+ fs_headers[lfd].offset + ird_descriptors[fildes].cur_pos);	
	while(i<=nbyte) {	
		file_start[i] = appoggio[i];	
		i++;
	} 
	fs_headers[lfd].length = fs_headers[lfd].length+i;
	free(appoggio);
	return i;
	//fs_headers[ird_descriptors.fildes].
}

int initrd_close(int fildes){
	ird_descriptors[fildes].file_descriptor = -1;
	ird_descriptors[fildes].cur_pos = 0;
	return 0;
}

