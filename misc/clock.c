/***************************************************************************
 *            clock.c
 s *  Sat Mar 31 07:47:17 2007
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
#include <clock.h>
#include <io.h>
#include <stdio.h>
#include <use.h>

int get_second()
{
  outportb(SECOND_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

int get_minute()
{
  outportb(MINUTE_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

int get_hour()
{
  outportb(HOUR_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

int get_day_w()
{
  outportb(DAY_W_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

int get_day_m()
{
  outportb(DAY_M_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

int get_month()
{
  outportb(MONTH_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

int get_year()
{
  outportb(YEAR_RTC,0x70);
  int c = inportb(0x71);
  return c;
}

char* get_month_lng()
{
  int c;
  c = get_month();
  switch(c) {
    case 1:
      return LNG_DATE_JAN;
      break;
    case 2:
      return LNG_DATE_FEB;
      break;
    case 3:
      return LNG_DATE_MAR;
      break;
    case 4:
      return LNG_DATE_APR;
      break;
    case 5:
      return LNG_DATE_MAY;
      break;
    case 6:
      return LNG_DATE_JUN;      
      break;
    case 7:
      return LNG_DATE_JUL;     
      break;
    case 8:
      return LNG_DATE_AUG;  
      break;
    case 9:
      return LNG_DATE_SEP;
      break;
    case 10:
      return LNG_DATE_OCT;
      break;
    case 11:
      return LNG_DATE_NOV;
      break;
    case 12:
      return LNG_DATE_DEC;     
      break;
  }
  return "";
}

char* get_day_lng()
{
  int c;
  c = get_day_w();
  switch(c) {
    case 0:
      return LNG_DAY_SUN;
      break;
    case 1:
      return LNG_DAY_MON;
      break;
    case 2:
      return LNG_DAY_TUE;
      break;
    case 3:
      return LNG_DAY_WED;
      break;
    case 4:
      return LNG_DAY_THU;
      break;
    case 5:
      return LNG_DAY_FRI;      
      break;
    case 6:
      return LNG_DAY_SAT;     
      break;
  }
  return "";
}

void date()
{
       asm("cli");
       printf("%s %x:%x:%x %s %s %02x %s %02x\n",LNG_TIMESTAMP,get_hour(),get_minute(),get_second(),LNG_TIMESTAMP3, get_day_lng(), get_day_m(), get_month_lng(), 0x2000+get_year());
       asm("sti");
}

