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
#include <stddef.h>
#include <stdint.h>
/* ------------------------------------
   This function find by a symbol definition on dynamic symbol table.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */
#include "elf.h"
#include "../api/ascii/string.h"

#include "../api/tuim.h"
#include "../api/tuim_ctx.h"
#include "tuim_backend.h"

uint64_t tuim_get_symbol(const tuim_ctx *ctx, void *ptr, const uint8_t *symbol){
   struct tuim_backend *info = ptr;
   const Elf(Ehdr) *ehdr = (void*)(info->obj);
   const Elf(Sym) *dynsym = info->dynsym;
   const Elf(Byte) *dynstr = info->dynstr;

   const Elf(Sym) *sym;
   size_t i, i_max;
   uint64_t address;

   (void)ctx;

   if(symbol == NULL){
      address = (is_Elf32 ? swap32(ehdr->e_entry) : swap64(ehdr->e_entry));
      address = (uint64_t)(info->program_image) + (address - info->start_vaddr);
      return address;
   }

   i_max = info->sh_size / info->sh_entsize;
   for(i = info->sh_info; i < i_max; ++i){
      sym = (void*)((Elf(Byte)*)dynsym + i * info->sh_entsize);
      if(ascii_strcmp(symbol, dynstr + swap32(sym->st_name)) == 0){
         unsigned int st_shndx = swap16(sym->st_shndx);
         if(st_shndx == STN_UNDEF){
            return (uint64_t)NULL;
         }else if(st_shndx == SHN_XINDEX){
            return (uint64_t)NULL; // FIXME: Find for it on SHT_SYMTAB_SHNDX
         }else{
            address =
               (is_Elf32 ? swap32(sym->st_value) : swap64(sym->st_value));
            return
               (uint64_t)(info->program_image) + (address - info->start_vaddr);
         }
      }
   }
   return (uint64_t)NULL;
}
