/*
 * Dreamos
 * mouse.c
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

#include "mouse.h"
#include "video.h"
#include "port_io.h"
#include "isr.h"

static uint8_t mouse_cycle = 0;
static int8_t mouse_bytes[3];
static int32_t mouse_x = (800 / 2);
static int32_t mouse_y = (600 / 2);

void mouse_install()
{
    // Enable the auxiliary mouse device.
    mouse_waitcmd(1);
    outportb(0x64, 0xA8);

    // Enable the interrupts.
    mouse_waitcmd(1);
    outportb(0x64, 0x20);
    mouse_waitcmd(0);
    uint8_t status_byte = (inportb(0x60) | 2);
    mouse_waitcmd(1);
    outportb(0x64, 0x60);
    mouse_waitcmd(1);
    outportb(0x60, status_byte);

    // Tell the mouse to use default settings
    mouse_write(MOUSE_USE_DEFAULT_SETTINGS);
    // Acknowledge.
    mouse_read();

    // Setup the mouse handler.
    pic8259_irq_install_handler(MOUSE, mouse_isr);

    mouse_enable();
}

void mouse_enable()
{
    // Enable the mouse interrupts.
    pic8259_irq_enable(MOUSE);
    // Disable the mouse.
    mouse_write(MOUSE_ENABLE_PACKET);
    // Acknowledge.
    mouse_read();
}

void mouse_disable()
{
    // Disable the mouse interrupts.
    pic8259_irq_disable(MOUSE);
    // Disable the mouse.
    mouse_write(MOUSE_DISABLE_PACKET);
    // Acknowledge.
    mouse_read();
}

void mouse_waitcmd(unsigned char type)
{
    register unsigned int _time_out = 100000;
    if (type == 0)
    {
        while (_time_out--) // DATA
        {
            if ((inportb(0x64) & 1) == 1)
            {
                return;
            }
        }
        return;
    }
    else
    {
        while (_time_out--) // SIGNALS
        {
            if ((inportb(0x64) & 2) == 0)
            {
                return;
            }
        }
        return;
    }
}

void mouse_write(unsigned char data)
{
    mouse_waitcmd(1);
    outportb(0x64, 0xD4);
    mouse_waitcmd(1);
    outportb(0x60, data);
}

unsigned char mouse_read()
{
    mouse_waitcmd(0);
    return inportb(0x60);
}

void mouse_isr()
{
    // Get the input bytes.
    mouse_bytes[mouse_cycle++] = (char) inportb(0x60);
    if (mouse_cycle == 3)
    {
        // Reset the mouse cycle.
        mouse_cycle = 0;
        // ----------------------------
        // Get the X coordinates.
        // ----------------------------
        if ((mouse_bytes[0] & 0x40) == 0)
        {
            // Bit number 4 of the first byte (value 0x10) indicates that
            // delta X (the 2nd byte) is a negative number, if it is set.
            if ((mouse_bytes[0] & 0x10) == 0)
            {
                mouse_x -= mouse_bytes[1];
            }
            else
            {
                mouse_x += mouse_bytes[1];
            }
        }
        else
        {
            // Overflow.
            mouse_x += mouse_bytes[1] / 2;
        }
        // ----------------------------
        // Get the Y coordinates.
        // ----------------------------
        if ((mouse_bytes[0] & 0x80) == 0)
        {
            // Bit number 5 of the first byte (value 0x20) indicates that
            // delta Y (the 3rd byte) is a negative number, if it is set.
            if ((mouse_bytes[0] & 0x20) == 0)
            {
                mouse_y -= mouse_bytes[2];
            }
            else
            {
                mouse_y += mouse_bytes[2];
            }
        }
        else
        {
            // Overflow.
            mouse_y -= mouse_bytes[2] / 2;
        }
        // ----------------------------
        // Apply cursor constraint (800x600).
        // ----------------------------
        if (mouse_x <= 0)
        {
            mouse_x = 0;
        }
        else if (mouse_x >= (800 - 16))
        {
            mouse_x = 800 - 16;
        }
        if (mouse_y <= 0)
        {
            mouse_y = 0;
        }
        else if (mouse_y >= (600 - 24))
        {
            mouse_y = 600 - 24;
        }
        // Print the position.
//        dbg_print("\rX: %d | Y: %d\n", mouse_x, mouse_y);

        // Move the cursor.
//        video_set_cursor(mouse_x, mouse_y);

        // Qui si rilevato un problema, se il mouse si muove
        // vengono rilevati tasti premuti..
        // Rilevo i tasti premuti..
        // Centrale premuto.
        if ((mouse_bytes[0] & 0x04) == 0)
        {
//            dbg_print(LNG_MOUSE_MID);
        }
        // Destro premuto.
        if ((mouse_bytes[0] & 0x02) == 0)
        {
//            dbg_print(LNG_MOUSE_RIGHT);
        }
        // Sinistro premuto
        if ((mouse_bytes[0] & 0x01) == 0)
        {
//            dbg_print(LNG_MOUSE_LEFT);
        }
    }
} //End

