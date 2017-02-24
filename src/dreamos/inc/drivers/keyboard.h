/***************************************************************************
 *            keyboard.h
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Definitions about the keyboard
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

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

/* Special keys */
#define CODE_BREAK      0x80

#define KEY_ESCAPE     0x1
#define KEY_SPACE      0x39
#define KEY_ENTER      0x1c
#define KEY_BACKSPACE  0xe
#define KEY_TAB        0xf
#define KEY_UPARROW    0x48
#define KEY_DOWNARROW  0x50
#define KEY_LEFTARROW  0x4B
#define KEY_RIGHTARROW 0x4D
#define KEY_PGUP       0x49
#define KEY_PGDOWN     0x51
#define EXTENDED       0xE0
#define EXTENDED_2     0x2A


#define KEY_LEFT_SHIFT  0x2a
#define KEY_RIGHT_SHIFT 0x36
#define KEY_CTRL        0x1d
#define KEY_ALT         0x38
#define KEY_ALTGR       0x39

#define NUM_LED        0x45
#define SCROLL_LED     0x46
#define CAPS_LED       0x3a

#define BUFSIZE        256

/// Determines if CRTL is pressed.
extern bool_t is_ctrl_pressed;
/// Determines if SHIFT is pressed.
extern bool_t is_shifted;
/// Determines if TAB is pressed.
extern bool_t is_tab_pressed;
/// Determines if BLOC_NUM is pressed.
extern bool_t is_num_pressed;
/// Determines if BLOC_SCROLL is pressed.
extern bool_t is_scroll_pressed;

void keyboard_isr(void);

void keyboard_enable(void);

void keyboard_disable(void);

void _ksetleds(const bool_t capslock,
               const bool_t numlock,
               const bool_t scrlock);

/// @brief Set Keyboard echo Shadow
/// @author Ivan Gualandri
/// @version 1.0
/// @param value 1 if you want enable shadow 0 otherwise
void set_shadow(const bool_t value);

/// @brief Get Keyboard Shadow information
/// @author Ivan Gualandri
/// @version 1.0
/// @return
bool_t get_shadow();

int _kgetch(void);

#endif
