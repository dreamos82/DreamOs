/*
 * Dreamos
 * vfs.h
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
#ifndef _VFS_H
#define _VFS_H

#define MAX_MOUNTPOINT 10
#define MAX_FD 255

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02

struct inode {
	char filename[256];
	int type;
 	int length;
	struct inode *parent;

	/* Inutili per adesso */
	int uid;
	int gid;
	int pmask;

 	/* puntatori a funzione */
	void (*open)(struct inode *);
	void (*close)(struct inode *);
	int (*read)(struct inode *, int, int, int *);
	int (*write)(struct inode *, int, int, int *);
};

/*!  \struct mountpoint_t
     \brief Struttura dati che contiene le informazioni sui files system montati
 */
struct mountpoint_t {
		char mountpoint[64];/**< Indica il nome del mountpoint*/
		unsigned int pmask; /**< Maschera dei permessi*/
		unsigned int uid; /**< User ID*/
		unsigned int gid; /**< Group ID*/
		unsigned int start_address; /**< Indirizzo di partenza del FileSystem*/
		int dev_id; /**< Device ID*/		
		struct super_node_operations *operations;
};

struct super_node_operations {
	/*Qui vanno i puntatori alle funzioni sul supernode*/
	void (*open)(char *r_path, int o_flags);
	void (*close)(int);
	/*t (*read)(stct inode *, int, int, int *);
	int (*write)(struct inode *, int, int, int *);*/
};

extern struct inode *root;
extern struct mountpoint_t mountpoint_list[MAX_MOUNTPOINT];

void open_vfs (struct inode *);
void close (struct inode *);
int read (struct inode *, int, int, int *);
int write (struct inode *, int, int, int *);

void vfs_init();

#endif

