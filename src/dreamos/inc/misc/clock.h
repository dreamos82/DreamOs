/// @file   clock.h
/// @brief  Clock functions.
/// @author Alessandro Lopez - Ivan Gualandri
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

#pragma once

/// Used to store time values.
typedef unsigned int time_t;

/// Used to get information about the current time.
typedef struct tm_t
{
    int tm_sec;   ///< Seconds                [0 to 59]
    int tm_min;   ///< Minutes                [0 to 59]
    int tm_hour;  ///< Hours                  [0 to 23]
    int tm_mday;  ///< Day of the month       [1 to 31]
    int tm_mon;   ///< Month                  [0 to 11]
    int tm_year;  ///< Year                   [since 1900]
    int tm_wday;  ///< Day of the week        [0 to 6]
    int tm_yday;  ///< Day in the year        [0 to 365]
    int tm_isdst; ///< Is Daylight Saving Time.
} tm_t;

#define SECOND_RTC  0x00    ///< Value used to retrieve the seconds.
#define MINUTE_RTC  0x02    ///< Value used to retrieve the minutes.
#define HOUR_RTC    0x04    ///< Value used to retrieve the hours.
#define DAY_W_RTC   0x06    ///< Value used to retrieve the day of the week.
#define DAY_M_RTC   0x07    ///< Value used to retrieve the day of the month.
#define MONTH_RTC   0x08    ///< Value used to retrieve the month.
#define YEAR_RTC    0x09    ///< Value used to retrieve the year.

/// @brief Returns the seconds.
time_t get_second();

/// @brief Returns the hours.
time_t get_hour();

/// @brief Returns the minutes.
time_t get_minute();

/// @brief Returns the day of the month.
time_t get_day_m();

/// @brief returns the month.
time_t get_month();

/// @brief Returns the year.
time_t get_year();

/// @brief Returns the day of the week.
time_t get_day_w();

/// @brief Returns the name of the month.
char * get_month_lng();

/// @brief Returns the name of the day.
char * get_day_lng();

/// @brief Returns the current time.
time_t time(time_t * timer);
