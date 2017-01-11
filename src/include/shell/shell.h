/*
 * Shell.h
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _SHELL_H
#define _SHELL_H

#define USER_LEN 22
#define CMD_LEN 256
#define DESC_LEN 256
#define CURPATH_LEN 256
#define NUM_COM 22
#define DEF_COM 18
#define HST_LEN 10

#define RESET_POSIZ posiz = HST_LEN - 1;

#define CHECK_POSIZ if (posiz < free_slots)\
                        RESET_POSIZ\
                    else if (posiz > HST_LEN - 1)\
                        posiz = free_slots;

#define INC_POSIZ ++posiz;\
                    CHECK_POSIZ
#define DEC_POSIZ --posiz;\
                    CHECK_POSIZ


struct cmd {
	char cmdname[CMD_LEN];
	void (*h_func)(void);
	char cmddesc[DESC_LEN];
};

struct user_env {
	char username[USER_LEN];
	char cur_path[CURPATH_LEN];
	unsigned int uid;
	unsigned int gid;
};

//extern char cur_path[256];
extern struct cmd shell_cmd[NUM_COM];
typedef struct user_env userenv_t;
extern userenv_t current_user;
void shell_init();
int shell(void *);
void history (char *);
void history_start(void);

#endif
