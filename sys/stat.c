/*
 * Dreamos
 * stat.c
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
#include <stdio.h>
#include <sys/stat.h>

struct mountpoint_t mountpoint_list[MAX_MOUNTPOINT];

int stat(const char *path, struct stat *buf){
	int mp_id;
	mp_id = 0;
	mp_id = get_mountpoint_id((char*) path);	
	printf("%d\n", mp_id);
	buf->st_dev = mp_id;
	if(mountpoint_list[mp_id].stat_op.stat!=NULL) mountpoint_list[mp_id].stat_op.stat((char*)path, buf);
	else printf("Null\n");
	return 0;
}

