/// @file   shell_history.c
/// @date   apr 01 2017

#include "shell_history.h"
#include "keyboard.h"
#include "string.h"
#include "video.h"
#include "stdio.h"

#define RESET_MAX(A)   A = HST_LEN - 1
#define RESET_MIN(A)   A = free_slots

/// The command history.
char cmd_history[HST_LEN][CMD_LEN];
/// The number of free slots on the history.
int free_slots = HST_LEN;
/// The position inside the history.
int pos = HST_LEN - 1;
/// Index of history array, where we save the command
int write_index = HST_LEN - 1;

void history_init(void)
{
    int it;
    for (it = 0; it < HST_LEN; it++)
    {
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
        video_delete_last_character();
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

void history_print(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
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
