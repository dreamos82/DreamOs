/*
 * Dreamos
 * dirent.c
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
 
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <vfs.h>
#include <kheap.h>

struct mountpoint_t mountpoint_list[MAX_MOUNTPOINT];

DIR *opendir(const char *path){
	int mpoint_id = 0;
	char* rel_path;	
	DIR* pdir;

	mpoint_id = get_mountpoint_id((char*)path);
	rel_path = get_rel_path(mpoint_id, path);	
	//printf("%d - %s\n", mpoint_id, rel_path);
	if(mountpoint_list[mpoint_id].dir_op.opendir_f!=NULL) {
		pdir = mountpoint_list[mpoint_id].dir_op.opendir_f(rel_path);
		pdir->handle = mpoint_id;
		return pdir;
	}
	else {
		//printf("Could not open_dir no function found\n");
		return NULL;
	}
	return NULL;
}

struct dirent *readdir(DIR *dirp){
	//printf("Handle: %d\n", dirp->handle);
	if(mountpoint_list[dirp->handle].dir_op.readdir_f!=NULL){
		//printf("Trovata readdir\n");
		return mountpoint_list[dirp->handle].dir_op.readdir_f(dirp);
	}
	else printf("No readdir - No party\n");
	return NULL;
}

int closedir(DIR *dirp){
	//printf("Closing directory\n");
	free(dirp);
	return 0;
}

DIR* fake_opendir (const char *path){
	printf("One day, when i will grow up, i could open that path: %s\n", path);
	return NULL;
}
