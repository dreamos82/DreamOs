/***************************************************************************
 *            cloch.h
 *
 *  Sat Mar 31 07:47:17 2007
 *  Copyright  2007  Alessandro Lopez - Ivan Gualandri
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

#ifndef __CLOCK_H
#define __CLOCK_H

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
    int tm_isdst;
} tm_t;

#define SECOND_RTC 0x00
#define MINUTE_RTC 0x02
#define HOUR_RTC 0x04
#define DAY_W_RTC 0x06
#define DAY_M_RTC 0x07
#define MONTH_RTC 0x08
#define YEAR_RTC 0x09

time_t get_second();

time_t get_hour();

time_t get_minute();

time_t get_second();

time_t get_day_m();

time_t get_month();

time_t get_year();

time_t get_day_w();

char * get_month_lng();

char * get_day_lng();

time_t time(time_t * timer);

#endif
