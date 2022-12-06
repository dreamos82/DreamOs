/***************************************************************************
 *            vesa.c
 *
 *  Fri Jan 6 07:47:55 2017
 *  Copyright  2017  Davide Gessa (dakk)
 *  Email : gessadavide@gmail.com
 *  VESA driver
 * ***************************************************************************/

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

#include <drivers/vesa.h>
#include <misc/support_defs.h>

vesa_info_t *vci;
vesa_mode_info_t *vmi;

			
vesa_mode_info_t *vesa_get_mode_info()
{
	return vmi;
}


void vesa_draw_pixel(uint32_t x, uint32_t y, uint32_t cl)
{
	register char *ptmp;
	
	if (x < 0 || x > vmi->width || y < 0 || y > vmi->height) return;
	
	x = (x * (vmi->bits_per_pixel >> 3));
	y = (y * vmi->bytes_per_scan_line);
	
	char *asd = (char *)vmi->phys_base_ptr;
	ptmp = &asd[x+y];
	ptmp[0] = cl & 0xff;
	ptmp[1] = (cl>>8) & 0xff;
	ptmp[2] = (cl>>16) & 0xff;
}



uint32_t vesa_get_pixel(uint32_t x, uint32_t y)
{
	if (x < 0 || x > vmi->width || y < 0 || y > vmi->height) return 0;
	
	x = (x * (vmi->bits_per_pixel >> 3));
	y = (y * vmi->bytes_per_scan_line);
	
	char *vmem = (char *) vmi->phys_base_ptr;
    return (uint32_t) vmem [x+y];
}


uint32_t vesa_set_mode(uint32_t x, uint32_t y, uint32_t bit)
{
	return 0;
}


uint32_t vesa_init(void *vbe_control_info, void *vbe_mode_info)
{
	vci = (vesa_info_t *) vbe_control_info;
	vmi = (vesa_mode_info_t *) vbe_mode_info;
    return 0;
}