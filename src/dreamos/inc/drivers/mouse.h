/*
 * Dreamos
 * mouse.h
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
  * Driver for *PS2* Mouses.
  * Authors DT, Osiris
  * Contributor: finarfin 
  * first version: 16/06/2009
  */

#ifndef _MOUSE_H_
#define _MOUSE_H_

/// The mouse starts sending automatic packets when the mouse moves or is
/// clicked.
#define MOUSE_ENABLE_PACKET         0xF4
/// The mouse stops sending automatic packets.
#define MOUSE_DISABLE_PACKET        0xF5
/// Disables streaming, sets the packet rate to 100 per second, and
/// resolution to 4 pixels per mm.
#define MOUSE_USE_DEFAULT_SETTINGS  0xF6

/// @brief Sets up the mouse by installing the mouse handler into IRQ12.
void mouse_install();

/// @brief Enable the mouse driver.
/// @author Ivan Gualandri, Diego Stamigni, DT
/// @version 1.0
void mouse_enable();

/// @brief Disable the mouse driver.
/// @author Ivan Gualandri, Diego Stamigni, DT
/// @version 1.0
void mouse_disable();

/// @brief Mouse wait for a command
/// @param type 1 for sending - 0 for receiving.
void mouse_waitcmd(unsigned char type);

/// @brief Send data to mouse.
/// @param data The data to send.
void mouse_write(unsigned char data);

/// @brief Read data from mouse.
/// @return The data received from mouse.
unsigned char mouse_read();

/// @brief The mouse handler.
void mouse_isr(void);

#endif /* _MOUSE_H_ */
