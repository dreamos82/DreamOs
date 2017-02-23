/***************************************************************************
 *            fdc.h
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Definitions about the floppy
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

enum FdcRegisters
{
    STATUS_REGISTER_A = 0x3F0, // read-only
    STATUS_REGISTER_B = 0x3F1, // read-only
    DOR = 0x3F2,
    TAPE_DRIVE_REGISTER = 0x3F3,
    MAIN_STATUS_REGISTER = 0x3F4, // read-only
    DATARATE_SELECT_REGISTER = 0x3F4, // write-only
    DATA_FIFO = 0x3F5,
    DIGITAL_INPUT_REGISTER = 0x3F7, // read-only
    CONFIGURATION_CONTROL_REGISTER = 0x3F7  // write-only
};

void fdc_disable_motor(void);

void fdc_enable_motor(void);
