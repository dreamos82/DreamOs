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
#include <stddef.h>
#include <video.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <use.h>
#include <clock.h>
#include <user_shell.h>
#include <debug.h>

#define RESET_MAX(A)   A = HST_LEN - 1
#define RESET_MIN(A)   A = free_slots

void _getCommand(char *);

void _debug_history_stack(void);

userenv_t current_user;
int hst_flag;
char cmd[CMD_LEN]; //history stack
//#define PWD_CHECK 1
struct cmd shell_cmd[MAX_NUM_COM] = {
    {"aalogo",   aalogo,    "Show an ascii art logo"},
    {"clear",    _kclear,   "Clear the screen"},
    {"poweroff", poweroff,  "Turn off the machine"},
    {"uname",    uname_cmd, "Print kernel version, try uname --help for more info"},
    {"credits",  credits,   "Show DreamOS credits"},
    {"sleep",    sleep_cmd, "Pause DreamOS for a particular number of seconds"},
    {"cpuid",    cpuid,     "Show cpu identification informations"},
    {"date",     date,      "Show date and time"},
    {"echo",     echo,      "Print some lines of text"},
    {"help",     help,      "See the 'help' list to learn the DreamOS commands now available"},
    {"answer",   answer,    "42"},
    {"drv_load", drv_load,  "Tool to load and kill drivers"},
    {"ls",       ls,        "Tool for listing dir - not complete-"},
    {"cd",       cd,        "Change dir - not complete-"},
    {"whoami",   whoami,    "Show the current user name"},
    {"tester",   tester,    "Try some functions, 'tester --help' for more info'"},
    {"pwd",      pwd,       "Print current working directory"},
    {"more",     more,      "Read content of a file"},
    {"newfile",  newfile,   "Create a new file"},
    {"ps",       ps,        "Show task list"}
};
/*
 * Inserisce gli argomenti di un comando in un array di stringhe
 * argc = numero degli argomenti
 * argv[0] = nome del comando
 * argv[n] = opzioni successive
 */

int free_slots = HST_LEN, pos = HST_LEN - 1, c = 0, limit = 1;
char * lastcmd[HST_LEN] = {};
//Index of history array, where we save the command
int write_index = HST_LEN - 1;

void options(char * com)
{
    int i = 0;
    argc = 0;
    for (; *com; com++)
    {
        argv[argc] = (char *) kmalloc(sizeof(char) * 30);
        while (*com != ' ' && *com != '\0')
        {
            *(argv[argc] + i) = *com++;
            i++;
        }
        *(argv[argc] + i) = '\0';
        argc++;
        i = 0;
    }
    argv[argc] = '\0';
}

/* corpo della shell */
int shell(void * args)
{
    dbg_bochs_print("\nNewShell\n");
    char * cmd_ptr;
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
    _kcolor(BRIGHT_BLUE);
    printf(LNG_WELCOME);
    _kcolor(WHITE);

    do
    {
        memset(cmd, '\0', CMD_LEN);
        memset(current_user.username, '\0', USER_LEN);
        memset(password, '\0', 30);
        memset(current_user.cur_path, '\0', CURPATH_LEN);
        printf(LNG_USER);
        scanf("%23s", current_user.username);
        printf(LNG_USER_R);
        #ifdef PWD_CHECK
        printf(LNG_PWD);
        set_shadow(1);
        scanf ("%23s",password);
        set_shadow(0);
        #endif
        dbg_bochs_print("Asking username");
        ret_val = user_chk(current_user.username, password);
    } while ((!strlen(current_user.username) || ret_val != 1));

    _kclear();
    aalogo();
    printf("\n\n\n\n");
    argc = 1;
    strcpy(current_user.cur_path, "/");
    current_user.uid = 1;
    current_user.gid = 0;
    //Command history set allocation
    for (c = 0; c < HST_LEN; c++)
    {
        lastcmd[c] = (char *) kmalloc(sizeof(char) * 30);
        //Initializing new allocated strings
        memset(lastcmd[c], 0, 30);
    }

    for (;;)
    {
        dbg_bochs_print("shell loop\n");
        _kcolor(9);
        printf("%s", current_user.username);
        _kcolor(15);

        _getCommand("~:%s# ");

        /* Cleans all blanks at the beginning of the command */
        for (i = 0, cmd_ptr = cmd; cmd[i] == ' '; i++, cmd_ptr++);

        // Retrieve the options from the command.
        options(cmd_ptr);

        if (strlen(cmd_ptr) > 0)
        {
            history(cmd_ptr);
        }
        else
        {
            memset(cmd_ptr, 0, CMD_LEN);
            for (--argc; argc >= 0; argc--)
            {
                free(argv[argc]);
            }
            continue;
        }

        // Matching and executing the command.
        for (i = MAX_NUM_COM; i >= 0; --i)
        {
            // Skip commands with undefined functions.
            if (shell_cmd[i].h_func == NULL) continue;
            if (strcmp(argv[0], shell_cmd[i].cmdname) == (int) NULL)
            {
                (*shell_cmd[i].h_func)();
                break;
            }
        }
        //Print unknown command message
        if (i < 0)
            printf(LNG_UNKNOWN_CMD " %s\n", argv[0]);
    }

    return 0;
}

// Saves cmd_pass string to history buffer (lastcmd)
void history(char * cmd_pass)
{
    int prev_index = write_index + 1;

    if (prev_index > HST_LEN - 1)
        prev_index = free_slots;

    //We should always clean before copying data inside
    memset(lastcmd[write_index], 0, 30);
    strcpy(lastcmd[write_index], cmd_pass);
    pos = write_index;

    --write_index;
    if (write_index < 0)
        RESET_MAX(write_index);

    if (free_slots > 0)
        --free_slots;

    #ifdef DEBUG
    _debug_history_stack();
    #endif
}

//downarrow and uparrow keys handler to get commands from history buffer
void history_start(const int key)
{
    int delete = 0, max_limit = strlen(cmd);
    //Backspace handling
    if (limit < max_limit)
    {
        limit = max_limit;
    }

    while (delete <= limit)
    {
        _kbackspace();
        delete++;
    }

    //Showing picked history command
    printf("%s", lastcmd[pos]);
    //We copy the history command to cmd
    memset(cmd, 0, CMD_LEN);
    strcpy(cmd, lastcmd[pos]);
    hst_flag = 1;

    if (key == KEY_UPARROW)
        ++pos;
    else if (key == KEY_DOWNARROW)
        --pos;

    if (pos < free_slots)
        RESET_MAX(pos);
    else if (pos > HST_LEN - 1)
        RESET_MIN(pos);
}

//Input shell command (a private hacked version of gets)
void _getCommand(char * prompt)
{
    int i, c;

    i = c = 0;
    //Initializing the current command line buffer
    memset(cmd, 0, CMD_LEN);
    //Input command
    printf(prompt, current_user.cur_path);

    //Important to update these values otherwise backspace will not work!!
    shell_mess_col = _kgetcolumn();
    shell_mess_line = _kgetline();

    cmd[0] = '\0';
    while (i < CMD_LEN && (c = getchar()) != '\n')
    {
        if (hst_flag)
        {
            hst_flag = 0;
            //We have to write new chars at the end of string imported from history
            i = strlen(cmd);
        }
        if (c == '\b')
        {
            if (i > 0)
                cmd[--i] = '\0';
            else if (i == 0)
                cmd[i] = '\0';
            continue;
        }
        cmd[i++] = c;
        cmd[i] = '\0';
    }
}

void _debug_history_stack(void)
{
    //Prints the history stack with current indexes values
    int i;
    printf("\n------------------------------\n");
    printf("        Debug history\n");
    printf("------------------------------\n");
    for (i = 0; i < HST_LEN; ++i)
    {
        if (i == write_index)
            printf("w"); //write_index value is here
        else if (i == pos)
            printf("->"); //pos value is here

        printf("\tHistory[%d]: %s", i, lastcmd[i]);
        putchar('\n');
    }
    printf("#free slots: %d\n", free_slots);
}
