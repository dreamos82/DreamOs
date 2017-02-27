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
#include <language.h>
#include <user_shell.h>
#include <debug.h>

#define RESET_MAX(A)   A = HST_LEN - 1
#define RESET_MIN(A)   A = free_slots

void shell_print_prompt();

void shell_get_command();

void shell_get_options(char * command);

void shell_login();

/// The current user.
userenv_t current_user;
/// The input command.
char cmd[CMD_LEN];
/// The index of the cursor.
int cmd_cursor_index;
/// The command history.
char * cmd_history[HST_LEN];
/// The number of free slots on the history.
int free_slots = HST_LEN;
/// The position inside the history.
int pos = HST_LEN - 1;
/// Index of history array, where we save the command
int write_index = HST_LEN - 1;

command_t shell_cmd[MAX_NUM_COM] = {
    {"aalogo",   aalogo,        "Show an ascii art logo"},
    {"clear",    _kclear,       "Clear the screen"},
    {"poweroff", poweroff,      "Turn off the machine"},
    {"uname",    uname_cmd,     "Print kernel version, try uname --help for more info"},
    {"credits",  credits,       "Show DreamOS credits"},
    {"sleep",    sleep_cmd,     "Pause DreamOS for a particular number of seconds"},
    {"cpuid",    cpuid,         "Show cpu identification informations"},
    {"date",     date,          "Show date and time"},
    {"echo",     echo,          "Print some lines of text"},
    {"help",     help,          "See the 'help' list to learn the DreamOS commands now available"},
    {"answer",   answer,        "42"},
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
    {"history",  history_print, "Shows the shell history"}
};

/* corpo della shell */
int shell(void * args)
{
    (void) args;
    dbg_print("\nNewShell\n");
    int i = 0;
    _kcolor(BRIGHT_BLUE);
    printf(LNG_WELCOME);
    _kcolor(WHITE);

    shell_login();

    memset(current_user.cur_path, '\0', CURPATH_LEN);

    _kclear();
    aalogo();
    printf("\n\n\n\n");
    argc = 1;
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
        shell_get_options(cmd);
        // Check if the command is empty.
        if (strlen(cmd) == 0)
        {
            memset(cmd, 0, CMD_LEN);
            for (--argc; argc >= 0; argc--)
            {
                kfree(argv[argc]);
            }
            continue;
        }
        // Add the command to the history.
        history_push(cmd);
        // Matching and executing the command.
        for (i = MAX_NUM_COM; i >= 0; --i)
        {
            // Skip commands with undefined functions.
            if (shell_cmd[i].function == NULL) continue;
            if (strcmp(argv[0], shell_cmd[i].cmdname) == (int) NULL)
            {
                (*shell_cmd[i].function)();
                break;
            }
        }
        //Print unknown command message
        if (i < 0)
            printf(LNG_UNKNOWN_CMD " %s\n", argv[0]);
    }
    return 0;
}

void shell_print_prompt()
{
    _kcolor(BRIGHT_BLUE);
    printf(current_user.username);
    _kcolor(WHITE);
    printf("~:%s# ", current_user.cur_path);
    // Update the lower-bounds for the video.
    lower_bound_x = _kgetcolumn();
    lower_bound_y = _kgetline();
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
        if ((is_ctrl_pressed == 1) && (c == 'c'))
        {
            // However, the ISR of the keyboard has already put the char.
            // Thus, delete it by using backspace.
            _kbackspace();
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

void shell_get_options(char * command)
{
    int i = 0;
    argc = 0;
    for (; *command; command++)
    {
        argv[argc] = (char *) kmalloc(sizeof(char) * 30);
        while (*command != ' ' && *command != '\0')
        {
            *(argv[argc] + i) = *command++;
            i++;
        }
        *(argv[argc] + i) = '\0';
        argc++;
        i = 0;
    }
    argv[argc] = "\0";
}

void shell_login()
{
    do
    {
        // Initialize the credentials.
        credentials_t credentials;
        init_credentials(&credentials);
        // ----------------------------
        // Ask the username.
        dbg_print("Asking the username.\n");
        printf(LNG_USER);
        // Update the lower-bounds for the video.
        lower_bound_x = _kgetcolumn();
        lower_bound_y = _kgetline();
        // Get the username.
        scanf("%50s", credentials.username);
        // ----------------------------
        // Ask the password.
        dbg_print("Asking the password.\n");
        printf(LNG_PWD);
        // Update the lower-bounds for the video.
        lower_bound_x = _kgetcolumn();
        lower_bound_y = _kgetline();
        // Set the shadow option.
        set_shadow(true);
        // Get the password.
        scanf("%50s", credentials.password);
        // Disable the shadow option.
        set_shadow(false);
        // ----------------------------
        // Check if the data are correct.
        if (check_credentials(&credentials))
        {
            strcpy(current_user.username, credentials.username);
            break;
        }
        printf(LNG_WRONG_CRED);
    } while (true);
}

void history_init(void)
{
    int it;
    for (it = 0; it < HST_LEN; it++)
    {
        cmd_history[it] = (char *) kmalloc(sizeof(char) * CMD_LEN);
        // Initializing new allocated strings.
        memset(cmd_history[it], 0, CMD_LEN);
    }
}

// Saves cmd_pass string to history buffer (lastcmd)
void history_push(char * command)
{
    int prev_index = write_index + 1;
    if (prev_index > HST_LEN - 1)
    {
        prev_index = free_slots;
    }
    // Clean the history slot.
    memset(cmd_history[write_index], 0, CMD_LEN);
    // Copy the command inside the slot.
    strcpy(cmd_history[write_index], command);
    // Increment the history position.
    pos = write_index;
    // Decrement the index.
    --write_index;
    if (write_index < 0)
    {
        RESET_MAX(write_index);
    }
    if (free_slots > 0)
    {
        --free_slots;
    }
}

void history_start(const int key)
{
    // If the history is empty do nothing.
    if (free_slots == HST_LEN) return;
    // Completely delete the current command.
    while (cmd_cursor_index > 0)
    {
        _kbackspace();
        cmd_cursor_index--;
    }
    // Print the command at the given position of the history.
    printf("%s", cmd_history[pos]);
    // Completely clear the variable which holds the command.
    memset(cmd, 0, CMD_LEN);
    // Set the variable to the retrieved command.
    strcpy(cmd, cmd_history[pos]);
    // Update the position inside the history.
    if (key == KEY_UPARROW)
    {
        ++pos;
    }
    else if (key == KEY_DOWNARROW)
    {
        --pos;
    }
    if (pos < free_slots)
    {
        RESET_MAX(pos);
    }
    else if (pos > HST_LEN - 1)
    {
        RESET_MIN(pos);
    }
    // Set the cursor index to the end of the current command.
    cmd_cursor_index = strlen(cmd);
}

void history_print(void)
{
    //Prints the history stack with current indexes values
    int i;
    printf("\n");
    printf("------------------------------\n");
    printf("        Debug history         \n");
    printf("------------------------------\n");
    for (i = 0; i < HST_LEN; ++i)
    {
        if (i == write_index) printf("w "); //write_index value is here
        else if (i == pos) printf("->");    //pos value is here
        else printf("  ");
        printf("\tHistory[%d]: %s\n", i, cmd_history[i]);
    }
    printf("#free slots: %d\n", free_slots);
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
