/***************************************************************************
 *            fdc.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Floppy driver controller handling
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

#include <fdc.h>
#include <video.h>
#include <stdio.h>
#include <ctype.h>
#include <port_io.h>

void fdc_disable_motor()
{
    /* Setting bits:
     * 2: (RESET)
     * 3: (IRQ)
     */
    outportb(DOR, 0x0c);
}

void fdc_enable_motor()
{
    /* Setting bits:
     * 3: (IRQ)
     * 4: (MOTA)
     */
    outportb(DOR, 0x18);
}