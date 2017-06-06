/// @file   initrd.c
/// @brief  Headers of functions for initrd filesystem.
/// @author Ivan Gualandri
/// @date   Mar 31 2007
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

#include "stdio.h"
#include "initrd.h"
#include "string.h"
#include "vfs.h"
#include "debug.h"
#include "shell.h"
#include "kheap.h"
#include "fcntl.h"

char * module_start;

initrd_t * fs_specs;
initrd_file_t * fs_headers;
unsigned int fs_end;

/// The list of file descriptors.
initrd_fd ird_descriptors[MAX_INITRD_DESCRIPTORS];
/// The currently opened file descriptor.
unsigned int cur_irdfd;

uint32_t initfs_init()
{
    int i = 0;
    fs_end = 0;
    fs_specs = (initrd_t *) module_start;
    fs_headers = (initrd_file_t *) (module_start + sizeof(initrd_t));
    while (i < MAX_INITRD_DESCRIPTORS)
    {
        ird_descriptors[i].file_descriptor = -1;
        ird_descriptors[i].cur_pos = 0;
        i++;
    }
    cur_irdfd = 0;
    printf(" * Number of Files: %d\n", fs_specs->nfiles);
    fs_end = fs_headers[(fs_specs->nfiles) - 1].offset +
             fs_headers[(fs_specs->nfiles) - 1].length;
    printf(" * Filesystem end : %d\n", fs_end);
    return fs_specs->nfiles;
}

DIR * initfs_opendir(const char * path)
{
    DIR * pdir = NULL;
    initrd_file_t * module_var = fs_headers;
    if (!strcmp(path, "/") || path[0] == '\0')
    {
        pdir = kmalloc(sizeof(DIR));
        strcpy(pdir->path, path);
        pdir->handle = 0x01;
        pdir->cur_entry = 0x00;
        pdir->entry.d_ino = 0x00;
        strcpy(pdir->entry.d_name, module_var[0].fileName);
        for (uint32_t i = 0; i < MAX_FILES; ++i)
        {
            dbg_print("[%d] uid:%3d name:%s\n",
                      i,
                      module_var[i].uid,
                      module_var[i].fileName);
        }
    }
    else
    {
        dbg_print("No OpenDir function has been set.");
    }
    return pdir;
}

dirent_t * initrd_readdir(DIR * dirp)
{
    initrd_file_t * fs_type;
    if (dirp->cur_entry < fs_specs->nfiles)
    {
        fs_type = (initrd_file_t *) (module_start + sizeof(initrd_t));
        dirp->entry.d_ino = dirp->cur_entry;
        dirp->entry.d_type = fs_type[dirp->cur_entry].file_type;
        strcpy(dirp->entry.d_name, fs_type[dirp->cur_entry].fileName);
        dirp->cur_entry++;
        return &(dirp->entry);
    }
    return NULL;
}

int initfs_open(const char * path, int flags, ...)
{
    initrd_file_t * module_var = fs_headers;
    // If we have reached the maximum number of descriptors, just try to find
    // a non-used one.
    if (cur_irdfd >= MAX_INITRD_DESCRIPTORS)
    {
        // Reset the current ird file descriptor.
        cur_irdfd = 0;
        while ((ird_descriptors[cur_irdfd].file_descriptor != -1) &&
               (cur_irdfd < MAX_INITRD_DESCRIPTORS))
        {
            ++cur_irdfd;
        }
    }
    for (uint32_t it = 0; it < fs_specs->nfiles; ++it)
    {
        // Discard the headers which has a different name.
        if (strcmp(path, module_var[it].fileName)) continue;
        // However, if the name is the same, but the file type is different,
        // stop the function and return failure value.
        if ((module_var[it].file_type == FS_DIRECTORY) ||
            (module_var[it].file_type == FS_MOUNTPOINT))
        {
            return -1;
        }
        // Otherwise, if the file is correct, update
        ird_descriptors[cur_irdfd].file_descriptor = it;
        ird_descriptors[cur_irdfd].cur_pos = 0;
        if (flags & O_APPEND)
        {
            ird_descriptors[cur_irdfd].cur_pos = module_var[it].length;
        }
        return cur_irdfd++;
    }
    if (flags & O_CREAT)
    {
        if (fs_specs->nfiles < MAX_FILES)
        {
            module_var[fs_specs->nfiles].magic = 0xBF;
            strcpy(module_var[fs_specs->nfiles].fileName, path);
            module_var[fs_specs->nfiles].file_type = FS_FILE;
            module_var[fs_specs->nfiles].uid = current_user.uid;
            module_var[fs_specs->nfiles].offset = ++fs_end;
            module_var[fs_specs->nfiles].length = 0;
            ird_descriptors[cur_irdfd].file_descriptor = fs_specs->nfiles;
            ird_descriptors[cur_irdfd].cur_pos = 0;
            fs_specs->nfiles++;
            return cur_irdfd++;
        }
        return -1;
    }
    return -1;
}

ssize_t initfs_read(int fildes, char * buf, size_t nbyte)
{
    // If the number of byte to read is zero, skip.
    if (nbyte == 0) return 0;
    // Get the file descriptor of the file.
    int lfd = ird_descriptors[fildes].file_descriptor;
    // Get the current position.
    int read_pos = ird_descriptors[fildes].cur_pos;
    // Get the legnth of the file.
    int file_size = fs_headers[lfd].length;
    // Get the begin of the file.
    char * file_start = (module_start + fs_headers[lfd].offset);
    // Declare an iterator.
    size_t it = 0;
    while ((it < nbyte) && (read_pos < file_size))
    {
        *buf++ = file_start[read_pos];
        ++read_pos;
        ++it;
    }
    ird_descriptors[fildes].cur_pos = read_pos;
    if (read_pos == file_size)
    {
        return 0; //Return an EOF
    }
    return nbyte;
}

int initrd_stat(const char * path, stat_t * stat)
{
    int i;
    initrd_file_t * module_var;
    module_var = fs_headers;
    i = 0;
    while (i < MAX_FILES)
    {
        if (!strcmp(path, module_var[i].fileName))
        {
            stat->st_uid = module_var[i].uid;
            stat->st_size = module_var[i].length;
            break;
        }
        i++;
    }
    //dbg_print("Initrd stat function\n");
    //buf->st_uid = 33;
    if (i == MAX_FILES) return -1;
    else return 0;
}

ssize_t initrd_write(int fildes, const void * buf, size_t nbyte)
{
    // If the number of byte to write is zero, skip.
    if (nbyte == 0) return 0;
    // Make a copy of the buffer.
    char * tmp = (char *) kmalloc(strlen(buf) * sizeof(char));
    strcpy(tmp, buf);
    // Get the file descriptor of the file.
    int lfd = ird_descriptors[fildes].file_descriptor;
    printf("Please wait, im writing the world...\n");
    printf("And the world begun with those words: %s and his mark his: %d\n",
           tmp, lfd);
    // Get the begin of the file.
    char * file_start = (module_start + fs_headers[lfd].offset +
                         ird_descriptors[fildes].cur_pos);
    // Declare an iterator.
    size_t it = 0;
    while (it <= nbyte)
    {
        file_start[it] = tmp[it];
        ++it;
    }
    // Increment the length of the file.
    fs_headers[lfd].length = fs_headers[lfd].length + it;
    // Free the memory of the temporary file.
    kfree(tmp);
    // Return the number of written bytes.
    return it;
}

int initrd_close(int fildes)
{
    ird_descriptors[fildes].file_descriptor = -1;
    ird_descriptors[fildes].cur_pos = 0;
    return 0;
}

