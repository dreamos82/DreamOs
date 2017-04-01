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

#include "commands.h"
#include <stddef.h>
#include <video.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <language.h>
#include <debug.h>
#include "kheap.h"
#include "shell_login.h"
#include "shell_history.h"

void shell_print_prompt();

void shell_get_command();

void shell_get_options(char * command, int * argc, char *** argv);

/// The current user.
userenv_t current_user;
/// The input command.
char cmd[CMD_LEN];
/// The index of the cursor.
uint32_t cmd_cursor_index;

command_t shell_cmd[MAX_NUM_COM] = {
        {"aalogo",   aalogo,        "Show an ascii art logo"},
        {"clear",    clear,         "Clear the screen"},
        {"poweroff", poweroff,      "Turn off the machine"},
        {"uname",    uname_cmd,     "Print kernel version, try uname --help for more info"},
        {"credits",  credits,       "Show DreamOS credits"},
        {"sleep",    sleep_cmd,     "Pause DreamOS for a particular number of seconds"},
        {"cpuid",    cpuid,         "Show cpu identification informations"},
        {"date",     date,          "Show date and time"},
        {"echo",     echo,          "Print some lines of text"},
        {"help",     help,          "See the 'help' list to learn the DreamOS commands now available"},
        {"drv_load", drv_load,      "Tool to load and kill drivers"},
        {"ls",       ls,            "Tool for listing dir - not complete-"},
        {"cd",       cd,            "Change dir - not complete-"},
        {"whoami",   whoami,        "Show the current user name"},
        {"tester",   tester,        "Try some functions, 'tester --help' for more info'"},
        {"pwd",      pwd,           "Print current working directory"},
        {"more",     more,          "Read content of a file"},
        {"newfile",  newfile,       "Create a new file"},
        {"ps",       ps,            "Show task list"},
        {"clear",    clear,         "Clears the screen"},
        {"showpid",  showpid,       "Shows the PID of the shell"},
        {"heapdump", heapdump,      "Shows the heap status"},
        {"history",  history_print, "Shows the shell history"}
};

/* corpo della shell */
int shell(void * args)
{
    int argc = 1;
    char ** argv;

    (void) args;
    dbg_print("\nNewShell\n");
    int i = 0;
    video_set_color(BRIGHT_BLUE);
    printf(LNG_WELCOME);
    video_set_color(WHITE);

    shell_login();

    memset(current_user.cur_path, '\0', CURPATH_LEN);

    video_clear();
    aalogo(1, NULL);
    printf("\n\n\n\n");

    strcpy(current_user.cur_path, "/");
    current_user.uid = 1;
    current_user.gid = 0;
    // Initialize the history.
    history_init();
    for (;;)
    {
        // First print the prompt.
        shell_print_prompt();
        // Get the input command.
        shell_get_command();
        // Cleans all blanks at the beginning of the command.
        trim(cmd);
        // Retrieve the options from the command.
        shell_get_options(cmd, &argc, &argv);
        // Check if the command is empty.
        if (strlen(cmd))
        {
            // Add the command to the history.
            history_push(cmd);
            // Matching and executing the command.
            for (i = MAX_NUM_COM; i >= 0; --i)
            {
                // Skip commands with undefined functions.
                if (shell_cmd[i].function == NULL) continue;
                if (strcmp(argv[0], shell_cmd[i].cmdname) == (int) NULL)
                {
                    (*shell_cmd[i].function)(argc, argv);
                    break;
                }
            }
            //Print unknown command message
            if (i < 0)
            {
                printf(LNG_UNKNOWN_CMD " %s\n", argv[0]);
            }
        }
        else
        {
            for (int it = 0; it < argc; ++it)
            {
                kfree(argv[it]);
            }
        }
        memset(cmd, 0, CMD_LEN);
    }
    return 0;
}

void shell_print_prompt()
{
    video_set_color(BRIGHT_BLUE);
    printf(current_user.username);
    video_set_color(WHITE);
    printf("~:%s# ", current_user.cur_path);
    // Update the lower-bounds for the video.
    lower_bound_x = video_get_column();
    lower_bound_y = video_get_line();
}

//Input shell command (a private hacked version of gets)
void shell_get_command()
{
    // Re-Initialize the cursor index.
    cmd_cursor_index = 0;
    // Initialize the read character.
    int c = 0;
    //Initializing the current command line buffer
    memset(cmd, 0, CMD_LEN);
    // Tap the command.
    cmd[0] = '\0';
    while ((cmd_cursor_index < CMD_LEN) && ((c = getchar()) != '\n'))
    {
        // If CTRL is pressed and the current character is c, stop reading
        // the command.
        if ((keyboard_is_ctrl_pressed() == 1) && (c == 'c'))
        {
            // However, the ISR of the keyboard has already put the char.
            // Thus, delete it by using backspace.
            video_delete_last_character();
            // Re-set the index to the beginning.
            cmd_cursor_index = 0;
            // Go to the new line.
            printf("\n");
            // Break the while loop.
            break;
        }
        if (c == '\b')
        {
            if (cmd_cursor_index > 0)
            {
                cmd[--cmd_cursor_index] = 0;
            }
            else if (cmd_cursor_index == 0)
            {
                cmd[cmd_cursor_index] = 0;
            }
            continue;
        }
        cmd[cmd_cursor_index++] = (char) c;
        cmd[cmd_cursor_index] = 0;
    }
}

void shell_get_options(char * command, int * argc, char *** argv)
{
    int i = 0;
    (*argc) = 0;
    for (; *command; command++)
    {
        (*argv)[(*argc)] = (char *) kmalloc(sizeof(char) * 30);
        while (*command != ' ' && *command != '\0')
        {
            *((*argv)[(*argc)] + i) = *command++;
            i++;
        }
        *((*argv)[(*argc)] + i) = '\0';
        (*argc)++;
        i = 0;
    }
    (*argv)[(*argc)] = "\0";
}

void move_cursor_left(void)
{
    if (cmd_cursor_index > lower_bound_x)
    {
        --cmd_cursor_index;
    }
}

void move_cursor_right(void)
{
    if (cmd_cursor_index < shell_lowe_bound_x)
    {
        ++cmd_cursor_index;
    }
}
