/// @file   vfs.h
/// @brief  Headers for Virtual File System (VFS).
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

#pragma once

#include "dirent.h"
#include "stddef.h"
#include "unistd.h"
#include "stdint.h"
#include "language.h"
#include "stat.h"

#define MAX_MOUNTPOINT 10
#define MAX_FD _SC_OPEN_MAX

#define FS_FILE         0x01
#define FS_DIRECTORY    0x02
#define FS_MOUNTPOINT   0x03

/// @brief Set of functions used to perform operations on directories.
typedef struct directory_operations_t
{
    /// @brief Open directory.
    DIR * (* opendir_f)(const char *);

    /// @brief Close directory.
    int (* closedir_f)(DIR * dirp);

    /// @brief Read next entry inside the directory.
    dirent_t * (* readdir_f)(DIR * dirp);
} directory_operations_t;

/// @brief Qui vanno i puntatori alle funzioni sul supernode.
typedef struct super_node_operations_t
{
    /// @brief Open a file.
    int (* open)(const char *, int, ...);

    /// @brief Close a file.
    int (* close)(int);

    /// @brief Read from a file.
    ssize_t (* read)(int, char *, size_t);

    /// @brief Write inside a file.
    ssize_t (* write)(int, const void *, size_t);
} super_node_operations_t;

/// @brief Stat operations.
typedef struct stat_operations_t
{
    /// @brief Stat function.
    int (* stat)(const char *, stat_t *);
} stat_operations_t;

/// @brief Struttura dati che contiene le informazioni sui files system montati.
typedef struct mountpoint_t
{
    /// Indica il nome del mountpoint.
    char mountpoint[64];
    /// Maschera dei permessi.
    unsigned int pmask;
    /// User ID.
    unsigned int uid;
    /// Group ID.
    unsigned int gid;
    /// Indirizzo di partenza del FileSystem.
    unsigned int start_address;
    /// Device ID.
    int dev_id;
    /// Operations on files.
    super_node_operations_t operations;
    /// Operations on directories.
    directory_operations_t dir_op;
    /// Stat operations.
    stat_operations_t stat_op;
} mountpoint_t;

/// @brief Struttura dati che contiene le informazioni su di un file aperto.
typedef struct file_descriptor_t
{
    /// Il descrittore del file interno del FileSystem.
    int fs_spec_id;
    /// L'id del mountpoint nel quale si trova il file.
    int mountpoint_id;
    /// Offset per lettura file, per la prossima read.
    int offset;
    /// Flags per le modalita di apertura del file.
    int flags_mask;
} file_descriptor_t;

extern int current_fd;
extern file_descriptor_t fd_list[_SC_OPEN_MAX];
extern mountpoint_t mountpoint_list[MAX_MOUNTPOINT];

/// @brief Initialize the Virtual File System (VFS).
void vfs_init();

/// @brief Retrieves the id of the mount point where the path resides.
/// @param path The path to the mountpoint.
/// @return The id of the mountpoint.
int32_t get_mountpoint_id(const char * path);

/// @brief Dato un path viene estratto il percorso relativo, escluso il mountpoint.
/// @param mp_id    Id del punto di mountpoint del file.
/// @param path     Percorso del file da aprire.
/// @return Path senza la parte relativa al mountpoint.
char * get_relative_path(uint32_t mp_id, const char * path);

/// @brief Dato un path ne estrae il percorso assoluto (a partire da quello
/// corrente).
/// @param path Percorso del file da aprire.
/// @return Error code.
int get_absolute_path(char * path);
