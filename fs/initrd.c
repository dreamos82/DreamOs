/*
 * Dreamos
 * initrd.c
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

#include <stdio.h>
#include <stddef.h>
#include <initrd.h>

void dummy(){
	printf("Qui solo per una prova\n");
}	

DIR *dummy_opendir(const char *path){
	printf("Sono una dummy_opendir, ti piaccio? path: %s\n", path);
	return NULL;
}

int dummy_open(char *path, int flags){
	printf("Hi, i'm a dummy open. And i do nothing!!!\n");
}
