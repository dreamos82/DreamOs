/*
 * Copyright (c), Dario Casalinuovo
 * 
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
 
 //
 // Based on JamesM's kernel developement tutorials.
 //

#include <elf.h>
#include <string.h>

elf_t elf_from_multiboot (struct multiboot_info *mb)
{
  elf_t elf;
  elf_section_header_t *sh = (elf_section_header_t*)mb->u.elf_sec.addr;

  uint32_t shstrtab = sh[mb->u.elf_sec.shndx].addr;
  for (int i = 0; i < mb->u.elf_sec.num; i++) {
    const char *name = (const char *) (shstrtab + sh[i].name);
    if (!strcmp (name, ".strtab")) {
      elf.strtab = (const char *)sh[i].addr;
      elf.strtabsz = sh[i].size;
    }

    if (!strcmp (name, ".symtab")) {
      elf.symtab = (elf_symbol_t*)sh[i].addr;
      elf.symtabsz = sh[i].size;
    }
  }
  return elf;
}

const char *elf_lookup_symbol (uint32_t addr, elf_t *elf)
{
  for (int i = 0; i < (elf->symtabsz/sizeof (elf_symbol_t)); i++) {
    if (ELF32_ST_TYPE(elf->symtab[i].info) != 0x2)
      continue;

    if ( (addr >= elf->symtab[i].value) &&
         (addr < (elf->symtab[i].value + elf->symtab[i].size)) ) {
      const char *name
		= (const char *) ((uint32_t)elf->strtab + elf->symtab[i].name);
      return name;
    }
  }
  return NULL;
}
