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

#include <timer.h>
#include <stdint.h>
#include <pic8259.h>
#include <port_io.h>
#include "irqflags.h"

/// This will keep track of how many ticks the system has been running for.
__volatile__ uint32_t timer_ticks = 0;
/// This will keep track of how many seconds the system has been running for.
__volatile__ uint32_t timer_seconds = 0;
/// The number of ticks for a second.
uint32_t ticks_seconds = 100;

void timer_phase(const uint32_t hz)
{
    uint32_t divisor = PIT_DIVISOR / hz;       // Calculate our divisor.
    // Disable the IRQs.
    irq_disable();
    outportb(PIT_COMREG, 0x36);             // Set our command byte 0x36.
    outportb(PIT_DATAREG0, divisor & 0xFF);   // Set low byte of divisor.
    outportb(PIT_DATAREG0, divisor >> 8);     // Set high byte of divisor.
    // Re-Enable the IRQs.
    irq_enable();
}

void timer_handler()
{
    // Increment our 'tick count'.
    ++timer_ticks;
    // Check if a second has passed.
    if (timer_ticks % ticks_seconds == 0)
    {
        ++timer_seconds;
    }
}

void timer_install()
{
    // Set the timer phase.
    timer_phase(100);
    // Installs 'timer_handler' to IRQ0.
    pic8259_irq_install_handler(TIMER, timer_handler);
    // Enable the IRQ of the itemer.
    pic8259_irq_enable(TIMER);
}

void sleep(const unsigned int seconds)
{
    uint32_t future_time = timer_seconds + seconds;
    while (timer_seconds < future_time);
}

