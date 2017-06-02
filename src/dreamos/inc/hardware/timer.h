/// @file   timer.h
/// @brief  Timer definitions.
/// @author shainer <shainer@debianclan.org>
/// @date   Jun 2007
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

#define PIT_COMREG      0x43
#define PIT_DATAREG0    0x40
#define PIT_DIVISOR     1193180

/// @brief Handles the timer.
/// @details In this case, it's very simple: We
/// increment the 'timer_ticks' variable every time the
/// timer fires. By default, the timer fires 18.222 times
/// per second. Why 18.222Hz? Some engineer at IBM must've
/// been smoking something funky
void timer_handler();

/// @brief Sets up the system clock by installing the timer handler into IRQ0.
void timer_install();

/// @brief Makes the process sleep for the given ammount of time.
/// @param seconds The ammount of seconds.
void sleep(const unsigned int seconds);
