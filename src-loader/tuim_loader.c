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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/tuim.h"
#include "elf.h"

#include "tuim_private.h"
#include "tuim_utils.h"
/* ------------------------------------
   Function to load in memory an PIE (shared object, ELF file).
   * Part of tuim project.
   * Last modified: December 7, 2024.
------------------------------------ */

//#define DBG(str, ...) fprintf(stdout, str, __VA_ARGS__)
#define DBG(str, ...)

#define STATIC static
#include "tuim_mprotect.c"
#include "tuim_pathsearch.c"
#include "aligned_alloc.c"

static bool haveslash(uint8_t *str){
   const uint8_t ascii_slash = UINT8_C(0x2f);
   for(; *str; ++str)
      if(*str == ascii_slash) return true;
   return false;
}

/* load_program:
   - Compute the size of the program memory image using the diference
     from the last virtual address byte and the first virtual address byte of
     loadable segments.
   - Allocate memory for the program memory image and copy the file image.
   - Initialize elf->segments and elf->start_vaddr. */

static void* load_program(
   unsigned int e_phnum, Elf_Phdr *phdr, FILE *file_ptr, tuim_elf *elf
){
   void *program, *buf;
   size_t prog_size, prog_alignment;
   uintptr_t start_vaddr, end_vaddr;

   /* Compute the program size */
   prog_alignment = (size_t)0;
   for(unsigned int i = 0U; i < e_phnum; ++i){
      uintptr_t p_vaddr;
      size_t p_memsz;

      if(ELF_P_TYPE(phdr[i]) != PT_LOAD) continue;

      p_vaddr = ELF_P_VADDR(phdr[i]);
      p_memsz = (size_t)ELF_P_MEMSZ(phdr[i]);

      if(prog_alignment == (size_t)0){
         prog_alignment = (size_t)ELF_P_ALIGN(phdr[i]);
         start_vaddr = p_vaddr;
      }
      end_vaddr = p_vaddr + (uintptr_t)p_memsz;
   }

   /* Allocate memory */
   prog_size = (size_t)(end_vaddr - start_vaddr);
   program = aligned_alloc(prog_alignment, prog_size);
   tuim_mprotect(program, prog_size, (int)(PF_R | PF_W | PF_X));

   /* Read loadable segments */
   for(unsigned int i = 0U; i < e_phnum; ++i){
      uintptr_t p_offset, p_vaddr;
      size_t p_memsz, p_filesz;

      p_vaddr = ELF_P_VADDR(phdr[i]);
      elf->segments[i] = buf = (uint8_t*)program + p_vaddr - start_vaddr;

      if(ELF_P_TYPE(phdr[i]) != PT_LOAD) continue;

      p_memsz = (size_t)ELF_P_MEMSZ(phdr[i]);
      p_filesz = (size_t)ELF_P_FILESZ(phdr[i]);
      p_offset = ELF_P_OFFSET(phdr[i]);

      if(fseek(file_ptr, (long)p_offset, SEEK_SET) != 0){
         free(program);
         tuim_error = TUIM_ERROR_READING;
         return NULL;
      }

      if(fread(buf, sizeof(uint8_t), p_filesz, file_ptr) != p_filesz){
         free(program);
         tuim_error = TUIM_ERROR_READING;
         return NULL;
      }

      if(p_memsz > p_filesz)
         memset((uint8_t*)buf + p_filesz, 0, p_memsz - p_filesz);
   }
   elf->start_vaddr = start_vaddr;
   return program;
}

/* Load:
   - Allocate size bytes of memory and copy size bytes at the offset
     from file pointed by file_ptr. */
static void* load(uintptr_t offset, size_t size, FILE *file_ptr){
   void* data;

   data = malloc(size);
   if(data == NULL){
      tuim_error = TUIM_ERROR_MEMORY;
      return NULL;
   }

   if(fseek(file_ptr, (long)offset, SEEK_SET) != 0){
      tuim_error = TUIM_ERROR_READING;
      free(data);
      return NULL;
   }

   if(fread(data, sizeof(uint8_t), size, file_ptr) != size){
      tuim_error = TUIM_ERROR_READING;
      return NULL;
   }

   return data;
}

tuim_elf* tuim_loader(const char *file_path){
   size_t dyn_num, status;
   FILE *file_ptr;

   Elf_Ehdr *ehdr = NULL;
   Elf_Phdr *phdr = NULL;
   Elf_Shdr *shdr = NULL;
   Elf_Dyn *dyn = NULL;

   tuim_elf *elf = NULL;

   tuim_error_filename = (char*)file_path;

   /* --------------------------------------------------------------------------
   In memory loading
   -------------------------------------------------------------------------- */
   /* Open the ELF to read */
   file_ptr = fopen(file_path, "rb");
   if(file_ptr == NULL){
      tuim_error = TUIM_ERROR_READING;
      goto error;
   }

   /* Allocate memory to elf structure and add it to a list of
      files loaded. */
   elf = malloc(sizeof(tuim_elf));
   if(elf == NULL){
      tuim_error = TUIM_ERROR_MEMORY;
      goto error;
   }
   elf->ehdr = ehdr;
   elf->phdr = phdr;
   elf->shdr = shdr;

   DBG("%s: loading the ELF header.\n", file_path);

   /* Load the ELF header e_ident field */
   ehdr = malloc(sizeof(Elf_Ehdr));
   if(ehdr == NULL){
      tuim_error = TUIM_ERROR_MEMORY;
      goto error;
   }
   elf->ehdr = ehdr;

   status = fread(
      *ehdr,
      sizeof(uint8_t),
      EI_NIDENT,
      file_ptr
   );
   if(status != EI_NIDENT){
      tuim_error = TUIM_ERROR_READING;
      goto error;
   }else if(
      (ELF_EI_MAG0(*ehdr) != ELFMAG0) ||
      (ELF_EI_MAG1(*ehdr) != ELFMAG1) ||
      (ELF_EI_MAG2(*ehdr) != ELFMAG2) ||
      (ELF_EI_MAG3(*ehdr) != ELFMAG3)
   ){
      tuim_error = TUIM_ERROR_INVALIDELF;
      goto error;
   }else if(ELF_EI_CLASS(*ehdr) != ELFCLASS){
      tuim_error = TUIM_ERROR_MACHINE;
      goto error;
   }else if(ELF_EI_DATA(*ehdr) != ELFDATA){
      tuim_error = TUIM_ERROR_MACHINE;
      goto error;
   }else if(ELF_EI_VERSION(*ehdr) != EV_CURRENT){
      tuim_error = TUIM_ERROR_INVALIDELF;
      goto error;
   }

   /* Load the ELF header */
   status = fread(
      *ehdr + EI_NIDENT,
      sizeof(uint8_t),
      sizeof(Elf_Ehdr) - EI_NIDENT,
      file_ptr
   );
   if(status != (sizeof(Elf_Ehdr) - EI_NIDENT)){
      tuim_error = TUIM_ERROR_READING;
      goto error;
   }else if((ELF_E_TYPE(*ehdr) != ET_DYN) && (ELF_E_TYPE(*ehdr) != ET_EXEC)){
      tuim_error = TUIM_ERROR_NOTDYN;
      goto error;
   }else if(ELF_E_MACHINE(*ehdr) != EM_){
      tuim_error = TUIM_ERROR_MACHINE;
      goto error;
   }else if(ELF_E_VERSION(*ehdr) != EV_CURRENT){
      tuim_error = TUIM_ERROR_INVALIDELF;
      goto error;
   }

   /* Parse the Program Header table and load the segments. */
   {
      uint16_t e_phnum;
      e_phnum = ELF_E_PHNUM(*ehdr);

      if(e_phnum != UINT16_C(0)){
         uint16_t e_phentsize;
         uintptr_t e_phoff;

         e_phentsize = ELF_E_PHENTSIZE(*ehdr);
         e_phoff = ELF_E_PHOFF(*ehdr);

         if((size_t)e_phentsize < sizeof(Elf_Phdr)){
            tuim_error = TUIM_ERROR_INVALIDELF;
            goto error;
         }

         /* Parse the program header table. */
         DBG("%s: loading the program header table.\n", file_path);
         elf->phdr = phdr = load(
            e_phoff, sizeof(Elf_Phdr) * (size_t)(e_phnum), file_ptr
         );
         if(phdr == NULL)
            goto error;

         elf->segments = malloc(sizeof(void*) * (size_t)(e_phnum));
         if(elf->segments == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            free(phdr);
            elf->phdr = NULL;
            goto error;
         }

         /* Load PT_LOAD segments:
            Find for loadable segments and load it. */
         DBG("%s: loading the program memory image at ", file_path);
         elf->program = load_program(e_phnum, phdr, file_ptr, elf);
         DBG("%p.\n", elf->program);
         if(elf->program == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            free(phdr);
            elf->phdr = NULL;
            goto error;
         }
      }
   }

   DBG("%s: loading the section header table.\n", file_path);

   /* Parse the Section Header table and map sections. */
   {
      uint16_t e_shnum;
      e_shnum = ELF_E_SHNUM(*ehdr);

      if(e_shnum != UINT16_C(0)){
         uint16_t e_shentsize;
         uintptr_t e_shoff;

         e_shentsize = ELF_E_SHENTSIZE(*ehdr);
         e_shoff = ELF_E_SHOFF(*ehdr);

         if(e_shentsize < sizeof(Elf_Shdr)){
            tuim_error = TUIM_ERROR_INVALIDELF;
            goto error;
         }

         /* Parse the section header table. */
         elf->shdr = shdr = load(
            e_shoff, sizeof(Elf_Shdr) * (size_t)e_shnum, file_ptr
         );
         if(shdr == NULL)
            goto error;

         elf->sections = malloc(sizeof(void*) * (size_t)e_shnum);
         if(elf->sections == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            free(shdr);
            elf->shdr = NULL;
            goto error;
         }
         elf->sections[0] = NULL;

         for(uint16_t i = UINT16_C(1); i < e_shnum; ++i){
            size_t sh_size;
            uint16_t e_phnum;
            uint32_t sh_flags;

            sh_size = (size_t)ELF_SH_SIZE(shdr[i]);
            elf->sections[i] = NULL;

            if(sh_size == (size_t)0) continue;

            e_phnum = ELF_E_PHNUM(*ehdr);
            sh_flags = ELF_SH_FLAGS(shdr[i]);

            if(sh_flags & SHF_ALLOC){
               /* Map section data from segments. */
               uintptr_t sh_addr;
               sh_addr = ELF_SH_ADDR(shdr[i]);
               elf->sections[i] =
               (uint8_t*)(elf->program) + sh_addr - elf->start_vaddr;
            }else{
               /* Otherwise copy it from the file. */
               if(ELF_SH_TYPE(shdr[i]) == SHT_NOBITS){
                  elf->sections[i] = malloc(sh_size);
                  if(elf->sections[i] == NULL){
                     tuim_error = TUIM_ERROR_MEMORY;
                     goto error;
                  }
               }else{
                  uintptr_t sh_offset;
                  sh_offset = ELF_SH_OFFSET(shdr[i]);

                  elf->sections[i] = load(
                     sh_offset, sh_size, file_ptr
                  );
                  if(elf->sections[i] == NULL)
                     goto error;
               }
               DBG(
                  "%s:   section %02d loaded at %p.\n",
                  file_path, i, elf->sections[i]
               );
            }
         }
      }
   }

   fclose(file_ptr);

   DBG("%s: dynamic linking...\n", file_path);

   /* Find for the PT_DYNAMIC segment */
   {
      unsigned int e_phnum;
      e_phnum = (unsigned int)ELF_E_PHNUM(*ehdr);
      for(unsigned int i = 0U; i < e_phnum; ++i){
         uint32_t p_type;

         p_type = ELF_P_TYPE(phdr[i]);
         if(p_type == PT_LOAD) continue;

         if(p_type == PT_DYNAMIC){
            size_t p_memsz;
            dyn = elf->segments[i];
            p_memsz = (size_t)ELF_P_MEMSZ(phdr[i]);
            dyn_num = p_memsz / sizeof(Elf_Dyn);
         }
      }
   }

   /* --------------------------------------------------------------------------
   Load dependencies
   -------------------------------------------------------------------------- */

   if(dyn != NULL){
      size_t d_tag;
      uint8_t *strtab;

      // Find for the string table
      for(uint16_t i = UINT16_C(0); i < dyn_num; ++i)
         if((d_tag = ELF_D_TAG(dyn[i]), (d_tag == DT_STRTAB))){
            uintptr_t d_ptr;
            d_ptr = ELF_D_UN(dyn[i]);
            strtab = (uint8_t*)(elf->program) + d_ptr - elf->start_vaddr;
            break;
         }

      // Loop on dynamic section entries
      for(uint16_t i = UINT16_C(0); i < dyn_num; ++i)
      if((d_tag = ELF_D_TAG(dyn[i]), (d_tag == DT_NEEDED))){
         size_t d_val;
         uint8_t *soname;
         tuim_elf *dep;
         elf_list *node;

         d_val = ELF_D_UN(dyn[i]);
         soname = strtab + d_val;
         if(!haveslash(soname)){
            /* When the name not contain any slash I need to find for the
               shared object on LD_LIBRARY_PATH and on predefined places. */
            char **sonames, **sonames_bak;

            sonames = sonames_bak = path_search(soname);
            while(*sonames){
               dep = list_find(tuim_loaded, *sonames);
               if(dep == NULL)
                  dep = tuim_loader(*sonames);
               if(dep) break;
               free(*sonames);
               ++sonames;
            }
            while(*sonames){
               free(*sonames);
               ++sonames;
            }
            free(sonames_bak);
            if(dep == NULL)
               goto error;
         }else{
            dep = list_find(tuim_loaded, str(soname));
            if(dep == NULL)
               dep = tuim_loader(str(soname));
            if(dep == NULL)
               goto error;
         }

         node = list_add((void*)(elf->sections), str(soname), dep);
         if(node == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            goto error;
         }
      }
   }

   /* --------------------------------------------------------------------------
   Relocation
   -------------------------------------------------------------------------- */
   DBG("%s: applying relocations.\n", file_path);
   if(dyn){
      size_t d_tag;
      // Loop on dynamic section entries
      for(uint16_t i = UINT16_C(0); i < dyn_num; ++i){
         void *reltab; // Handle both Elf_Rel[] and Elf_Rela[]
         size_t relnum;
         uint8_t *rel_base_addrs, *strtab;
         uint16_t rel_shndx;
         uint32_t sh_info, sh_link, sh_type;
         uintptr_t d_ptr;
         Elf_Sym *symtab;

         d_tag = ELF_D_TAG(dyn[i]);
         if((d_tag != DT_REL) && (d_tag != DT_RELA) && (d_tag != DT_JMPREL))
            continue;

         // The relocation table
         d_ptr = ELF_D_UN(dyn[i]); // reltab virtual address
         rel_shndx = shndx(elf, d_ptr); // reltab section index
         sh_type = ELF_SH_TYPE(shdr[rel_shndx]);
         reltab = elf->sections[rel_shndx];

         // The associated symbol table
         sh_link = ELF_SH_LINK(shdr[rel_shndx]);
         symtab = elf->sections[sh_link];

         // The associated string table
         sh_link = ELF_SH_LINK(shdr[sh_link]);
         strtab = elf->sections[sh_link];

         // The section to which relocation applies
         sh_info = ELF_SH_INFO(shdr[rel_shndx]);

         relnum = (size_t)ELF_SH_SIZE(shdr[rel_shndx]) / ((sh_type == SHT_REL) ?
            sizeof(Elf_Rel) : sizeof(Elf_Rela)
         );

         // Loop on relocation table entries
         for(size_t j = UINT32_C(0); j < relnum; ++j){
            void *rel;
            uint32_t r_type, r_sym;
            uintptr_t r_offset;

            if(sh_type == SHT_REL)
               rel = (*(((Elf_Rel*)reltab) + j));
            else
               rel = (*(((Elf_Rela*)reltab) + j));

            r_offset = ELF_R_OFFSET((uint8_t*)rel);
            r_sym = (uint32_t)ELF_R_SYM((uint8_t*)rel);
            r_type = (uint32_t)ELF_R_TYPE((uint8_t*)rel);

            /* At this point, the only TODO is apply relocations,
               this is a processor dependent step.
               Since this is a dynamic linker only dynamic relocations
               must be implemented. */
            {
#if defined(__ARM_ARCH)
#if __ARM_ARCH >= 8
/* ------------------------------------
AARCH64 relocations
------------------------------------ */
   #error "Not implemented yet."
#elif __ARM_ARCH <= 7
/* ------------------------------------
AARCH32 relocations
------------------------------------ */
   int32_t A;
   uint16_t st_shndx;
   uint32_t B, P, S;
   Elf_Sym *sym;

   // Relocation types with no action
   if((r_type == R_ARM_TLS_DESC) || (r_type == R_ARM_COPY)) continue;
   // Support to outdated linkers
   if(r_type == R_ARM_ABS32) r_type = R_ARM_GLOB_DAT;

   // Address of the symbol reference (place)
   //P = (uint32_t)(segment_base_addr(elf, phndx(elf, r_offset)) + r_offset);
   P = (uint32_t)(elf->program) + r_offset - elf->start_vaddr;

   /* From _ELF for the Arm Architecture_:
      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits.
   */
   // The addend
   A = ((sh_type == SHT_REL) ? *((int32_t*)P) : ELF_R_ADDEND(*(Elf_Rela*)rel));

   // Base address of the symbol definition
   if(r_sym != UINT32_C(0)){
      sym = symtab + r_sym;
      st_shndx = ELF_ST_SHNDX(*sym);
      if(st_shndx != STN_UNDEF){
         B = (uint32_t)(elf->program) - elf->start_vaddr;
      }else{
         uint32_t st_name;
         tuim_elf *dep = NULL;

         // The symbol is not defined in this file, find it on dependencies.
         st_name = ELF_ST_NAME(*sym);
         findsymbol(&dep, &sym, strtab + st_name, elf->sections[0]);

         if(dep == NULL){
            goto error;
         }

         st_shndx = ELF_ST_SHNDX(*sym);
         B = (uint32_t)(dep->program) - dep->start_vaddr;
      }
   }

   // Relocation types that do not need symbol value
   if(r_type == R_ARM_RELATIVE){
      /* RELATIVE relocations with undefined symbols use the addend
         as the address of symbols definition. */
      *((uint32_t*)P) = B + A;
      DBG(
         "%s:   adress 0x%08x now holds 0x%08x.\n",
         file_path, P, *((uint32_t*)P)
      );
      continue;
   }

   // Address of the symbol definition
   S = B + ELF_ST_VALUE(*sym);

   if(r_type == R_ARM_BREL_ADJ){
      // TODO:
      *((uint32_t*)P) = /* ChangeIn[B(S)] + */ A;
   }else if(r_type == R_ARM_TLS_DTPMOD32){
      // TODO:
      *((uint32_t*)P) = /*Module[*/ S /*]*/;
   }else if(r_type == R_ARM_TLS_DTPOFF32){
      // TODO:
      *((uint32_t*)P) = S + A /* - TLS */;
   }else if(r_type == R_ARM_TLS_TPOFF32){
      // TODO:
      *((uint32_t*)P) = S + A /* - tp */;
   }else if(r_type == R_ARM_GLOB_DAT){
      uint32_t T;
      if(ELF_ST_TYPE(*sym) == STT_FUNC)
         T = S & UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);

      *((uint32_t*)P) = (S + A) | T;
      DBG(
         "%s:   adress 0x%08x now holds 0x%08x.\n",
         file_path, P, *((uint32_t*)P)
      );
   }else if(r_type == R_ARM_JUMP_SLOT){
      uint32_t T;
      if(sh_type == SHT_REL) A = INT32_C(0);
      if(ELF_ST_TYPE(*sym) == STT_FUNC)
         T = S & UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);
      *((uint32_t*)P) = (S + A) | T;
   }else{
      tuim_error = TUIM_ERROR_UNSUPPORTED_RT;
      goto error;
   }
   DBG(
      "%s:   adress 0x%08x now holds 0x%08x.\n",
      file_path, P, *((uint32_t*)P)
   );
#endif // __ARM_ARCH >= 8
#elif defined(__x86_64__)
/* ------------------------------------
AMD64 relocations
------------------------------------ */
   int64_t A;
   uint16_t st_shndx;
   uint64_t S, G, L, Z;
   uintptr_t P, B, GOT;
   Elf_Sym *sym;

   // Address of the symbol reference (place)
   P = (uintptr_t)(segment_base_addr(elf, phndx(elf, r_offset)) + r_offset);

#if defined(__LP64__)
   A = ELF_R_ADDEND(*(Elf_Rela*)rel);
#endif // defined(__LP64__)

   // Base address of the symbol definition
   if(r_sym != UINT32_C(0)){
      sym = symtab + r_sym;
      st_shndx = ELF_ST_SHNDX(*sym);
      if(st_shndx != STN_UNDEF){
         B = (uintptr_t)section_base_addr(elf, st_shndx);
      }else{
         uint32_t st_name;
         tuim_elf *dep = NULL;

         // The symbol is not defined in this files, find it on dependencies.
         st_name = ELF_ST_NAME(*sym);
         findsymbol(&dep, &sym, strtab + st_name, elf->sections[0]);

         if(dep == NULL){
            goto error;
         }

         st_shndx = ELF_ST_SHNDX(*sym);
         B = (uintptr_t)section_base_addr(dep, st_shndx);
      }
   }

   if(r_type == R_X86_64_RELATIVE){
#if defined(__ILP32__)
      A = ((d_tag == DT_REL) ?
         (int64_t)(*((int32_t*)P)) : ELF_R_ADDEND(*(Elf_Rela*)rel)
      );
#endif // defined(__ILP32__)
      /* RELATIVE relocations with undefined symbols use the addend
         as the address of symbols definition. */
      if(r_sym == UINT32_C(0))
         B = (uintptr_t)segment_base_addr(elf, phndx(elf, A));
      *((wordclass*)P) = (wordclass)((uint64_t)B + A);
      continue;
   }
#if defined(__ILP32__)
   if(r_type == R_X86_64_RELATIVE64){
      A = ((d_tag == DT_REL) ?
         (int64_t)(*((int64_t*)P)) : ELF_R_ADDEND(*(Elf_Rela*)rel)
      );
      if(r_sym == UINT32_C(0))
         B = (uintptr_t)segment_base_addr(elf, phndx(elf, A));
      *((uint64_t*)P) = (uint64_t)B + A;
      continue;
   }
#endif // defined(__ILP32__)

   /* word8 relocations */
   if(
      (r_type == R_X86_64_8) || (r_type == R_X86_64_PC8)
   ){

#if defined(__ILP32__)
      A = ((d_tag == DT_REL) ?
         (int64_t)(*((int8_t*)P)) : ELF_R_ADDEND(*(Elf_Rela*)rel)
      );
#endif // defined(__ILP32__)
   }

#elif defined(__i386__)
/* ------------------------------------
i386 relocations
------------------------------------ */
   #error "Not implemented yet."
#elif defined(__riscv)
#if __riscv_xlen == 64
/* ------------------------------------
RISC-V 64 relocations
------------------------------------ */
   #error "Not implemented yet."
#elif __riscv_xlen == 32
/* ------------------------------------
RISC-V 32 relocations
------------------------------------ */
   #error "Not implemented yet."
#endif // __riscv_xlen == 64
#else
#error "Architecture not supported."
#endif // defined(__ARM_ARCH)
            }
         }
      }
   }

   /* --------------------------------------------------------------------------
   Correct the memory protection
   -------------------------------------------------------------------------- */
   DBG("%s: protecting the memory.\n", file_path);
   {
      uint16_t e_phnum;

      e_phnum = ELF_E_PHNUM(*ehdr);

      for(uint16_t i = UINT16_C(0); i < e_phnum; ++i){
         size_t p_memsz;
         uint32_t p_flags;

         if(ELF_P_TYPE(phdr[i]) != PT_LOAD) continue;

         p_flags = ELF_P_FLAGS(phdr[i]);
         p_memsz = (size_t)ELF_P_MEMSZ(phdr[i]);

         p_flags &= UINT32_C(0x00000007);

         tuim_mprotect(elf->segments[i], p_memsz, (int)p_flags);
      }
   }

   tuim_error = TUIM_NO_ERROR;

   return elf;

   error:
   tuim_free(elf);
   return NULL;
}
