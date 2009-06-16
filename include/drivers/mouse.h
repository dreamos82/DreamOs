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

#define MOUSE_ENABLE_PACKET 0xF4
#define MOUSE_DISABLE_PACKET 0xF5
#define MOUSE_USE_DEFAULT_SETTINGS 0xF6

/**
 * mouse_init()
 * @description inizializza il mouse 
 **/
void mouse_init();
/**
 * mouse_send_data()
 * @description manda dati al mouse
 * @param data:
 * i dati da mandare
 **/
void mouse_waitcmd(unsigned char type);

void mouse_write (unsigned char a_write);
/**
 * mouse_receive_data()
 * @description riceve dati dal mouse
 * @return i dati ricevuti
 **/
unsigned char mouse_read();
/**
 * mouse_waitcmd()
 * @description fa attendere il mouse per un comando
 * @param type:
 * 1 per mandare
 * 0 per ricevere
 **/

void mouse_dead();
void mouse_IRQhandler();
#endif /* _MOUSE_H_ */
