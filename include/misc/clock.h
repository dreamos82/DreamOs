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

#define SECOND_RTC 0x00
#define MINUTE_RTC 0x02
#define HOUR_RTC 0x04
#define DAY_W_RTC 0x06
#define DAY_M_RTC 0x07
#define MONTH_RTC 0x08
#define YEAR_RTC 0x09

int get_second();
int get_hour();
int get_minute();
int get_second();
int get_day_m();
int get_month();
int get_year();
int get_day_w();
char* get_month_lng();
char* get_day_lng();
void date();


#endif
