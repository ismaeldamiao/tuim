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
#include <stdlib.h>
/* ------------------------------------
   Function to load program segments on target machine.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */

#include "elf.h"
#include "../api/ascii/ascii.h"
#include "../api/ascii/string.h"

#include "tuim_backend.h"

static int get_dynamic(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr);
static int get_dynstr(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr);
static int get_dynsym(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr);
static int load(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr);

//int puts(const char *);

int tuim_load_segments(const tuim_ctx *ctx, const uint8_t *obj, void **ptr){
   struct tuim_backend info;

   info.obj = obj;
   return get_dynamic(ctx, &info, ptr);
}

static const void *tuim_get_shdr_entry(const uint8_t *obj, const uint8_t *section);
static const void *tuim_get_section(const uint8_t *obj, const uint8_t *section);

static int get_dynamic(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr){
   Elf(Byte) *section_name = ascii(".dynamic");
   info->dyn = tuim_get_section(info->obj, section_name);

   return get_dynstr(ctx, info, ptr);
}
static int get_dynstr(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr){
   Elf(Byte) *section_name = ascii(".dynstr");
   info->dynstr = tuim_get_section(info->obj, section_name);

   return get_dynsym(ctx, info, ptr);
}
static int get_dynsym(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr){
   const Elf(Byte) *section_name = ascii(".dynsym");
   const Elf(Shdr) *shdr = tuim_get_shdr_entry(info->obj, section_name);

   uint64_t sh_offset;

   sh_offset = (is_Elf32 ? swap32(shdr->sh_offset) : swap64(shdr->sh_offset));
   info->dynsym = info->obj + sh_offset;
   info->sh_info = swap32(shdr->sh_info);
   info->sh_size = (is_Elf32 ? swap32(shdr->sh_size) : swap64(shdr->sh_size));
   info->sh_entsize = (
      is_Elf32 ? swap32(shdr->sh_entsize) : swap64(shdr->sh_entsize));

   return load(ctx, info, ptr);
}

static int load(const tuim_ctx *ctx, struct tuim_backend *info, void **ptr){
   const Elf(Byte) *obj = (Elf(Byte) *)(info->obj);

   const Elf(Ehdr) *ehdr;
   const Elf(Phdr) *phdr;
   size_t e_phentsize;
   uint32_t p_type;
   uint64_t e_phoff, e_phnum, p_vaddr, p_memsz, p_filesz, p_offset;
   uint64_t start_vaddr, end_vaddr, program_size;
   bool first_time_here;
   Elf(Addr) program_image, segment;

   ehdr = (void*)obj;
   e_phoff = (is_Elf32 ? swap32(ehdr->e_phoff) : swap64(ehdr->e_phoff));
   if(e_phoff == UINT64_C(0)) return 0;

   e_phentsize = swap16(ehdr->e_phentsize);
   e_phnum = swap16(ehdr->e_phnum);

   /* Compute the size of program memory image */
   phdr = (void*)(obj + e_phoff);
   first_time_here = true;
   for(size_t i = SIZE_C(0); i < e_phnum; ++i){
      phdr = (void*)((uint8_t*)phdr + e_phentsize);

      p_type = swap32(phdr->p_type);
      if(p_type != PT_LOAD) continue;

      p_vaddr = (is_Elf32 ? swap32(phdr->p_vaddr) : swap64(phdr->p_vaddr));
      p_memsz = (is_Elf32 ? swap32(phdr->p_memsz) : swap64(phdr->p_memsz));

      if(first_time_here){
         start_vaddr = p_vaddr;
         first_time_here = false;
      }

      end_vaddr = p_vaddr + p_memsz;
   }
   program_size = end_vaddr - start_vaddr;

   /* Allocate memory for the program image */
   program_image = tuim_aligned_alloc(
      ctx, SIZE_C(0), program_size);
   if(program_image == tuim_nullptr){
      return 1;
   }

   /* Read and load loadable segments */
   phdr = (void*)(obj + e_phoff);
   for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_phnum); ++i){
      phdr = (void*)((Elf(Byte)*)phdr + e_phentsize);

      if(phdr->p_type != PT_LOAD) continue;

      p_type = swap32(phdr->p_type);
      if(p_type != PT_LOAD) continue;

      p_offset = (is_Elf32 ? swap32(phdr->p_offset) : swap64(phdr->p_offset));
      p_vaddr = (is_Elf32 ? swap32(phdr->p_vaddr) : swap64(phdr->p_vaddr));
      p_memsz = (is_Elf32 ? swap32(phdr->p_memsz) : swap64(phdr->p_memsz));
      p_filesz = (is_Elf32 ? swap32(phdr->p_filesz): swap64(phdr->p_filesz));

      segment = program_image + (p_vaddr - start_vaddr);
      tuim_store(ctx, segment, obj + p_offset, p_filesz);

      if(p_memsz > p_filesz){
         tuim_memset(ctx, segment + p_filesz, 0, p_memsz - p_filesz);
      }

      /* Protect the memory */
      tuim_mprotect(ctx, segment, p_memsz, phdr->p_flags);
   }

   info->program_image = program_image;
   info->program_size = program_size;
   info->start_vaddr = start_vaddr;
   info->auxiliary = NULL;

   {
      struct tuim_backend *info_tmp;

      info_tmp = malloc(sizeof(struct tuim_backend));
      if(info_tmp == NULL){
         tuim_free(ctx, program_image);
         return 1;
      }

      *info_tmp = *info;
      info = info_tmp;
   }

   *ptr = info;
   return 0;
}


static const void *tuim_get_shdr_entry(const uint8_t *obj, const uint8_t *section){
   const Elf(Ehdr) *ehdr;
   const Elf(Shdr) *shdr;
   size_t e_shnum, e_shentsize, sh_name, e_shstrndx;
   uint64_t e_shoff, sh_offset;
   const uint8_t *shstrtab;

   ehdr = (void*)obj;
   e_shoff = (uint64_t)(
      (offsetof(Elf(Ehdr), e_shoff) == offsetof(Elf32_Ehdr, e_shoff)) ?
      swap32(ehdr->e_shoff) : swap64(ehdr->e_shoff)
   );
   if(e_shoff == UINT64_C(0)) return NULL;
   shdr = (void*)(obj + e_shoff);

   e_shstrndx = swap16(ehdr->e_shstrndx);
   if(e_shstrndx == SHN_UNDEF){
      return NULL;
   }else if(e_shstrndx == SHN_XINDEX){
      e_shstrndx = swap32(shdr->sh_link);
   }

   e_shentsize = swap16(ehdr->e_shentsize);
   e_shnum = swap16(ehdr->e_shnum);
   if(e_shnum == SIZE_C(0)){
      e_shnum = swap32(shdr->sh_size);
   }

   sh_offset = (uint64_t)(
      (offsetof(Elf(Ehdr), e_shoff) == offsetof(Elf32_Ehdr, e_shoff)) ?
      swap32(((Elf(Shdr)*)((uint8_t*)shdr + e_shstrndx * e_shentsize))->sh_offset) :
      swap64(((Elf(Shdr)*)((uint8_t*)shdr + e_shstrndx * e_shentsize))->sh_offset)
   );
   shstrtab = obj + sh_offset;

   for(size_t i = SIZE_C(0); i < e_shnum; ++i){
      shdr = (void*)((uint8_t*)shdr + e_shentsize);
      sh_name = swap32(shdr->sh_name);
      if(ascii_strcmp(shstrtab + sh_name, section) != 0) continue;
      return shdr;
   }
   return NULL;
}

static const void *tuim_get_section(const uint8_t *obj, const uint8_t *section){
   const Elf(Shdr) *shdr = tuim_get_shdr_entry(obj, section);
   return obj + (is_Elf32 ? swap32(shdr->sh_offset) : swap64(shdr->sh_offset));
}
