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
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
} video_color_t;

/// @brief A rectangle described by a pair of coordinates and a size.
typedef struct rectangle_t
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} rectangle_t;

/// @brief Initialize the video.
void video_init();

/// @brief Print the given character on the screen.
void video_putc(int);

/// @brief Prints the given string on the screen.
void video_puts(const char * str);

/// @brief Change text colour.
void video_set_color(const video_color_t color);

/// @brief Deletes the last inserted character.
void video_delete_last_character();

void video_set_cursor(const unsigned int x, const unsigned int y);

void _kgoto(int, int);

void _ksetcursauto();

void _kshiftAll();

void _ktab();

void _kclear();

void _knewline();

void _kminline();

void _kprintOK();

int _kgetline();

int _kgetcolumn();

void _krotate_buffer();

void _kscrollup();

void _kscrolldown();

void _knntos(char *, int, int);

/// Determines the lower-bound on the x axis for the video.
int32_t lower_bound_x;
/// Determines the lower-bound on the y axis for the video.
int32_t lower_bound_y;

int32_t shell_current_x;
int32_t shell_current_y;
int32_t shell_lowe_bound_x;
int32_t shell_lowe_bound_y;

#endif /* _VIDEO_H */
