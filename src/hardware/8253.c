/*
 * Dreamos
 * 8253.h
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

#include <8253.h>
#include <io.h>
#include <clock.h>
#include <stddef.h>

static unsigned int ticks;
static unsigned int seconds;

void timer_pit_handler()
{
}

void timer_init()
{
    int divisor = PIT_DIVISOR;

    asm ("cli");
    ticks = seconds = 0;
    outportb(PIT_COMREG, 0x37);
    outportb(PIT_DATAREG0, divisor & 0xFF);
    outportb(PIT_DATAREG0, divisor >> 8);
    asm ("sti");
}

unsigned int sleep(const unsigned int seconds)
{
    time_t current_time = time(NULL);
    time_t future_time = current_time + seconds;
    do
    {
        current_time = time(NULL);
    } while (current_time < future_time);
    return 0;
}

