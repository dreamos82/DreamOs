/*
 * Dreamos
 * user_shell.h
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

#ifndef USER_SHELL_H
#define USER_SHELL_H

typedef enum password_ctrl {
	NONE,
	CLEAR,
	MD5
} pwd_t;

struct passwd_user {
	char username[50];
	char password[50];
};

int user_chk(char *, char *);
int user_get(int, struct passwd_user *);
//int check_pwd(struct passwd, char*);
#endif
