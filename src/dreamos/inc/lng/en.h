/// @file   en.h
/// @brief  English strings used for the interface.
/// @author Ivan Gualandri
/// @date   Oct 27 2003
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

// -----------------------------------------------------------------------------
// Boot Messages
#define LNG_INIT_SYSCALL    "Initialize system calls..."
#define LNG_INIT_MEMORY     "Initialize the memory..."
#define LNG_INIT_GDT        "Initialize GDT..."
#define LNG_INIT_IDT        "Initialize IDT..."
#define LNG_INIT_IRQ        "Initialize IRQ..."
#define LNG_INIT_VM         "Initialize virtual memory..."
#define LNG_INIT_HEAP       "Initialize heap..."
#define LNG_INIT_CPUID      "Initialize the CPUID structure..."
#define LNG_INIT_FS         "Initialize the fylesystem..."
#define LNG_INIT_SCHEDULER  "Initialize the scheduler..."
#define LNG_INIT_SHELL      "Initialize the shell..."
#define LNG_SITE            "\nSite:"

// -----------------------------------------------------------------------------
// Driver Loading Messages
#define LNG_KEYBOARD_SETUP  "Setting up keyboard driver..."
#define LNG_MOUSE_SETUP     "Setting up mouse driver..."
#define LNG_TIMER_SETUP     "Setting up timer..."

// -----------------------------------------------------------------------------
// Shell Messages
#define LNG_WELCOME         "\t\t.: Welcome to DreamOS :.\n\n"

// -----------------------------------------------------------------------------
#define LNG_CPU             "\nCPU:"
#define LNG_USER            "[?] Insert your username :"
#define LNG_PWD             "[?] Insert your password :"
#define LNG_WRONG_CRED      "[x] Sorry, try again.\n"
#define LNG_UNKNOWN_CMD     "Unknown command:"

// -----------------------------------------------------------------------------
// MONTHS
#define LNG_DATE_JAN "January"
#define LNG_DATE_FEB "February"
#define LNG_DATE_MAR "March"
#define LNG_DATE_APR "April"
#define LNG_DATE_MAY "May"
#define LNG_DATE_JUN "June"
#define LNG_DATE_JUL "July"
#define LNG_DATE_AUG "August"
#define LNG_DATE_SEP "September"
#define LNG_DATE_OCT "October"
#define LNG_DATE_NOV "November"
#define LNG_DATE_DEC "December"
// DAYS
#define LNG_DAY_SUN "Sunday"
#define LNG_DAY_MON "Monday"
#define LNG_DAY_TUE "Tuesday"
#define LNG_DAY_WED "Wednesday"
#define LNG_DAY_THU "Thursday"
#define LNG_DAY_FRI "Friday"
#define LNG_DAY_SAT "Saturday"
// HOURS
#define LNG_HOUR_SUMM "Summer hour"
#define LNG_HOUR_WINT "Winter hour"
#define LNG_TIMESTAMP "It's "
#define LNG_TIMESTAMP2 " of "
#define LNG_TIMESTAMP3 " of "