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

// ------------------------------------
// Special keys
// ------------------------------------
#define CODE_BREAK      0x80

#define KEY_ESCAPE      0x1
#define KEY_SPACE       0x39
#define KEY_ENTER       0x1c
#define KEY_BACKSPACE   0xe
#define KEY_TAB         0xf
#define KEY_UPARROW     0x48
#define KEY_DOWNARROW   0x50
#define KEY_LEFTARROW   0x4B
#define KEY_RIGHTARROW  0x4D
#define KEY_PGUP        0x49
#define KEY_PGDOWN      0x51
#define EXTENDED        0xE0
#define EXTENDED_2      0x2A

#define KEY_LEFT_SHIFT  0x2a
#define KEY_RIGHT_SHIFT 0x36
#define KEY_CTRL        0x1d
#define KEY_ALT         0x38
#define KEY_ALTGR       0x39

#define NUM_LED         0x45
#define SCROLL_LED      0x46
#define CAPS_LED        0x3a

#define BUFSIZE         256


void keyboard_install();

/// @brief The keyboard handler.
void keyboard_isr();

/// @brief Enable the keyboard.
void keyboard_enable();

/// @brief Disable the keyboard.
void keyboard_disable();

/// @brief Leds handler.
void keyboard_update_leds();

/// @brief Get a char from the buffer.
/// @details It loops until there is something new to read.
/// @return The read character.
int keyboard_getc();

/// @brief Set Keyboard echo Shadow
/// @author Ivan Gualandri
/// @version 1.0
/// @param value 1 if you want enable shadow 0 otherwise
void keyboard_set_shadow(const bool_t value);

/// @brief Get Keyboard Shadow information
/// @author Ivan Gualandri
/// @version 1.0
bool_t keyboard_get_shadow();

/// @brief Get ctrl status.
bool_t keyboard_is_ctrl_pressed();

/// @brief Get shift status.
bool_t keyboard_is_shifted();

/// @brief Get tab status.
bool_t keyboard_is_tab_pressed();

/// @brief Get num status.
bool_t keyboard_is_num_pressed();

/// @brief Get scroll status.
bool_t keyboard_is_scroll_pressed();

#endif
