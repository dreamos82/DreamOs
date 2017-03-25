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
#include <stdio.h>
#include <string.h>
#include <vfs.h>
#include "debug.h"

#ifdef LEGACY

#include <kheap.h>

#endif
#ifdef LATEST
#include <heap.h>
#endif

/**
  * @author Ivan Gualandri
  * @param char* path percorso del file da aprire
  * @return DIR* puntatore alla cartella aperta se esiste, NULL altrimenti
  *
  * Dato un path contenente una cartella viene aperta se presente, e si torna il puntatore a DIR*
  */
DIR * opendir(const char * path)
{
    // Get the mount point id.
    int mpoint_id = get_mountpoint_id((char *) path);
    // Get the relative path.
    char * rel_path = get_rel_path(mpoint_id, path);
    #ifdef DEBUG
    int error = get_abs_path((char *) path);
    dbg_print("OPENDIR\n");
    dbg_print("\tPath     : %s\n", path);
    dbg_print("\tPLength  : %d\n", strlen(path));
    dbg_print("\tError    : %d\n", error);
    dbg_print("\tRelative : %s\n", rel_path);
    dbg_print("\tRLength  : %d\n", strlen(rel_path));
    #else
    get_abs_path((char *) path);
    #endif
    DIR * pdir = NULL;
    if (mountpoint_list[mpoint_id].dir_op.opendir_f != NULL)
    {
        pdir = mountpoint_list[mpoint_id].dir_op.opendir_f(rel_path);
        if (pdir != NULL)
        {
            pdir->handle = mpoint_id;
        }
    }
    else
    {
        dbg_print("Could not open_dir no function found!\n");
    }
    return pdir;
}

/**
  * @author Ivan Gualandri
  * @param DIR* dirp la cartella aperta
  * @return struct dirent Struttura dati contenente il prossimo elemento della cartella. 
  * 
  * ad ogni chiamata su dirp torna il successivo elemento presente in quel path. Se non ve ne sono piu torna NULL
  *  
  */
struct dirent * readdir(DIR * dirp)
{
    if (dirp == NULL)
    {
        dbg_print("NULL dirp\n");
        return NULL;
    }
//    dbg_print("Handle: %d\n", dirp->handle);
    if (mountpoint_list[dirp->handle].dir_op.readdir_f != NULL)
    {
//        dbg_print("Trovata readdir\n");
        return mountpoint_list[dirp->handle].dir_op.readdir_f(dirp);
    }
    else
    {
//        dbg_print("No readdir - No party\n");
    }
    return NULL;
}

/**
  * @author Ivan Gualandri
  * @param DIR* dirp
  *
  * Dato un puntatore a DIR lo chiude liberando le strutture dati.
  */
int closedir(DIR * dirp)
{
    //dbg_print("Closing directory\n");
    kfree(dirp);
    return 0;
}

DIR * fake_opendir(const char * path)
{
    dbg_print("One day, when i will grow up, i could open that path: %s\n", path);
    return NULL;
}
