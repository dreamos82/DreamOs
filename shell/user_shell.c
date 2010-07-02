/*
 * Dreamos
 * user_shell.c
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <user_shell.h>
#include <stdio.h>
#include <vfs.h>
#include <fcntl.h>
#include <string.h>
 
int user_chk(char *username){
	int fd = -1;
	int i = 0;
	int err;	
	struct passwd_user checking_user;	
	printf("Username provided: %s\n", username);
	fd = open("/passwd", O_RDONLY,0);
	if(fd != -1) printf("File passwd found\n");
	else printf("not found\n");
	user_get(fd, &checking_user);
	//strcpy(checking_user.username, appo);
	printf("%s - %s\n", username, checking_user.username);
	close(fd);
}

int user_get(int fd, struct passwd_user *checking_user){
	int i=0;
	char appo[50];	
	int err;
	memset(appo, '\0', 50);
//	printf("placeholder\n");
	while(i < 50 || appo[i] != ':'){
		err = read(fd, &appo[i], 1);
		if(appo[i] == ':') {
			appo[i] = '\0'; 
			break;
		}
		else i++;
	}
	strcpy(checking_user->username, appo);
	return 0;
}
