/***************************************************************************
 *            utsname.c
 *
 *  Mon Jan 26 23:30:13 2008
 *  Copyright  2007  Ivan Gualandri
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

#include <sys/utsname.h>
#include <string.h>
#include <version.h>

int uname(struct utsname* os_infos){
/*Uname code goes here*/  
  strcpy(os_infos->sysname, NAME);
  strcpy(os_infos->release, PATCHLEVEL);
  strcpy(os_infos->version, VERSION);
  strcpy(os_infos->nodename, "testbed");
  strcpy(os_infos->machine, "i686");
  return 0;
}
