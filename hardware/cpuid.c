/*
 * Dreamos
 * cpuid.c
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* CPUID-based function to detect CPU type
 * Author: shainer
 * Email: shainer@debianclan.org
 * June 2007
 */


#include <cpuid.h>
#include <kheap.h>

struct cpuinfo_generic *get_cpuid(void)
{
    int eax, ebx, ecx, edx;
    struct cpuinfo_generic *sh_info = kmalloc(sizeof(struct cpuinfo_generic));
    /* First call with EAX==0 */
    asm ("xorl %%eax, %%eax\n\t"
	 "cpuid\n\t"
	 : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx));

  /* Extract vendor string byte per byte */
  cpu_vendor[0] = (char)((ebx & 0x000000FF));
  cpu_vendor[1] = (char)((ebx & 0x0000FF00)>>8);
  cpu_vendor[2] = (char)((ebx & 0x00FF0000)>>16);
  cpu_vendor[3] = (char)((ebx & 0xFF000000)>>24);
  cpu_vendor[4] = (char)((edx & 0x000000FF));
  cpu_vendor[5] = (char)((edx & 0x0000FF00)>>8);
  cpu_vendor[6] = (char)((edx & 0x00FF0000)>>16);
  cpu_vendor[7] = (char)((edx & 0xFF000000)>>24);
  cpu_vendor[8] = (char)((ecx & 0x000000FF));
  cpu_vendor[9] = (char)((ecx & 0x0000FF00)>>8);
  cpu_vendor[10] = (char)((ecx & 0x00FF0000)>>16);
  cpu_vendor[11] = (char)((ecx & 0xFF000000)>>24);
  cpu_vendor[12] = '\0';
  sh_info->cpu_vendor = cpu_vendor;

  eax = ebx = ecx = edx = 0;

  /* Second call with EAX == 1 */
  asm ("movl $0x1, %%eax\n\t"
       "cpuid\n\t"
       "andl $0x3000, %%eax\n\t"
       "cmpl %%eax, 0\n\t"
       "je oem\n\t"
       "cmpl %%eax, 0x2000 \n\t"
       "je dual_core \n\t"
       "cmpl %%eax, 0x1000 \n\t"
       "je overdrive \n\t"
       "jmp other \n\t"
       "oem: \n\t"
       "movl $0x1, %%eax\n\t"
       "jmp other\n\t"
       "dual_core:\n\t"
       "movl $0x2, %%eax\n\t"
       "jmp other\n\t"
       "overdrive:\n\t"
       "movl $0x3, %%eax\n\t"
       "other:\n\t"
       "nop\n\t"
       : "=a" (eax));

  /* Processor type */
  switch (eax) {
  case 0:
      sh_info->cpu_type = "Others";
      break;
  case 1:
      sh_info->cpu_type = "Original OEM Processor";
      break;
  case 2:
      sh_info->cpu_type = "Intel Dual Core";
      break;
  case 3:
      sh_info->cpu_type = "Intel Overdrive";
  }

  return sh_info;
}
