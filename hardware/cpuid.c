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
 * Email: syn.shainer@gmail.com
 */

#include <cpuid.h>
#include <kheap.h>
#include <string.h>

/*
 * Main CPUID procedure
 * @author shainer
 * @param sh_info Structure to fill with CPUID information
 * @return none
 */
void get_cpuid(struct cpuinfo_generic *sh_info)
{
  struct registri *ereg = kmalloc(sizeof(struct registri));
  
  ereg->eax = ereg->ebx = ereg->ecx = ereg->edx = 0;
  cpuid_write_vendor (sh_info, ereg);

  ereg->eax = 1;
  ereg->ebx = ereg->ecx = ereg->edx = 0;
  cpuid_write_proctype (sh_info, ereg);

  free (ereg);
}

/* Actual CPUID call */
void call_cpuid (struct registri *regs)
{
  asm ("cpuid\n\t"
       : "=a" (regs->eax), "=b" (regs->ebx), "=c" (regs->ecx), "=d" (regs->edx)
       : "a" (regs->eax));
}

/* Extract vendor string */
void cpuid_write_vendor (struct cpuinfo_generic *s, struct registri *regs)
{
  call_cpuid (regs);

  cpu_vendor[0] = (char)((regs->ebx & 0x000000FF));
  cpu_vendor[1] = (char)((regs->ebx & 0x0000FF00)>>8);
  cpu_vendor[2] = (char)((regs->ebx & 0x00FF0000)>>16);
  cpu_vendor[3] = (char)((regs->ebx & 0xFF000000)>>24);
  cpu_vendor[4] = (char)((regs->edx & 0x000000FF));
  cpu_vendor[5] = (char)((regs->edx & 0x0000FF00)>>8);
  cpu_vendor[6] = (char)((regs->edx & 0x00FF0000)>>16);
  cpu_vendor[7] = (char)((regs->edx & 0xFF000000)>>24);
  cpu_vendor[8] = (char)((regs->ecx & 0x000000FF));
  cpu_vendor[9] = (char)((regs->ecx & 0x0000FF00)>>8);
  cpu_vendor[10] = (char)((regs->ecx & 0x00FF0000)>>16);
  cpu_vendor[11] = (char)((regs->ecx & 0xFF000000)>>24);
  cpu_vendor[12] = '\0';
  s->cpu_vendor = cpu_vendor;
}

/*
 * CPUID is called with EAX=1
 * EAX contains Type, Family, Model and Stepping ID
 * EBX contains the Brand Index if supported, and the APIC ID
 * ECX/EDX contains feature information
 */
void cpuid_write_proctype (struct cpuinfo_generic *s, struct registri *regs)
{
  int type;
  int familyID;
  int model;
  int ext_model;

  call_cpuid (regs);

  type = cpuid_get_byte (regs->eax, 0xB, 0x3);
  switch (type)
  {
    case 0:
    s->cpu_type = "Original OEM Processor";
    break;

    case 1:
    s->cpu_type = "Intel Overdrive Processor";
    break;

    case 2:
    s->cpu_type = "Dual processor";
    break;

    case 3:
    s->cpu_type = "(Intel reserved bit)";
    break;
  }

  familyID = cpuid_get_byte(regs->eax, 0x7, 0xE);
  s->cpu_family = familyID;
  if (familyID == 0x0F)
    s->cpu_family += cpuid_get_byte(regs->eax, 0x13, 0xFF);

  model = cpuid_get_byte(regs->eax, 0x3, 0xE);
  s->cpu_model = model;
  if (familyID == 0x06 || familyID == 0x0F) {
    ext_model = cpuid_get_byte (regs->eax, 0xF, 0xE);
    s->cpu_model += (ext_model<<4);
  }

  s->apic_id = cpuid_get_byte (regs->ebx, 0x17, 0xFF);
  cpuid_feature_ecx (s, regs->ecx);
  cpuid_feature_edx (s, regs->edx);

  /* Get brand string to identify the processor */
  if (familyID >= 0x0F && model >= 0x03)
    s->brand_string = cpuid_brand_string (regs);
  else 
    s->brand_string = cpuid_brand_index (regs);
}

/*
 * Index of brand strings
 */
char *cpuid_brand_index (struct registri *r)
{
  char *indexes[21] = { "Reserved", "Intel Celeron", "Intel Pentium III", "Intel Pentium III Xeon", "Mobile Intel Pentium III", "Mobile Intel Celeron", "Intel Pentium 4", "Intel Pentium 4", "Intel Celeron", "Intel Xeon MP", "Intel Xeon MP", "Mobile Intel Pentium 4", "Mobile Intel Celeron", "Mobile Genuine Intel", "Intel Celeron M", "Mobile Intel Celeron", "Intel Celeron", "Mobile Genuine Intel", "Intel Pentium M", "Mobile Intel Celeron", NULL };

  int bx = (r->ebx & 0xFF);

  if (bx > 0x17)
    bx=0;

  return indexes[bx];
}

/*
 * Brand string is contained in EAX, EBX, ECX and EDX
 */
char *cpuid_brand_string (struct registri *r)
{
  char *temp;

  for(r->eax = 0x80000002; r->eax <= 0x80000004; (r->eax)++) {
    r->ebx = r->ecx = r->edx = 0;
    call_cpuid (r);
    temp = strncat (temp, (const char*)r->eax, strlen((const char*)r->eax));
    temp = strncat (temp, (const char*)r->ebx, strlen((const char*)r->ebx));
    temp = strncat (temp, (const char*)r->ecx, strlen((const char*)r->ecx));
    temp = strncat (temp, (const char*)r->edx, strlen((const char*)r->edx));
  }
   
  return temp;
}

/*
 * EAX=1
 * ECX contains a list of supported features
 */
void cpuid_feature_ecx (struct cpuinfo_generic *s, int ecx)
{
  int temp = ecx;
  int i;

  for (i=0; i<ECX_FLAGS_SIZE; i++) {
    temp = cpuid_get_byte (temp, i, 1);
    s->cpuid_ecx_flags[i] = temp;
    temp = ecx;
  }   
}

/*
 * Same as above
 */
void cpuid_feature_edx (struct cpuinfo_generic *s, int edx)
{
  int temp=edx;
  int i;

  for (i=0; i<EDX_FLAGS_SIZE; i++) {
    temp = cpuid_get_byte (temp, i, 1);
    s->cpuid_edx_flags[i] = temp;
    temp=edx;
  }
}

/*
 * Extract single byte from a register
 */
int cpuid_get_byte (int reg, int position, int value)
{
  return ((reg >> position) & value);
}
