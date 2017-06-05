/// @file   keymap.h
/// @brief  Keymap for keyboard.
/// @author Galfurian
/// @date   Apr 1 2017
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

#pragma once

#include "stdint.h"

#define KEY_ESCAPE          0x01 ///< Escape character.
#define KEY_ONE             0x02 ///< 1
#define KEY_TWO             0x03 ///< 2
#define KEY_THREE           0x04 ///< 3
#define KEY_FOUR            0x05 ///< 4
#define KEY_FIVE            0x06 ///< 5
#define KEY_SIX             0x07 ///< 6
#define KEY_SEVEN           0x08 ///< 7
#define KEY_EIGHT           0x09 ///< 8
#define KEY_NINE            0x0A ///< 9
#define KEY_ZERO            0x0B ///< 0
#define KEY_APOSTROPHE      0x0C ///< '
#define KEY_I_ACC           0x0D ///< i'
#define KEY_BACKSPACE       0x0E ///< Backspace
#define KEY_TAB             0x0F ///< Tabular
#define KEY_Q               0x10 ///< q
#define KEY_W               0x11 ///< w
#define KEY_E               0x12 ///< e
#define KEY_R               0x13 ///< r
#define KEY_T               0x14 ///< t
#define KEY_Y               0x15 ///< y
#define KEY_U               0x16 ///< u
#define KEY_I               0x17 ///< i
#define KEY_O               0x18 ///< o
#define KEY_P               0x19 ///< p
#define KEY_LEFT_BRAKET     0x1A ///< (
#define KEY_RIGHT_BRAKET    0x1B ///< )
#define KEY_ENTER           0x1C ///< Enter
#define KEY_LEFT_CONTROL    0x1D ///< left-ctrl
#define KEY_A               0x1E ///< a
#define KEY_S               0x1F ///< s
#define KEY_D               0x20 ///< d
#define KEY_F               0x21 ///< f
#define KEY_G               0x22 ///< g
#define KEY_H               0x23 ///< h
#define KEY_J               0x24 ///< j
#define KEY_K               0x25 ///< k
#define KEY_L               0x26 ///< l
#define KEY_SEMICOLON       0x27 ///< '
#define KEY_DOUBLE_QUOTES   0x28 ///< "
#define KEY_GRAVE           0x29 ///< ` or ~
#define KEY_LEFT_SHIFT      0x2A ///< LShift
#define KEY_BACKSLASH       0x2B ///< \ or |
#define KEY_Z               0x2c ///< Z
#define KEY_X               0x2d ///< X
#define KEY_C               0x2e ///< C
#define KEY_V               0x2f ///< V
#define KEY_B               0x30 ///< B
#define KEY_N               0x31 ///< N
#define KEY_M               0x32 ///< M
#define KEY_COMMA           0x33 ///< , or <
#define KEY_PERIOD          0x34 ///< . or >
#define KEY_MINUS           0x35 ///< - or _
#define KEY_RIGHT_SHIFT     0x36 ///< RShift
#define KEY_KP_MUL          0x37 ///< NP - *
#define KEY_LEFT_ALT        0x38 ///< LAlt
#define KEY_SPACE           0x39 ///< Space
#define KEY_CAPS_LOCK       0x3a ///< Caps Lock
#define KEY_F1              0x3B ///<  59
#define KEY_F2              0x3C ///<  60
#define KEY_F3              0x3D ///<  61
#define KEY_F4              0x3E ///<  62
#define KEY_F5              0x3F ///<  63
#define KEY_F6              0x40 ///<  64
#define KEY_F7              0x41 ///<  65
#define KEY_F8              0x42 ///<  66
#define KEY_F9              0x43 ///<  67
#define KEY_F10             0x44 ///<  68
#define KEY_NUM_LOCK        0x45 ///<  69       Num Lock
#define KEY_SCROLL_LOCK     0x46 ///<  70       Scroll Lock
#define KEY_KP7             0x47 ///<  71       NP - Home
#define KEY_KP8             0x48 ///<  72       NP - UpArrow
#define KEY_KP9             0x49 ///<  73       NP - Pgup
#define KEY_KP_SUB          0x4A ///<  74       NP - Grey
#define KEY_KP4             0x4B ///<  75       NP - LArrow
#define KEY_KP5             0x4C ///<  76       NP - Center
#define KEY_KP6             0x4D ///<  77       NP - RArrow
#define KEY_KP_ADD          0x4E ///<  78       NP - Grey +
#define KEY_KP1             0x4F ///<  79       NP - End
#define KEY_KP2             0x50 ///<  80       NP - DArrow
#define KEY_KP3             0x51 ///<  81       NP - Pgdn
#define KEY_KP0             0x52 ///<  82       NP - Ins
#define KEY_KP_DEC          0x53 ///<  83       NP - Del
#define KEY_KP_LESS         0x56 ///<  86       NP - Del

#define KEY_KP_RETURN       0xe01c ///<           NP - Enter   57372
#define KEY_RIGHT_CONTROL   0xE01D ///<           Right Ctrl   57373
#define KEY_KP_DIV          0xE035 ///<           Divide       57397
#define KEY_RIGHT_ALT       0xe038 ///<           Right Alt    57400
#define KEY_F11             0xe057 ///<           F11          57431
#define KEY_F12             0xe058 ///<           F12          57432
#define KEY_LEFT_WIN        0xe05b ///<           Left Winkey  57435
#define KEY_RIGHT_WIN       0xe05c ///<           Right Winkey 57436
#define KEY_INSERT          0xe052 ///<           Ins          57426
#define KEY_HOME            0xe047 ///<           Home         57415
#define KEY_UP_ARROW        0xe048 ///<           Up Arrow     57416
#define KEY_PAGE_UP         0xe049 ///<           Pgup         57417
#define KEY_LEFT_ARROW      0xe04b ///<           Left Arrow   57419
#define KEY_DELETE          0xe053 ///<           Del          57427
#define KEY_END             0xe04f ///<           End          57423
#define KEY_PAGE_DOWN       0xe051 ///<           Pgdn         57425
#define KEY_RIGHT_ARROW     0xe04d ///<           Right Arrow  57421
#define KEY_DOWN_ARROW      0xe050 ///<           Down Arrow   57424

#define CODE_BREAK          0x80

#define NUM_LED         0x45
#define SCROLL_LED      0x46
#define CAPS_LED        0x3a

/// @brief Defines a set of arrays used to map key to characters.
typedef struct keymap_t
{
    /// @brief The basic mapping.
    int32_t base[65536];
    /// @brief The mapping when shifted.
    int32_t shift[65536];
    /// @brief The mapping when numlock is active.
    uint32_t numlock[65536];
} keymap_t;

extern const keymap_t keymap_it;
