/* *****************************************************************************
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
#include "elf.h"
#include "../../api/ascii/string.h"
#include "tuim_backend.h"
/* ------------------------------------
   templete for the tuim_load_segment function
   * Part of Tuim Project.
   * Last modified: July 08, 2025.
------------------------------------ */

#if 0
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Half  swap16
#define Swap_Word  swap32
#define Swap_Xword swap64

#if defined(USE_ELF32_TEMPLATE)
   #define LOAD_SEGMENTS  load_segments32
   #define GET_SHDR_ENTRY get_shdr_entry32
   #define GET_SECTION    get_section32

   #define Elf(x)    Elf32_##x
   #define Swap_Off  swap32
   #define Swap_Addr swap32
#elif defined(USE_ELF64_TEMPLATE)
   #define LOAD_SEGMENTS  load_segments64
   #define GET_SHDR_ENTRY get_shdr_entry64
   #define GET_SECTION    get_section64

   #define Elf(x)    Elf64_##x
   #define Swap_Off  swap64
   #define Swap_Addr swap64
#endif

#define FILE_OFFSET(offset) (void*)(obj + (offset))

static const void *
   GET_SHDR_ENTRY(const Elf(Byte) *obj, const Elf(Byte) *section);
static const void *
   GET_SECTION(const Elf(Byte) *obj, const Elf(Byte) *section);

static int LOAD_SEGMENTS(
   const tuim_ctx * const ctx,
   const Elf(Byte) * const obj,
   struct tuim_backend * const info){

   const Elf(Ehdr) * const ehdr = (void*)obj;
   const Elf(Phdr) *phdr;
   const Elf(Shdr) *shdr;

   Elf(Half) e_phentsize, e_phnum;
   Elf(Word) p_type, i;
   Elf(Off) e_phoff, sh_offset, p_offset;
   Elf(Addr) p_vaddr, start_vaddr, end_vaddr;
   Elf(Addr) program_image, segment;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word p_filesz, p_memsz, p_align;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword p_filesz, p_memsz, p_align;
   #endif

   bool first_time_here;
   size_t program_size;

   const Elf(Byte)
      dynamic_name[] = {46, 100, 121, 110, 97, 109, 105, 99, 00},
      dynstr_name[]  = {46, 100, 121, 110, 115, 116, 114, 0},
      synsym_name[]  = {46, 100, 121, 110, 115, 121, 109, 0};

   /* --- find useful sections --- */

   info->dyn     = GET_SECTION(info->obj, dynamic_name);
   info->dynstr  = GET_SECTION(info->obj, dynstr_name);
   shdr          = GET_SHDR_ENTRY(info->obj, synsym_name);
   sh_offset     = Swap_Off(shdr->sh_offset);
   info->dynsym  = info->obj + sh_offset;
   info->sh_info = Swap_Word(shdr->sh_info);
   #if defined(USE_ELF32_TEMPLATE)
      info->sh_size    = Swap_Word(shdr->sh_size);
      info->sh_entsize = Swap_Word(shdr->sh_entsize);
   #elif defined(USE_ELF64_TEMPLATE)
      info->sh_size    = Swap_Xword(shdr->sh_size);
      info->sh_entsize = Swap_Xword(shdr->sh_entsize);
   #endif

   /* --- Compute the size of program memory image --- */

   e_phoff = Swap_Off(ehdr->e_phoff);
   if(e_phoff == 0) return 0;
   e_phentsize = Swap_Half(ehdr->e_phentsize);
   e_phnum = Swap_Half(ehdr->e_phnum);

   first_time_here = true;

   for(i = 0; i < e_phnum; ++i){
      phdr = FILE_OFFSET(e_phoff + i * e_phentsize);

      p_type = Swap_Word(phdr->p_type);
      if(p_type != PT_LOAD) continue;

      p_vaddr = Swap_Addr(phdr->p_vaddr);

      if(first_time_here){
         start_vaddr = p_vaddr;
         #if defined(USE_ELF32_TEMPLATE)
            p_align  = Swap_Word(phdr->p_align);
         #elif defined(USE_ELF64_TEMPLATE)
            p_align  = Swap_Xword(phdr->p_align);
         #endif
         first_time_here = false;
      }

      #if defined(USE_ELF32_TEMPLATE)
         end_vaddr = p_vaddr + Swap_Word(phdr->p_memsz);
      #elif defined(USE_ELF64_TEMPLATE)
         end_vaddr = p_vaddr + Swap_Xword(phdr->p_memsz);
      #endif
   }
   program_size = end_vaddr - start_vaddr;

   /* --- Allocate memory for the program image --- */

   program_image = tuim_aligned_alloc(
      ctx, p_align, program_size);
   if(program_image == tuim_nullptr){
      return 1;
   }

   DBG("0x%llx\n", (unsigned long long)program_image);

   /* --- Read and load loadable segments --- */

   for(i = 0; i < e_phnum; ++i){
      phdr = FILE_OFFSET(e_phoff + i * e_phentsize);

      p_type = Swap_Word(phdr->p_type);
      if(p_type != PT_LOAD) continue;

      p_vaddr = Swap_Addr(phdr->p_vaddr);
      segment = program_image + (p_vaddr - start_vaddr);

      p_offset = Swap_Off(phdr->p_offset);
      #if defined(USE_ELF32_TEMPLATE)
         p_filesz = Swap_Word(phdr->p_filesz);
         p_memsz  = Swap_Word(phdr->p_memsz);
         p_align  = Swap_Word(phdr->p_align);
      #elif defined(USE_ELF64_TEMPLATE)
         p_filesz = Swap_Xword(phdr->p_filesz);
         p_memsz  = Swap_Xword(phdr->p_memsz);
         p_align  = Swap_Xword(phdr->p_align);
      #endif

      /* check if alignment match expected */
      if((segment % p_align) != 0){
         tuim_free(ctx, program_image);
         return 3;
      }

      /* copy the file segment to the machine */
      tuim_store(ctx, segment, obj + p_offset, p_filesz);
      if(p_memsz > p_filesz)
         tuim_memset(ctx, segment + p_filesz, 0, p_memsz - p_filesz);

      /* protect the memory */
      tuim_mprotect(ctx, segment, p_memsz, Swap_Word(phdr->p_flags));
   }

   info->program_image = program_image;
   info->program_size = program_size;
   info->start_vaddr = start_vaddr;
   info->auxiliary = NULL;

   return 0;
}

static const void *
GET_SHDR_ENTRY(const Elf(Byte) *obj, const Elf(Byte) *section){
   const Elf(Ehdr) * const ehdr = FILE_OFFSET(0x0);
   const Elf(Shdr) * shdr;
   const Elf(Byte) * shstrtab;
   Elf(Half) e_shstrndx, e_shentsize;
   Elf(Word) sh_name;
   Elf(Off)  e_shoff, sh_offset;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word  e_shnum, i;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword e_shnum, i;
   #endif

   e_shoff = Swap_Off(ehdr->e_shoff);
   if(e_shoff == 0) return NULL;
   shdr = FILE_OFFSET(e_shoff);

   e_shstrndx = Swap_Half(ehdr->e_shstrndx);
   if(e_shstrndx == SHN_UNDEF){
      return NULL;
   }else if(e_shstrndx == SHN_XINDEX){
      e_shstrndx = Swap_Word(shdr->sh_link);
   }

   e_shentsize = Swap_Half(ehdr->e_shentsize);
   shdr = FILE_OFFSET(e_shoff + e_shstrndx * e_shentsize);
   sh_offset = Swap_Off(shdr->sh_offset);
   shstrtab = FILE_OFFSET(sh_offset);

   e_shnum = Swap_Half(ehdr->e_shnum);
   if(e_shnum == 0){
      shdr = FILE_OFFSET(e_shoff);
      #if defined(USE_ELF32_TEMPLATE)
         e_shnum = Swap_Word(shdr->sh_size);
      #elif defined(USE_ELF64_TEMPLATE)
         e_shnum = Swap_Xword(shdr->sh_size);
      #endif
   }

   for(i = 1; i < e_shnum; ++i){
      shdr = FILE_OFFSET(e_shoff + i * e_shentsize);
      sh_name = Swap_Word(shdr->sh_name);
      if(ascii_strcmp(shstrtab + sh_name, section) != 0) continue;
      return shdr;
   }
   return NULL;
}

static const void *
GET_SECTION(const Elf(Byte) *obj, const Elf(Byte) *section){
   const Elf(Shdr) *shdr = GET_SHDR_ENTRY(obj, section);
   return FILE_OFFSET(Swap_Off(shdr->sh_offset));
}

#undef LOAD_SEGMENTS
#undef GET_SHDR_ENTRY
#undef GET_SECTION
#undef Elf
#undef Swap_Off
#undef Swap_Addr
