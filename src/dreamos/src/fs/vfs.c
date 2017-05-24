/*
 * Dreamos
 * vfs.c
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

#include <video.h>
#include <vfs.h>
#include "debug.h"

#ifdef LEGACY

#include <kheap.h>

#endif
#ifdef LATEST
#include <heap.h>
#endif

#include <string.h>
#include <initrd.h>
#include <shell.h>

struct mountpoint_t mountpoint_list[MAX_MOUNTPOINT];
file_descriptor_t fd_list[_SC_OPEN_MAX];
char * module_start;
int current_fd;

void vfs_init()
{
    int i, j;
    i = 0;
    j = 0;
    current_fd = 0;

    while (i < MAX_MOUNTPOINT)
    {
        strcpy(mountpoint_list[i].mountpoint, "");
        mountpoint_list[i].uid = 0;
        mountpoint_list[i].gid = 0;
        mountpoint_list[i].pmask = 0;
        mountpoint_list[i].dev_id = 0;
        mountpoint_list[i].start_address = 0;
        mountpoint_list[i].dir_op.opendir_f = NULL;
        mountpoint_list[i].operations.open = NULL;
        mountpoint_list[i].operations.close = NULL;
        mountpoint_list[i].operations.read = NULL;
        mountpoint_list[i].operations.write = NULL;
        mountpoint_list[i].stat_op.stat = NULL;
        i++;
    }

    strcpy(mountpoint_list[0].mountpoint, "/");
    mountpoint_list[0].uid = 0;
    mountpoint_list[0].gid = 0;
    mountpoint_list[0].pmask = 0;
    mountpoint_list[0].dev_id = 0;
    mountpoint_list[0].start_address = (unsigned int) module_start;
    mountpoint_list[0].dir_op.opendir_f = initfs_opendir;
    mountpoint_list[0].dir_op.readdir_f = initrd_readdir;
    mountpoint_list[0].operations.open = &initfs_open;
    mountpoint_list[0].operations.close = &initrd_close;
    mountpoint_list[0].operations.read = &initfs_read;
    mountpoint_list[0].operations.write = &initrd_write;
    mountpoint_list[0].stat_op.stat = initrd_stat;
//    mountpoint_list[0].operations = kmalloc(sizeof(struct super_node_operations));

    strcpy(mountpoint_list[1].mountpoint, "/dev");
    mountpoint_list[1].uid = 0;
    mountpoint_list[1].gid = 0;
    mountpoint_list[1].pmask = 0;
    mountpoint_list[1].dev_id = 0;
    mountpoint_list[1].start_address = 0;
    //mountpoint_list[1].operations = kmalloc(sizeof(struct super_node_operations));

    strcpy(mountpoint_list[2].mountpoint, "/dev/video");
    mountpoint_list[2].uid = 0;
    mountpoint_list[2].gid = 0;
    mountpoint_list[2].pmask = 0;
    mountpoint_list[2].dev_id = 0;
    mountpoint_list[2].start_address = 0;
    //mountpoint_list[2].operations = kmalloc(sizeof(struct super_node_operations));

    strcpy(mountpoint_list[3].mountpoint, "/root");
    mountpoint_list[3].uid = 0;
    mountpoint_list[3].gid = 0;
    mountpoint_list[3].pmask = 0;
    mountpoint_list[3].dev_id = 0;
    mountpoint_list[3].start_address = 0;
    //mountpoint_list[3].operations = kmalloc(sizeof(struct super_node_operations));

    while (j < _SC_OPEN_MAX)
    {
        fd_list[j].fs_spec_id = -1;
        fd_list[j].mountpoint_id = -1;
        fd_list[j].offset = 0;
        j++;
    }
}

int32_t get_mountpoint_id(const char * path)
{
    size_t last_mpl = 0;
    int32_t last_mp_id = -1;
    for (uint32_t it = 0; it < MAX_MOUNTPOINT; ++it)
    {
        size_t mpl = strlen(mountpoint_list[it].mountpoint);
        if (!strncmp(path, mountpoint_list[it].mountpoint, mpl))
        {
            if (((mpl > last_mpl) && it > 0) || (it == 0))
            {
                last_mpl = mpl;
                last_mp_id = it;
            }
        }
    }
    return last_mp_id;
}

char * get_relative_path(uint32_t mp_id, const char * path)
{
    // Get the size of the path.
    size_t path_size = strlen(path);
    // Get the size of the mount-point.
    size_t mnpt_size = strlen(mountpoint_list[mp_id].mountpoint);
    // Get the size of the relative path.
    size_t rltv_size = path_size - mnpt_size;
    // Create a new string for storing the relative path.
    char * rltv_path = kmalloc(rltv_size * sizeof(char));
    // Copy the relative path.
    if (rltv_size > 0)
    {
        size_t i;
        for (i = 0; i < rltv_size; ++i)
        {
            rltv_path[i] = path[mnpt_size + i];
        }
        rltv_path[i] = '\0';
    }
    else
    {
        strcpy(rltv_path, path);
    }
    return rltv_path;
}

int get_absolute_path(char * path)
{
    if (path[0] != '/')
    {
        char abspath[CURPATH_LEN];
        int abs_size = 0;
        abs_size = strlen(current_user.cur_path);
        memset(abspath, '\0', CURPATH_LEN);
        strcpy(abspath, current_user.cur_path);
        if (abspath[abs_size - 1] == '/')
        {
            strncat(abspath, path, strlen(path));
        }
        else
        {
            strncat(abspath, "/", strlen(path));
            strncat(abspath, path, strlen(path));
        }
        strcpy(path, abspath);
        return strlen(path);
    }
    return 0;
}
