/***************************************************************************
 *            kernel.h
 *
 *  Sat Mar 31 07:45:13 2007
 *  Copyright  2007  Ivan Gualandri
 *  Email
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
 
#ifndef _KERNEL_H
#define _KERNEL_H

#define asmlinkage __attribute__((regparm(0)))
asmlinkage void _start(struct multiboot_info*);
int main_loop(struct multiboot_info*);

#define DREAMOS_VER "DreamOS ver 0.1.2 - trunk"
#define SITEURL "www.dreamos.org"

#endif
