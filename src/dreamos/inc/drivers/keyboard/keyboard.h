/// @file   keyboard.h
/// @brief  Definitions about the keyboard.
/// @author shainer <shainer@debianclan.org>
/// @date   Mar 31 2007
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

#include "stdbool.h"
#include "stdint.h"

/// @brief The dimension of the circular buffer used to store video history.
#define BUFSIZE         256

/// @brief Function used to install the keyboard.
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
