/*
 * Dreamos
 * shell.c
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

#include <commands.h>
#include <multiboot.h>
#include <kernel.h>
#include <stddef.h>
#include <video.h>
#include <pic8259.h>
#include <8253.h>
#include <gdt.h>
#include <idt.h>
#include <cpuid.h>
#include <stdio.h>
#include <string.h>
#include <fismem.h>
#include <io.h>
#include <keyboard.h>
#include <paging.h>
#include <use.h>
#include <shell.h>
#include <version.h>
#include <cpuid.h>
#include <clock.h>
#include <sys/utsname.h>
#include <user_shell.h>
#include <debug.h>
#ifdef LATEST
#include <heap.h>
#endif
#define NUM_COM 30
userenv_t current_user;
int hst_flag;
char cmd[CMD_LEN];
//#define PWD_CHECK 1
struct cmd shell_cmd[NUM_COM] = {
 { "aalogo",   aalogo,    "  Show an ascii art logo" },
 { "clear",    _kclear,   "   Clear the screen" },
 { "poweroff", poweroff,  "Turn off the machine" },
 { "uname",    uname_cmd, "   Print kernel version, try uname --help for more info" },
 { "credits",  credits,   " Show DreamOS credits" },
 { "sleep",    sleep_cmd, "   Pause DreamOS for a particular number of seconds" },
 { "cpuid",    cpuid, 	  "   Show cpu identification informations" },
 { "date",     date, 	  "    Show date and time" },
 { "echo",     echo, 	  "    Print some lines of text" },
 { "help",     help,	  "    See the 'help' list to learn the DreamOS commands now available" },
 { "answer",   answer,    "  42" },
 { "drv_load", drv_load,  "Tool to load and kill drivers" },
 { "ls",       ls,        "      Tool for listing dir - not complete-" },
 { "cd",       cd,        "      Change dir - not complete-" },
 { "whoami",   whoami,    "  Show the current user name" },
 { "tester",   tester,    "  Try some functions, 'tester --help' for more info'" },	
 { "pwd",      pwd,       "     Print current working directory" },
 { "more",     more,      "    Read content of a file" },
 { "newfile",  newfile,	  "	Create a new file"}, 
 { "ps", 	   ps,		  "	Show task list"}, 
};
/*
 * Inserisce gli argomenti di un comando in un array di stringhe
 * argc = numero degli argomenti
 * argv[0] = nome del comando
 * argv[n] = opzioni successive
 */

int count = 9, posiz = 9, c = 10, limit = 1;
char *lastcmd[10] = {};
int hist_press = 0;
void options(char *com)
{  
  int i=0;
  argc=0;  
  for (; *com; com++)
  {    
    argv[argc] = (char *)kmalloc(sizeof(char) * 30);
    while (*com != ' ' && *com != '\0') {
      *(argv[argc] + i) = *com++;
      i++;
    } 
    *(argv[argc] + i) = '\0';
    argc++;
    i=0;
  }
  argv[argc] = '\0';
}

/* corpo della shell */
void shell()
{
  //dbg_bochs_print("\nNewShell\n");
  char *cmd_ptr;
  char password[30];
  /*static struct cmd shell_cmd[NUM_COM] = {
	{ "aalogo",   aalogo      },
	{ "clear",    _kclear     },
	{ "poweroff", poweroff    },
	{ "uname",    uname_cmd   },
	{ "credits",  credits     },
	{ "sleep",    sleep_cmd   },
	{ "cpuid",    cpuid 	  },
	{ "date",     date 	  },
	{ "echo",     echo 	  },
	{ "help",     help	  },
	{ "answer",   answer  },
	{ "drv_load", drv_load},
	{ "ls",       ls},
	{ "cd",       cd},
	{ "whoami",   whoami},
	{ "tester", tester},	
	{ "pwd", pwd},
	{ "more", more},
	{ "newfile", newfile}
  };*/


  int i = 0;
  int ret_val;
  hst_flag = 0;
  memset(cmd, '\0', CMD_LEN);
  memset(current_user.username, '\0', USER_LEN);
  memset(password, '\0', 30);
  memset(current_user.cur_path, '\0', CURPATH_LEN); 
  _kcolor(BRIGHT_BLUE);
  printf(LNG_WELCOME);
  _kcolor(WHITE);
  
  do {		
    printf(LNG_USER);
    scanf ("%23s",current_user.username);
    printf(LNG_USER_R);
    #ifdef PWD_CHECK
    printf(LNG_PWD);
    set_shadow(1);
    scanf ("%23s",password);
    set_shadow(0);
    #endif
    //printf("Vediamo %s pwd: %s\n", current_user.username, password);
    dbg_bochs_print((const unsigned char*)"Asking username");
    ret_val = user_chk(current_user.username, password);
  } while ((!strlen(current_user.username) || ret_val!=1));
    
  _kclear();  
  aalogo();
  printf("\n\n\n\n");
  argc=1;  
  strcpy(current_user.cur_path, "/root");
  current_user.uid = 1;
  current_user.gid = 0;
  for (;;)
  {
	//dbg_bochs_print("shell loop\n");
    for (c = 1 ; c <= 10 ; c++) {
    	    lastcmd[c] = (char *)kmalloc(sizeof(char) * 30); 
    }
    int cmdclean = 0;
    while(cmdclean<CMD_LEN){
		cmd[cmdclean] = '\0';
		cmdclean++;
	}	
    _kcolor(9);	    
    printf("%s", current_user.username);
    _kcolor(15);
	printf("~:%s# ", current_user.cur_path);
    scanf("%30s",cmd);        
    history(cmd);
   
    /* elimina eventuali spazi all'inizio del comando */
    for (i = 0, cmd_ptr = cmd; cmd[i] == ' '; i++, cmd_ptr++);
    //printf("%s\n", cmd_ptr);
    options (cmd_ptr);
    if (strlen(argv[0]) == 0)
        goto end;

    for (i = NUM_COM; i >= 0; --i) {
        if(strcmp(argv[0], shell_cmd[i].cmdname) == (int) NULL) {
            (*shell_cmd[i].h_func)();
            break;
        }
    }

    if (i<0)
      printf(LNG_UNKNOWN_CMD " %s\n", argv[0]);

end:
    memset(cmd, 0, CMD_LEN);    
    for (--argc; argc>=0; argc--) {      
        free (argv[argc]);
    }
    for (c = 1 ; c <= 10 ; c++) {
    	    free(lastcmd[c]); 
    }
  }
}

// History
void history(char *cmd_pass) {
    if ( count == 0 ) {
		count = 10; 
	}

    strncpy(lastcmd[count], cmd_pass, strlen(cmd_pass));
    
    posiz = count;
    count--;
}

void history_start(void) { 
    static int sc_uparrow;
    count++;
    int delete = 0, max_limit = strlen(lastcmd[count]);
	
    if ( count == 0 || count > 9 ) {
    	count = posiz;
    }
    
    if (limit < max_limit) {
		limit = max_limit;
	}
	
    while ( delete <= limit) {
		_kbackspace();
		delete++;
	}

    if( (sc_uparrow = inportb (0x60) ) == KEY_UPARROW ) {
   		printf("%s", lastcmd[count]); 
   		int cmdclean = 0;
		while(cmdclean<CMD_LEN){
			cmd[cmdclean] = '\0';
			cmdclean++;
		}	
   		strcpy(cmd, lastcmd[count]);
	}
       
}

