/***************************************************************************
 *            video.h
 *
 *  Sat Mar 31 07:57:13 2007
 *  Copyright  2007  shainer & ^Inuyasha^
 *  Video functions and costants
 ****************************************************************************/

/*
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

#ifndef _VIDEO_H
#define _VIDEO_H

#include <stdint.h>

typedef enum video_color_t
{
    BLACK,          ///< 0
    BLUE,           ///< 1
    GREEN,          ///< 2
    CYAN,           ///< 3
    RED,            ///< 4
    MAGENTA,        ///< 5
    BROWN,          ///< 6
    GREY,           ///< 7
    DARK_GREY,      ///< 8
    BRIGHT_BLUE,    ///< 9
    BRIGHT_GREEN,   ///< 10
    BRIGHT_CYAN,    ///< 11
    BRIGHT_RED,     ///< 12
    BRIGHT_MAGENTA, ///< 13
    YELLOW,         ///< 14
    WHITE,          ///< 15
} video_color_t;

/// @brief Initialize the video.
void video_init();

/// @brief Print the given character on the screen.
void video_putc(int);

/// @brief Prints the given string on the screen.
void video_puts(const char * str);

/// @brief Change foreground colour.
void video_set_color(const video_color_t foreground);

/// @brief Change background colour.
void video_set_background(const video_color_t background);

/// @brief Deletes the last inserted character.
void video_delete_last_character();

/// @brief Move the cursor to the given position.
void video_set_cursor(const unsigned int x, const unsigned int y);

/// @brief When something is written in another position, update the cursor.
void video_set_cursor_auto();

/// @brief Move the cursor at the position x, y on the screen.
void video_move_cursor(int, int);

/// @brief Prints a tab on the screen.
void video_put_tab();

/// @brief Clears the screen.
void video_clear();

/// @brief Move to the following line (the effect of \n character).
void video_new_line();

/// @brief Move to the up line (the effect of \n character).
void video_cartridge_return();

/// @brief Get the current column number.
uint32_t video_get_column();

/// @brief Get the current row number.
uint32_t video_get_line();

// -----------------------------------------------------------------------------
// Functions concerning video scrolling.

/// @brief The whole screen is shifted up by one line. Used when the cursor
/// reaches the last position of the screen.
void video_shift_one_line();

/// @brief The scrolling buffer is updated to contain the screen up the
/// current one. The oldest line is lost to make space for the new one.
void video_rotate_scroll_buffer();

/// @brief Called by the pression of the PAGEUP key.
/// The screen aboce the current one is printed and the current one is
/// saved in downbuffer, ready to be restored in future.
void video_scroll_up();

/// @brief Called by the pression of the PAGEDOWN key.
/// The content of downbuffer (that is, the screen present when you pressed
/// PAGEUP) is printed again.
void video_scroll_down();

/// Determines the lower-bound on the x axis for the video.
uint32_t lower_bound_x;
/// Determines the lower-bound on the y axis for the video.
uint32_t lower_bound_y;

uint32_t shell_current_x;
uint32_t shell_current_y;
uint32_t shell_lowe_bound_x;
uint32_t shell_lowe_bound_y;

// -----------------------------------------------------------------------------
// Support functions.
/// @brief Prints [OK] at the current row and column 60.
void video_print_ok();

/// @brief Prints [FAIL] at the current row and column 60.
void video_print_fail();

#endif /* _VIDEO_H */
