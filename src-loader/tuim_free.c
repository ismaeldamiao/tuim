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
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/tuim.h"
#include "elf.h"

#include "tuim_private.h"
#include "tuim_utils.h"

/* ------------------------------------
   Function to free the memory alocated by tuim_loader.
   * Part of tuim project.
   * Last modified: December 7, 2024.
------------------------------------ */

void tuim_free(tuim_elf *elf){

   if(elf != NULL){
      Elf_Ehdr *ehdr;
      Elf_Shdr *phdr;
      Elf_Shdr *shdr;

      ehdr = elf->ehdr;
      phdr = elf->phdr;
      shdr = elf->shdr;

      list_remove(&tuim_loaded, elf);

      if(ehdr != NULL){
         uint16_t e_shnum, e_phnum;

         e_shnum = ELF_E_SHNUM(*ehdr);

         if(phdr != NULL) free(elf->program);
         free(elf->phdr);

         if(shdr != NULL){
            // Free dependencies
            while(elf->sections[0]){
               tuim_elf *dep;
               dep = ((elf_list*)elf->sections[0])->elf;
               list_remove((void*)elf->sections, dep);
               tuim_free(dep);
            }

            // Free section that do not appeas in program memory image
            for(uint16_t i = UINT16_C(1); i < e_shnum; ++i){
               size_t sh_size;
               uint32_t sh_flags;

               sh_size = ELF_SH_SIZE(shdr[i]);
               sh_flags = ELF_SH_FLAGS(shdr[i]);

               if(!(sh_flags & SHF_ALLOC) && (sh_size != (size_t)0))
                  free(elf->sections[i]);
            }
            free(elf->sections);
            free(shdr);
         }

         free(elf->ehdr);
      }
      free(elf);
   }
   return;
}
