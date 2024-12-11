/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#include <stdint.h>

#include "include/tuim.h"
#include "elf.h"
#include "tuim_private.h"
#include "tuim_utils.h"

/* ------------------------------------
   Function to find a symbol in a alread loaded ELF file.
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

void* tuim_findsymbol(const char *symbol, const tuim_elf *elf){
   Elf_Ehdr *ehdr;
   Elf_Shdr *shdr;
   unsigned int e_shnum;

   if(elf == NULL) return NULL;

   ehdr = elf->ehdr;
   shdr = elf->shdr;

   e_shnum = (unsigned int)ELF_E_SHNUM(*ehdr);

   for(unsigned int i = 0U; i < e_shnum; ++i){
      uint32_t sh_type;

      sh_type = ELF_SH_TYPE(shdr[i]);

      if(sh_type == SHT_SYMTAB){
         Elf_Sym *syms;
         uint8_t *str;
         unsigned int sym_num;

         syms = elf->sections[i];
         str = elf->sections[ELF_SH_LINK(shdr[i])];
         sym_num =
         (unsigned int)ELF_SH_SIZE(shdr[i]) / (unsigned int)sizeof(Elf_Sym);

         for(unsigned int j = 0U; j < sym_num; ++j){

            if(strcmp_(
               (uint8_t*)symbol,
               str + ELF_ST_NAME(syms[j])
            ) == 0){
               uint16_t st_shndx;
               st_shndx = ELF_ST_SHNDX(syms[j]);
               if(st_shndx == STN_UNDEF)
                  return NULL;
               else if(st_shndx == SHN_XINDEX)
                  return NULL; // FIXME: Find for it on SHT_SYMTAB_SHNDX
               return (
                  (uint8_t*)(elf->program) +
                  ELF_ST_VALUE(syms[j]) - elf->start_vaddr
               );
            }
         }
      }
   }
   return NULL;
}
