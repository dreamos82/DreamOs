/***************************************************************************
 *            cloch.c
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
#include <clock.h>
#include <stdio.h>

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
      return "Gennaio";
      break;
    case 2:
      return "Febbraio";
      break;
    case 3:
      return "Marzo";
      break;
    case 4:
      return "Aprile";
      break;
    case 5:
      return "Maggio";      
    case 6:
      return "Giugno";      
      break;
    case 7:
      return "Luglio";     
      break;
    case 8:
      return "Agosto";  
      break;
    case 9:
      return "Settembre";
      break;
    case 10:
      return "Ottobre";
      break;
    case 11:
      return "Novembre";
      break;
    case 12:
      return "Dicembre";     
      break;
  }
  return "";
}
		
void date()
{
	asm("cli");	
	printf("Ora: %02x:%02x:%02x - %02x - %s - %02x\n",get_hour(),get_minute(),get_second(),get_day_m(),get_month_lng(),0x2000+get_year());	
	asm("sti");
}