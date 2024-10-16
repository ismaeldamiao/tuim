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

#include "tuim_utils.h"

/* ------------------------------------
   Function to free the memory alocated by tuim_loader.
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

void tuim_free(tuim_elf *elf){

   if(elf){
      Elf_Ehdr *ehdr;
      Elf_Shdr *phdr;
      Elf_Shdr *shdr;

      ehdr = elf->ehdr;
      phdr = elf->phdr;
      shdr = elf->shdr;

      list_remove(&tuim_loaded, elf);

      if(ehdr){
         uint16_t e_shnum, e_phnum;

         e_shnum = ELF_E_SHNUM(*ehdr);
         e_phnum = ELF_E_PHNUM(*ehdr);

         if(shdr){
            // Free dependencies
            while(elf->sections[0]){
               tuim_elf *dep;
               dep = ((elf_list*)elf->sections[0])->elf;
               list_remove((void*)elf->sections, dep);
               tuim_free(dep);
            }

            for(uint16_t i = UINT16_C(1); i < e_shnum; ++i){
               bool free_it;
               size_t sh_size;
               uint32_t sh_flags;
               uintptr_t sh_addr;

               free_it = true;
               sh_size = ELF_SH_SIZE(shdr[i]);
               sh_addr = ELF_SH_OFFSET(shdr[i]);
               sh_flags = ELF_SH_FLAGS(shdr[i]);

               if(sh_flags & SHF_ALLOC){
                  for(uint16_t j = UINT16_C(0); j < e_phnum; ++j)
                  if(ELF_P_TYPE(phdr[j]) == PT_LOAD){
                     uintptr_t p_vaddr;
                     size_t p_memsz;

                     p_vaddr = ELF_P_VADDR(phdr[j]);
                     p_memsz = (size_t)ELF_P_MEMSZ(phdr[j]);

                     if(
                        (sh_addr >= p_vaddr) &&
                        ((sh_addr + sh_size) <= (p_vaddr + p_memsz))
                     ){
                        free_it = false;
                        break;
                     }
                  }
               }
               if(free_it) free(elf->sections[i]);
            }
            free(elf->sections);
            free(shdr);
         }

         if(phdr){
            for(uint16_t i = UINT16_C(0); i < e_phnum; ++i){
               bool free_it;
               size_t p_memsz;
               uint32_t p_type;
               uintptr_t p_vaddr;

               p_type = ELF_P_TYPE(phdr[i]);
               if(p_type == PT_LOAD){
                  free(elf->segments[i]);
                  continue;
               }

               free_it = true;
               p_vaddr = ELF_P_VADDR(phdr[i]);
               p_memsz = (size_t)ELF_P_MEMSZ(phdr[i]);
               elf->segments[i] = NULL;

               for(uint16_t j = UINT16_C(0); j < e_phnum; ++j){
                  uintptr_t pp_vaddr;
                  size_t pp_memsz;

                  if(ELF_P_TYPE(phdr[j]) != PT_LOAD) continue;

                  pp_vaddr = ELF_P_VADDR(phdr[j]);
                  pp_memsz = (size_t)ELF_P_MEMSZ(phdr[j]);
                  if(
                     (p_vaddr >= pp_vaddr) &&
                     ((p_vaddr + p_memsz) <= (pp_vaddr + pp_memsz))
                  ){
                     free_it = false;
                     break;
                  }
               }
               if(free_it)free(elf->segments[i]);
            }
            free(elf->segments);
            free(elf->phdr);
         }
         free(elf->ehdr);
      }
      free(elf);
   }
   return;
}
