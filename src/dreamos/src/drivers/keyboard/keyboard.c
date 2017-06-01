/// @file   keyboard.c
/// @brief  Keyboard handling.
/// @author shainer <shainer@debianclan.org>
/// @date   Mar 31 2007
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "keyboard.h"
#include "video.h"
#include "stdio.h"
#include "port_io.h"
#include "pic8259.h"
#include "shell_history.h"
#include "keymap.h"
#include "bitops.h"

/// A macro from Ivan to update buffer indexes.
#define STEP(x) ((x) == BUFSIZE-1 ? 0 : x+1)

/// Circular Buffer where the pressed keys are stored.
static int circular_buffer[BUFSIZE];
///
static int buf_r = 0;
///
static int buf_w = 0;
/// Tracks the state of the leds.
static uint8_t ledstate = 0;
/// The shadow option is active.
static bool_t shadow = false;

extern unsigned int last_tab;

// ------------------------------------
#define KBD_LEFT_SHIFT      1
#define KBD_RIGHT_SHIFT     2
#define KBD_CAPS_LOCK       4
#define KBD_NUM_LOCK        8
#define KBD_SCROLL_LOCK     16
#define KBD_LEFT_CONTROL    32
#define KBD_RIGHT_CONTROL   64
static uint32_t keyboard_flags = 0;
// ------------------------------------

void keyboard_install()
{
    // Install the IRQ.
    pic8259_irq_install_handler(KEYBOARD, keyboard_isr);
    // Enable the IRq.
    pic8259_irq_enable(KEYBOARD);
}

void keyboard_isr()
{
    if (!(inportb(0x64) & 1))
    {
        return;
    }
    // Take scancode from the port.
    uint32_t scancode = inportb(0x60);
    if (scancode == 0xE0)
    {
        scancode = (scancode << 8) | inportb(0x60);
    }

    // If the key has just been released
    if (scancode & 0x80)
    {
        if (scancode == (KEY_LEFT_SHIFT | CODE_BREAK))
        {
            clear_flag(&keyboard_flags, KBD_LEFT_SHIFT);
        }
        else if (scancode == (KEY_RIGHT_SHIFT | CODE_BREAK))
        {
            clear_flag(&keyboard_flags, KBD_RIGHT_SHIFT);
        }
        else if (scancode == (KEY_LEFT_CONTROL | CODE_BREAK))
        {
            clear_flag(&keyboard_flags, KBD_LEFT_CONTROL);
        }
        else if (scancode == (KEY_RIGHT_CONTROL | CODE_BREAK))
        {
            clear_flag(&keyboard_flags, KBD_RIGHT_CONTROL);
        }
    }
    else
    {
        int32_t character = 0;
        // Parse the key.
        switch (scancode)
        {
            case KEY_LEFT_SHIFT:
                set_flag(&keyboard_flags, KBD_LEFT_SHIFT);
                break;
            case KEY_RIGHT_SHIFT:
                set_flag(&keyboard_flags, KBD_RIGHT_SHIFT);
                break;
            case KEY_LEFT_CONTROL:
                set_flag(&keyboard_flags, KBD_LEFT_CONTROL);
                break;
            case KEY_RIGHT_CONTROL:
                set_flag(&keyboard_flags, KBD_RIGHT_CONTROL);
                break;
            case KEY_CAPS_LOCK:
                keyboard_flags ^= KBD_CAPS_LOCK;
                keyboard_update_leds();
                break;
            case KEY_NUM_LOCK:
                keyboard_flags ^= KBD_NUM_LOCK;
                keyboard_update_leds();
                break;
            case KEY_SCROLL_LOCK:
                keyboard_flags ^= KBD_SCROLL_LOCK;
                keyboard_update_leds();
                break;
            case KEY_PAGE_UP:
                video_scroll_up();
                break;
            case KEY_PAGE_DOWN:
                video_scroll_down();
                break;
            case KEY_ESCAPE:
                break;
            case KEY_BACKSPACE:
                if (STEP(buf_w) == buf_r)
                {
                    buf_r = STEP(buf_r);
                }
                circular_buffer[buf_w] = '\b';
                buf_w = STEP(buf_w);
                video_delete_last_character();
                video_set_cursor_auto();
                break;
            case KEY_KP_RETURN:
            case KEY_ENTER:
                if (STEP(buf_w) == buf_r)
                {
                    buf_r = STEP(buf_r);
                }
                circular_buffer[buf_w] = '\n';
                buf_w = STEP(buf_w);
                video_new_line();
                video_set_cursor_auto();
                last_tab = 0;
                outportb(MASTER_PORT, EOI);
                break;
            case KEY_TAB:
                if (STEP(buf_w) == buf_r)
                {
                    buf_r = STEP(buf_r);
                }
                circular_buffer[buf_w] = '\t';
                buf_w = STEP(buf_w);
                video_put_tab();
                video_set_cursor_auto();
                last_tab++;
                break;
            case KEY_UP_ARROW:
                history_start(scancode);
                video_set_cursor_auto();
                break;
            case KEY_DOWN_ARROW:
                history_start(scancode);
                video_set_cursor_auto();
                break;
            case KEY_LEFT_ARROW:
                break;
            case KEY_RIGHT_ARROW:
                break;
            case KEY_LEFT_ALT:
                break;
            case KEY_RIGHT_ALT:
                break;
            default:
                if (has_flag(keyboard_flags, KBD_NUM_LOCK))
                {
                    character = keymap_it.numlock[scancode];
                }
                if (!character)
                {
                    // Apply shift modifier.
                    if (has_flag(keyboard_flags, (KBD_LEFT_SHIFT |
                                                  KBD_RIGHT_SHIFT)))
                    {
                        character = keymap_it.shift[scancode];
                    }
                    else
                    {
                        character = keymap_it.base[scancode];
                    }
                }
                if (character)
                {
                    // Apply caps lock modifier.
                    if (has_flag(keyboard_flags, KBD_CAPS_LOCK))
                    {
                        if (character >= 'a' && character <= 'z')
                        {
                            character += 'A' - 'a';
                        }
                        else if (character >= 'A' && character <= 'Z')
                        {
                            character += 'a' - 'A';
                        }
                    }
                    if (!shadow)
                    {
                        putchar(character);
                    }
                    // Update buffer.
                    if (STEP(buf_w) == buf_r)
                    {
                        buf_r = STEP(buf_r);
                    }
                    circular_buffer[buf_w] = character;
                    buf_w = STEP(buf_w);
                }
                break;
        }
    }
    outportb(MASTER_PORT, EOI);
    return;
}

void keyboard_update_leds()
{
    // Handle scroll_loc & num_loc & caps_loc.
    (keyboard_flags & KBD_SCROLL_LOCK) ? (ledstate |= 1) : (ledstate ^= 1);
    (keyboard_flags & KBD_NUM_LOCK) ? (ledstate |= 2) : (ledstate ^= 2);
    (keyboard_flags & KBD_CAPS_LOCK) ? (ledstate |= 4) : (ledstate ^= 4);
    // Write on the port.
    outportb(0x60, 0xED);
    outportb(0x60, ledstate);
}

void keyboard_enable()
{
    outportb(0x60, 0xF4);
}

void keyboard_disable()
{
    outportb(0x60, 0xF5);
}

int keyboard_getc(void)
{
    int c = -1;
    if (buf_r != buf_w)
    {
        c = circular_buffer[buf_r];
        buf_r = STEP(buf_r);
    }
    return c;
}

void keyboard_set_shadow(const bool_t value)
{
    shadow = value;
}

bool_t keyboard_get_shadow()
{
    return shadow;
}

bool_t keyboard_is_ctrl_pressed()
{
    return (bool_t) (keyboard_flags & (KBD_LEFT_CONTROL));
}

bool_t keyboard_is_shifted()
{
    return (bool_t) (keyboard_flags & (KBD_LEFT_SHIFT | KBD_RIGHT_SHIFT));
}
