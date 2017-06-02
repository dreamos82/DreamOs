/// @file   stat.c
/// @brief  Stat functions.
/// @author Ivan Gualandri
/// @date   Oct 27 2003
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "vfs.h"
#include "stdio.h"
#include "shell.h"
#include "string.h"
#include "kheap.h"

int stat(const char * path, stat_t * buf)
{
    // Reset the structure.
    buf->st_dev = 0;
    buf->st_ino = 0;
    buf->st_mode = 0;
    buf->st_uid = 0;
    buf->st_gid = 0;
    buf->st_size = 0;
    buf->st_atime = 0;
    buf->st_mtime = 0;
    buf->st_ctime = 0;
    char tmp_path[CURPATH_LEN];
    strcpy(tmp_path, path);
    if (path[0] != '/')
    {
        get_absolute_path(tmp_path);
    }
    int32_t mp_id = get_mountpoint_id(tmp_path);
    if (mp_id < 0)
    {
        printf("stat: cannot execute stat of \"%s\": Not exists\n", path);
        return -1;
    }
    buf->st_dev = (uint32_t) mp_id;
    if (mountpoint_list[mp_id].stat_op.stat == NULL)
    {
        printf("stat: cannot execute stat of \"%s\": Not stat function\n",
               path);
        return -1;
    }
    if (path[0] == '/')
    {
        char * relative_path = get_relative_path((uint32_t) mp_id, path);
        mountpoint_list[mp_id].stat_op.stat(relative_path, buf);
        kfree(relative_path);
    }
    else
    {
        mountpoint_list[mp_id].stat_op.stat(path, buf);
    }
    return 0;
}

