/*
 * Dreamos
 * initrd.h
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

#ifndef INITRD_H
#define INITRD_H

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>

#define FILENAME_LENGTH         64
#define MAX_FILES               32
#define MAX_INITRD_DESCRIPTORS  _SC_OPEN_MAX

/// @brief Contains the number of files inside the initrd filesystem.
typedef struct initrd_t
{
    /// Number of readed files.
    uint32_t nfiles;
} initrd_t;

/// @brief Information concerning a file.
typedef struct initrd_file_t
{
    /// Number used as delimiter, it must be set to 0xBF.
    int magic;
    /// The name of the file.
    char fileName[FILENAME_LENGTH];
    /// The type of the file.
    short int file_type;
    /// The uid of the owner.
    int uid;
    /// Offset of the starting address.
    unsigned int offset;
    /// Dimension of the file.
    unsigned int length;
} initrd_file_t;

/// @brief Descriptor linked to open files.
typedef struct initrd_fd
{
    /// Id of the open file inside the file system. More precisely, its index
    /// inside the vector of files.
    int file_descriptor;
    /// The current position inside the file. Used for writing/reading
    /// opeations.
    int cur_pos;
} initrd_fd;

extern initrd_t * fs_specs;
extern initrd_file_t * fs_headers;
extern unsigned int fs_end;

/// Initializes the initrd file system.
uint32_t initfs_init();

/// Opens a directory at the given path.
/// @param path The path where the directory resides.
/// @return Structure used to access the directory.
DIR * initfs_opendir(const char * path);

/// Moves the position of the currently readed entry inside the directory.
/// @param dirp The directory handler.
/// @return A pointer to the next entry inside the directory.
dirent_t * initrd_readdir(DIR * dirp);

/// @brief Open the file at the given path and returns its file descriptor.
/// @param path  The path to the file.
/// @param flags The flags used to determine the behavior of the function.
/// @return The file descriptor of the opened file, otherwise returns -1.
int initfs_open(const char * path, int flags, ...);

/// @brief Reads from the file identified by the file descriptor.
/// @param fildes   The file descriptor of the file.
/// @param buf      Buffer where the read content must be placed.
/// @param nbyte    The number of bytes to read.
/// @return The number of red bytes.
ssize_t initfs_read(int fildes, char * buf, size_t nbyte);

/// @brief Retrieves information concerning the file at the given position.
/// @param path The path where the file resides.
/// @param stat The structure where the information are stored.
/// @return 0 if success.
int initrd_stat(const char * path, stat_t * stat);

/// @brief Writes the given content inside the file.
/// @param fildes   The file descriptor of the file.
/// @param buf      The content to write.
/// @param nbyte    The number of bytes to write.
/// @return The number of written bytes.
ssize_t initrd_write(int fildes, const void * buf, size_t nbyte);

/// @brief Closes the given file.
/// @param fildes The file descriptor of the file.
int initrd_close(int fildes);

#endif
