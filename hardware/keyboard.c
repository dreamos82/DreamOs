/*
 * Dreamos
 * keyboard.c
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 */

#include <keyboard.h>
#include <video.h>
#include <string.h>
#include <pic8259.h>
#include <io.h>

void keyboard_irq(){
    char *s;
    int keyvalue;
    keyvalue = inportb(0x60);
     if(keyvalue<0x80){
        _kputs("E' Stato premuto un tasto ");
        _kntos(s, keyvalue, 10);
        _kputs(s);
        _kputs("\n");
    }
}
