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
#include <debug.h>
#include <language.h>

//#define PWD_CHECK 1
int user_chk(char * username, char * usr_pwd)
{
    (void) usr_pwd;
    int fd = -1;
    struct passwd_user checking_user;
    //char *path = kmalloc(sizeof(char) * 8);
    //memset(path,0,8);
    //strcpy(path, "/passwd");
    memset(checking_user.username, '\0', 50);
    memset(checking_user.password, '\0', 50);
    //puts(username);
    //printf("user: %s\n", username);
    dbg_bochs_print("Logging In\n");
    fd = open("/passwd", O_RDONLY, 0);
    /*if(fd != -1) printf("File passwd found\n");
    else printf("not found\n");*/
    if (fd < 0) return 0;
    while (user_get(fd, &checking_user) == 2)
    {
        #ifdef PWD_CHECK
        if(!strcmp(username, checking_user.username) && !strcmp(usr_pwd, checking_user.password)) {
            close(fd);
            return 1;
        }
        #else
        if (!strcmp(username, checking_user.username))
        {
            close(fd);
            return 1;
        }
        #endif
        //else printf("Not Found ");
        //printf("name: %s - password: %s\n", checking_user.username, checking_user.password);
    }
    //user_get(fd, &checking_user);
    //printf("name: %s - password: %s\n", checking_user.username, checking_user.password);
    //strcpy(checking_user.username, appo);
    close(fd);
    return 0;
}

int user_get(int fd, struct passwd_user * checking_user)
{
    int i = 0;
    char appo[51];
    int err = 1;
    int status = 0;
    memset(appo, '\0', 50);
//	printf("placeholder\n");	
    while ((i < 50 || appo[i] != ':') && err != 0)
    {
        err = read(fd, &appo[i], 1);
        if (appo[i] == ':')
        {
            appo[i] = '\0';
            break;
        }
        else i++;
    }
    if (err == 0) return err;
    status++;
    strcpy(checking_user->username, appo);
    i = 0;
    memset(appo, '\0', 50);
    while ((i < 50 || appo[i] != '\n') && err != 0)
    {
        err = read(fd, &appo[i], 1);
        if (appo[i] == '\n')
        {
            appo[i] = '\0';
            break;
        }
        else i++;
    }
    //if(err ==0) return err;
    status++;
    strcpy(checking_user->password, appo);
    //printf("Status: %sd\n", status);
    return status;
}

int pwd_check(char * password, char * realpass)
{
    (void) password;
    (void) realpass;
    return 0;
}
