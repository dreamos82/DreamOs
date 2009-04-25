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

/* Special keys */
#define CODE_BREAK      0x80

#define KEY_ESCAPE     0x1
#define KEY_SPACE      0x39
#define KEY_ENTER      0x1c
#define KEY_BACKSPACE  0xe
#define KEY_TAB        0xf
#define KEY_UPARROW    0x48
#define KEY_DOWNARROW  0x50
#define KEY_PGUP       0x49
#define KEY_PGDOWN     0x51
#define EXTENDED       0xE0
#define EXTENDED_2     0x2A


#define KEY_LSHIFT     0x2a
#define KEY_RSHIFT     0x36
#define KEY_CTRL       0x1d
#define KEY_ALT        0x38

#define NUM_LED        0x45
#define SCROLL_LED     0x46
#define CAPS_LED       0x3a

#define BUFSIZE        256

/* Procedures */
void keyboard_isr (void);
void keyboard_enable (void);
void keyboard_disable (void);
void _ksetleds (int, int, int);
int  _kgetch (void);

#endif
