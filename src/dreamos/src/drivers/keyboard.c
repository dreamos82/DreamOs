/***************************************************************************
 *            keyboard.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Keyboard handling
 * ***************************************************************************/

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

#include <keyboard.h>
#include <video.h>
#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <pic8259.h>
#include <shell.h>

/// A macro from Ivan to update buffer indexes.
#define STEP(x) ((x) == BUFSIZE-1 ? 0 : x+1)

/// ITALIAN ASCII TABLE
static int key_it_map[] =
    {
        -1, 27, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48,         //11
        39, 141, 8, 9, 113, 119, 101, 114, 116, 121, 117, 105,  //23
        111, 112, 138, 43, 13, -1, 97, 115, 100, 102, 103, 104, //35
        106, 107, 108, 149, 133, 92, -1, 151, 122, 120, 99, 118,//47
        98, 110, 109, 44, 46, 45, -1, 42, -1, 32, -1, -1,       //59
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 55,         //71
        56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46,         //83
        -1, -1, 60, -1, -1                                      //88
    };

/// ITALIAN ALTERNATIVE ASCII TABLE
static int shifted_it_map[] =
    {
        -1, -1, 33, 34, 156, 36, 37, 38, 47, 40, 41, 61,    //11
        63, 94, -1, -1, 81, 87, 69, 82, 84, 89, 85, 73,     //23
        79, 80, 130, 42, -1, -1, 65, 83, 68, 70, 71, 72,    //35
        74, 75, 76, 128, 167, 124, -1, -1, 90, 88, 67, 86,  //47
        66, 78, 77, 59, 58, 95, -1, -1, -1, 32, -1, -1,     //59
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     //71
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     //83
        -1, -1, 62, -1, -1                                  //88
    };

/// Circular Buffer where the pressed keys are stored.
static int circular_buffer[BUFSIZE];
///
static int buf_r = 0;
///
static int buf_w = 0;
/// The keyboard map currently used.
static int * current_map = key_it_map;
/// Tracks the state of the leds.
static uint8_t ledstate = 0;
/// Determines if CRTL is pressed.
static bool_t is_ctrl_pressed;
/// Determines if SHIFT is pressed.
static bool_t is_shifted;
/// Determines if TAB is pressed.
static bool_t is_tab_pressed;
/// Determines if BLOC_NUM is pressed.
static bool_t is_num_pressed;
/// Determines if BLOC_SCROLL is pressed.
static bool_t is_scroll_pressed;
/// The shadow option is active.
static bool_t shadow = false;

extern unsigned int last_tab;

void keyboard_isr()
{
    // Take scancode from the port.
    int scan_code = inportb(0x60);
    // Error handling.
    if (scan_code == 0x00 || scan_code == 0xFF)
    {
        _kputs("Keyboard error\n");
        goto end;
    }

    // The right map is selected.
    if ((is_shifted && is_tab_pressed) || (!is_shifted && !is_tab_pressed))
    {
        current_map = key_it_map;
    }
    else
    {
        current_map = shifted_it_map;
    }

    // In case of useless break codes, switch controls.
    if ((scan_code > CODE_BREAK) &&
        (scan_code != (KEY_LEFT_SHIFT | CODE_BREAK)) &&
        (scan_code != (KEY_RIGHT_SHIFT | CODE_BREAK)) &&
        (scan_code != (KEY_CTRL | CODE_BREAK)))
    {
        if (scan_code == KEY_ENTER + 128)
        {
            outportb(MASTER_PORT, EOI);
        }
        goto end;
    }

    // Parse the key.
    switch (scan_code)
    {
        case KEY_LEFT_SHIFT:
        case KEY_RIGHT_SHIFT:
            is_shifted = true;
            break;
        case KEY_LEFT_SHIFT | CODE_BREAK:
        case KEY_RIGHT_SHIFT | CODE_BREAK:
            is_shifted = false;
            break;
        case KEY_PGDOWN:
            _kscrolldown();
            break;
        case KEY_PGUP:
            _kscrollup();
            break;
        case CAPS_LED:
            // Switch the state of the tab pressed.
            is_tab_pressed = (is_tab_pressed) ? false : true;
            // Update the led state.
            keyboard_update_leds();
            break;
        case NUM_LED:
            // Switch the state of the num pressed.
            is_num_pressed = (is_num_pressed) ? false : true;
            // Update the led state.
            keyboard_update_leds();
            break;
        case SCROLL_LED:
            // Switch the state of the scroll pressed.
            is_scroll_pressed = (is_scroll_pressed) ? false : true;
            // Update the led state.
            keyboard_update_leds();
            break;
        case KEY_ESCAPE:
            break;
        case KEY_BACKSPACE:
            if (STEP(buf_w) == buf_r)
            {
                buf_r = STEP(buf_r);
            }
            circular_buffer[buf_w] = '\b';
            buf_w = STEP(buf_w);
            _kbackspace();
            _ksetcursauto();
            break;
        case KEY_ENTER:
            if (STEP(buf_w) == buf_r)
            {
                buf_r = STEP(buf_r);
            }
            circular_buffer[buf_w] = '\n';
            buf_w = STEP(buf_w);
            _knewline();
            _ksetcursauto();
            last_tab = 0;
            outportb(MASTER_PORT, EOI);
            break;
        case KEY_TAB:
            if (STEP(buf_w) == buf_r)
            {
                buf_r = STEP(buf_r);
            }
            circular_buffer[buf_w] = '\t';
            buf_w = STEP(buf_w);
            _ktab();
            _ksetcursauto();
            last_tab++;
            break;
        case KEY_UPARROW:
            history_start(scan_code);
            _ksetcursauto();
            break;
        case KEY_DOWNARROW:
            history_start(scan_code);
            _ksetcursauto();
            break;
        case KEY_LEFTARROW:
//            _kgoto((_kgetcolumn() - 1), (_kgetline()));
//            move_cursor_left();
            break;
        case KEY_RIGHTARROW:
//            _kgoto((_kgetcolumn() + 1), (_kgetline()));
//            move_cursor_right();
            break;
        case KEY_ALT:
            // Presente un bug qui che non permette il fix dei relativi tasti
            // se si decommenta, il sistema all'avvio va in panic e si riavvia
            //_kputs("Alt key pressed, nothing to be done\n");
            break;
            /*case KEY_ALTGR:
            break;*/
        case KEY_CTRL:
            is_ctrl_pressed = true;
            break;
        case KEY_CTRL | CODE_BREAK:
            is_ctrl_pressed = false;
            break;
        default:
            // Print numbers if the tab is pressed.
            if (isdigit(key_it_map[scan_code]) && is_tab_pressed)
            {
                current_map = key_it_map;
            }
            if (!shadow)
            {
                putchar(current_map[scan_code]);
            }
            // Update buffer.
            if (STEP(buf_w) == buf_r)
            {
                buf_r = STEP(buf_r);
            }
            circular_buffer[buf_w] = current_map[scan_code];
            buf_w = STEP(buf_w);
    }

    end:
    /* Send acknowledge */
    //printf ("Prego");
    outportb(MASTER_PORT, EOI);
    return;
}

void keyboard_update_leds()
{
    // Handle scroll_loc & num_loc & caps_loc.
    (is_scroll_pressed) ? (ledstate |= 1) : (ledstate ^= 1);
    (is_num_pressed) ? (ledstate |= 2) : (ledstate ^= 2);
    (is_tab_pressed) ? (ledstate |= 4) : (ledstate ^= 4);
    // Write on the port.
    outportb(0x60, 0xED);
    outportb(0x60, ledstate);
}

void keyboard_enable()
{
    outportb(0x60, 0xF4);
}

void keyboard_disable()
{
    outportb(0x60, 0xF5);
}

int keyboard_getc(void)
{
    int c = -1;
    if (buf_r != buf_w)
    {
        c = circular_buffer[buf_r];
        buf_r = STEP(buf_r);
    }
    return c;
}

void keyboard_set_shadow(const bool_t value)
{
    shadow = value;
}

bool_t keyboard_get_shadow()
{
    return shadow;
}

bool_t keyboard_is_ctrl_pressed()
{
    return is_ctrl_pressed;
}

bool_t keyboard_is_shifted()
{
    return is_shifted;
}

bool_t keyboard_is_tab_pressed()
{
    return is_tab_pressed;
}

bool_t keyboard_is_num_pressed()
{
    return is_num_pressed;
}

bool_t keyboard_is_scroll_pressed()
{
    return is_scroll_pressed;
}