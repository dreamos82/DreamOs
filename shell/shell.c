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

#define NUM_COM 15

struct cmd {
	const char cmdname[CMD_LEN];
	void (*h_func)(void);
};

/*
 * Inserisce gli argomenti di un comando in un array di stringhe
 * argc = numero degli argomenti
 * argv[0] = nome del comando
 * argv[n] = opzioni successive
 */
void options(char *com)
{
  int i=0;
  argc=0;

  for (; *com; com++)
  {    
    argv[argc] = (char *)kmalloc(sizeof(char) * 10);
    while (*com != ' ') {
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
  char cmd[CMD_LEN];
  char *cmd_ptr;
  char *user = kmalloc(USER_LEN);

  static struct cmd shell_cmd[NUM_COM] = {
	{ "aalogo",   aalogo      },
	{ "clear",    _kclear     },
	{ "poweroff", poweroff    },
	{ "kmalloc",  kmalloc_try },
	{ "do_fault", do_fault    },
	{ "uname",    uname_cmd   },
	{ "printmem", printmem    },
	{ "credits",  credits     },
	{ "sleep",    sleep_cmd   },
	{ "cpuid",    cpuid 	  },
	{ "date",     date 	  },
	{ "echo",     echo 	  },
	{ "help",     help	  },
	{ "answer",   answer  },
	{ "modprobe", modprobe},
        };

  int i = 0;

  memset(user, '\0', USER_LEN);
  printf(LNG_WELCOME);
  
  do {		
    printf(LNG_USER);
    scanf ("%23s",user);
    printf(LNG_USER_R);
  } while (!strlen(user));
 
  _kclear();
  aalogo();
  printf("\n\n\n\n");
  argc=1;  
  
  for (;;)
  {
    printf("%s~# ",user);
    scanf("%254s",cmd);
        
    /* elimina eventuali spazi all'inizio del comando */
    for (i = 0, cmd_ptr = cmd; cmd[i] == ' '; i++, cmd_ptr++);
    
    options (cmd_ptr);
    if (strlen(argv[0]) == 0)
        goto end;

    for (i = NUM_COM; i >= 0; --i) {
        if(strcmp(argv[0], shell_cmd[i].cmdname) == NULL) {
            (*shell_cmd[i].h_func)();
            break;
        }
    }
    if (i<0)
      printf(LNG_UNKNOWN_CMD " %s\n", argv[0]);

end:
    memset(cmd, 0, strlen(cmd));
    for (--argc; argc>=0; argc--) {      
        free (argv[argc]);
    }
  }
}

