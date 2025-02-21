/* *****************************************************************************
   MIT License

   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
***************************************************************************** */
#include "main.h"
/* ------------------------------------
   Function to find a symbol in the symbol table of a ELF file.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

void *tuim_FindSymbol(const uint8_t *symbol, const elf_s *elf){
   uint8_t
      *obj = elf->file.obj,
      *program = elf->image.program,
      *strtab = elf->dynsym.str;
   Elf_Addr start_vaddr = elf->image.start_vaddr;
   Elf_Ehdr *ehdr = (Elf_Ehdr*)obj;
   Elf_Sym *sym = elf->dynsym.sym;

   for(size_t i = elf->dynsym.first_non_local; i < elf->dynsym.symnum; ++i){

      if(_strcmp(
         symbol,
         strtab + sym[i].st_name
      ) == 0){
         uint16_t st_shndx;
         st_shndx = sym[i].st_shndx;
         if(st_shndx == STN_UNDEF)
            return NULL;
         else if(st_shndx == SHN_XINDEX)
            return NULL; // FIXME: Find for it on SHT_SYMTAB_SHNDX
         return sym + i;
      }
   }

   return NULL;
}
