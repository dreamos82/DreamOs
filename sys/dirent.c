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
#include <vfs.h>


DIR *opendir(const char *path){
	int mpoint_id = 0;
	char* rel_path;

	mpoint_id = get_mountpoint_id(path);
	rel_path = get_rel_path(mpoint_id, path);
	printf("%d - %s\n", mpoint_id, rel_path);	
}
