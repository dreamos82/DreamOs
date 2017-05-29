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

#include "dirent.h"
#include "string.h"
#include "vfs.h"
#include "debug.h"
#include "kheap.h"

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
    int32_t mp_id = get_mountpoint_id(path);
    if (mp_id < 0)
    {
        return NULL;
    }
    if (mountpoint_list[mp_id].dir_op.opendir_f != NULL)
    {
        // Get the relative path.
        char * relative_path = get_relative_path((uint32_t) mp_id, path);
        DIR * pdir = mountpoint_list[mp_id].dir_op.opendir_f(relative_path);
        kfree(relative_path);
        if (pdir != NULL)
        {
            pdir->handle = mp_id;
        }
        return pdir;
    }
    dbg_print("Could not open_dir no function found!\n");
    return NULL;
}

/**
  * @author Ivan Gualandri
  * @param DIR* dirp la cartella aperta
  * @return Struttura dati contenente il prossimo elemento della cartella.
  * 
  * ad ogni chiamata su dirp torna il successivo elemento presente in quel path. Se non ve ne sono piu torna NULL
  *  
  */
dirent_t * readdir(DIR * dirp)
{
    if (dirp != NULL)
    {
        if (mountpoint_list[dirp->handle].dir_op.readdir_f != NULL)
        {
            return mountpoint_list[dirp->handle].dir_op.readdir_f(dirp);
        }
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
    if (dirp != NULL)
    {
        kfree(dirp);
    }
    return 0;
}

DIR * fake_opendir(const char * path)
{
    dbg_print("One day, when i will grow up, i could open that path: %s\n",
              path);
    return NULL;
}
