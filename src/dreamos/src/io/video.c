/***************************************************************************
 *            video.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  sgurtz
 *  Email
 *  Scrolling features and hexadecimal support added by shainer
 *  _kgetline() and _kprintOK developed by ^Inuyasha^
 *  Thanks to Schizzobau
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

#include <video.h>
#include <debug.h>
#include <stdio.h>

#define SCREEN_HEIGHT   25
#define SCREEN_WIDTH    80

/// @brief Structure used to hold information about the screen.
typedef struct screen_t
{
    /// The width of the screen.
    uint32_t width;
    /// The height of the screen.
    uint32_t height;
    /// Pointer to the memory of the screen.
    char * memory;
    /// Pointer to a position of the screen.
    char * pointer;
    /// The current color.
    char color;
    /// The buffer used when scrolling upward.
    char upbuffer[SCREEN_HEIGHT][SCREEN_WIDTH * 2];
    /// The buffer used when scrolling downward.
    char downbuffer[SCREEN_HEIGHT][SCREEN_WIDTH * 2];
} screen_t;

/// The information concerning the screen.
screen_t screen;

int is_scrolled = 0;
int is_shifted_once = 0;
unsigned int last_tab = 0;
int last_x = 0;
int last_y = 0;

void video_init()
{
    // --------------------------------
    screen.height = 25;
    screen.width = 80;
    screen.memory = (char *) 0xb8000;
    screen.pointer = (char *) 0xb8000;
    screen.color = GREY;
    // --------------------------------
    _kclear();
}

void video_putc(int c)
{
    // If we are at the end of the line and at the end of the column, scroll
    // down the page.
    if (last_x && last_y)
    {
        _kscrolldown();
    }
    // If the character is '\n' go the new line.
    if (c == '\n')
    {
        _knewline();
    }
    else if (c == '\033')
    {
        __asm__("nop");
    }
    else if (c == '\t')
    {
        _ktab();
    }
    else if (c == '\b')
    {
        video_delete_last_character();
    }
    else if (c == '\r')
    {
        _kminline();
    }
    else
    {
        *(screen.pointer++) = (char) c;
        *(screen.pointer++) = screen.color;
    }
    _kshiftAll();
    _ksetcursauto();
}

void video_puts(const char * str)
{
    while ((*str) != 0)
    {
        if ((*str) == '\n')
        {
            _knewline();
        }
        else if ((*str) == '\033')
        {
            video_set_color(*(++str));
        }
        else if ((*str) == '\t')
        {
            _ktab();
        }
        else if ((*str) == '\b')
        {
            video_delete_last_character();
        }
        else if ((*str) == '\r')
        {
            _kminline();
        }
        else
        {
            video_putc((*str));
        }
        ++str;
    }
}

void video_set_color(const video_color_t color)
{
    screen.color = color;
}

void video_delete_last_character()
{
    if (last_tab)
    {
        unsigned int i;
        for (i = 0; i < 7; i++)
        {
            screen.pointer -= 2;
            *screen.pointer = 0x20;
        }
        last_tab--;
    }
    else if (lower_bound_y != _kgetline() || lower_bound_x < _kgetcolumn())
    {
        screen.pointer -= 2;
        *screen.pointer = 0x20; // delete the character
    }
}

/*
 * Pression of the TAB key
 */
void _ktab()
{
    screen.pointer = screen.pointer + (7 * 2);
}

/*
 * Move the cursor at the position x, y on the screen
 */
void _kgoto(int x, int y)
{
    screen.pointer = screen.memory + ((y * screen.width * 2) + (x * 2));
    _ksetcursauto();
}

/*
 * Needed to clear the screen
 */
void _kclear()
{
    screen.pointer = screen.memory;
    for (uint32_t y = 0; y < screen.height; y++)
    {
        for (uint32_t x = 0; x < screen.width; x++)
        {
            *(screen.pointer++) = ' ';
            *(screen.pointer++) = 0x7;
        }
    }
    screen.pointer = screen.memory;
}

/*
 * Move to the following line (the effect of \n character)
 */
void _knewline()
{
    screen.pointer = screen.memory +
                     ((((screen.pointer - screen.memory) / (screen.width * 2)) +
                       1) *
                      (screen.width * 2));
    _kshiftAll();
    _ksetcursauto();
}

/*
 * Move to the up line (the effect of \n character)
 */
void _kminline()
{
    screen.pointer = screen.memory +
                     ((((screen.pointer - screen.memory) / (screen.width * 2)) -
                       1) *
                      (screen.width * 2));
    _knewline();
    _kshiftAll();
    _ksetcursauto();
}

/*
 * Move the cursor to the correct position
 */
void video_set_cursor(const unsigned int x, const unsigned int y)
{
    unsigned short position = (x * 80) + y;
    // cursor LOW port to vga INDEX register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char) (position & 0xFF));
    // cursor HIGH port to vga INDEX register
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char) ((position >> 8) & 0xFF));
}

/*
 * When something is written in another position, update the cursor
 */
void _ksetcursauto()
{
    long x = ((screen.pointer - screen.memory) / 2) / screen.width;
    long y = ((screen.pointer - screen.memory) / 2) % screen.width;
    if (x < 0)
    {
        dbg_print("Negative x while setting auto-cursor.\n");
        x = 0;
    }
    if (y < 0)
    {
        dbg_print("Negative x while setting auto-cursor.\n");
        y = 0;
    }
    video_set_cursor((uint32_t) x, (uint32_t) y);
}

/*
 * Print [OK] at the current row and column 60
 */
void _kprintOK()
{
    _kgoto(60, _kgetline());
    video_puts("\033\001[\033\012OK\033\001]\033\007\n");
    video_set_color(WHITE);
}

/*
 * Get the current column number
 */
int _kgetcolumn()
{
    long column = ((screen.pointer - screen.memory) % (screen.width * 2)) / 2;
    if (column < 0)
    {
        dbg_print("Get negative value while getting video column.\n");
        column = 0;
    }
    return column;
}

/*
 * Get the current row number
 */
int _kgetline()
{
    long line = ((screen.pointer - screen.memory) / (screen.width * 2));
    if (line < 0)
    {
        dbg_print("Get negative value while getting video line.\n");
        line = 0;
    }
    return line;
}

void _knntos(char * buffer, int num, int base)
{
    //int numval;
    char * p, * pbase;

    p = pbase = buffer;

    if (num < 0)
    {
        num = (~num) + 1;
        *p++ = '-';
        pbase++;
    }
    while (num > 0)
    {
        *p++ = (char) ('0' + (num % base));
        num = num / base;
    }

    *p-- = 0;
    while (p > pbase)
    {
        char tmp;
        tmp = *p;
        *p = *pbase;
        *pbase = tmp;

        p--;
        pbase++;
    }
}

/***************************************
 *                                     *
 * Functions regarding video scrolling *
 *                                     *
 **************************************/


/*
 * First the simplest one: when the cursor reaches the last position of the
 * screen, the whole screen is shifted up by one line
 */
void _kshiftAll(void)
{
    char * i;
    if (screen.pointer >= screen.memory + ((screen.height) * screen.width * 2))
    {
        // We save the line to be lost in a buffer, this will be useful for
        // scrolling.
        _krotate_buffer();
        uint32_t index;
        for (index = 0; index < screen.width * 2; index++)
        {
            screen.upbuffer[screen.height - 1][index] = *(screen.memory +
                                                          index);
        }
        for (i = screen.memory;
             i <= (screen.memory + ((screen.height) * screen.width * 2) +
                   (screen.width * 2)); i++)
        {
            *i = i[screen.width * 2];
        }
        screen.pointer = screen.memory +
                         ((((screen.pointer - screen.memory) /
                            (screen.width * 2)) -
                           1) *
                          (screen.width * 2));
        is_shifted_once = 1;
    }
}

/*
 * The scrolling buffer is updated to contain the screen up the current one
 * The oldest line is lost to make space for the new one
 */
void _krotate_buffer()
{
    uint32_t x;
    uint32_t y;
    for (y = 1; y < screen.height; y++)
    {
        for (x = 0; x < screen.width * 2; x++)
        {
            screen.upbuffer[y - 1][x] = screen.upbuffer[y][x];
        }
    }
}


/*
 * Called by the pression of the PAGEUP key
 * The screen up the current one is printed and the current one is saved in
 * downbuffer for being restored in future
 */
void _kscrollup()
{
    char * ptr = screen.memory;

    if (is_scrolled == 1 || is_shifted_once == 0)
    {
        return;
    }

    uint32_t x;
    uint32_t y;
    for (y = 0; y < screen.height; y++)
    {
        for (x = 0; x < screen.width * 2; x++)
        {
            screen.downbuffer[y][x] = *ptr;
            *ptr++ = screen.upbuffer[y][x];
        }
    }
    is_scrolled = 1;
    last_x = _kgetcolumn();
    last_y = _kgetline();
    _kgoto(screen.width, screen.height);
}

/*
 * Called by the pression of the PAGEDOWN key
 * The content of downbuffer (that is, the screen present when you pressed
 * PAGEUP) is printed again
 */
void _kscrolldown()
{
    char * ptr = screen.memory;

    /*
     * If PAGEUP hasn't been pressed, it's useless to go down, there is nothing
     */
    if (is_scrolled == 0)
    {
        return;
    }

    uint32_t x;
    uint32_t y;
    for (y = 0; y < screen.height; y++)
    {
        for (x = 0; x < screen.width * 2; x++)
        {
            *ptr++ = screen.downbuffer[y][x];
        }
    }
    is_scrolled = 0;
    _kgoto(last_x, last_y);
    last_x = 0;
    last_y = 0;
}

/* EOF */
