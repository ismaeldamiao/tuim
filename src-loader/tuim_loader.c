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

#include "tuim_utils.h"
/* ------------------------------------
   Function to load in memory an PIE (shared object, ELF file).
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

//#define DBG(str, ...) fprintf(stdout, str, __VA_ARGS__)
#define DBG(str, ...)

#define STATIC static
#include "tuim_mprotect.c"
#include "tuim_pathsearch.c"

#ifdef __ANDROID_API__
#if __ANDROID_API__ < 28
static void* aligned_alloc(size_t alignment, size_t size){
   void *buf;
   if(posix_memalign(&buf, alignment, size) == 0)
      return buf;
   else
      return NULL;
};
#endif /* __ANDROID_API__ >= 28 */
#endif /* __ANDROID_API__ */

static bool haveslash(uint8_t *str){
   /* Value of '/' in the ASCII chart is 0x2f */
   for(; *str; ++str)
      if(*str == UINT8_C(0x2f)) return true;
   return false;
}

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

static void* load_segment(
   uintptr_t p_offset, size_t p_align, size_t p_filesz, size_t p_memsz, FILE *file_ptr
){
   void* segment;

   segment = aligned_alloc(p_align, p_memsz);
   if(segment == NULL){
      tuim_error = TUIM_ERROR_MEMORY;
      return NULL;
   }

   if(fseek(file_ptr, (long)p_offset, SEEK_SET) != 0){
      free(segment);
      tuim_error = TUIM_ERROR_READING;
      return NULL;
   }
   
   if(fread(segment, sizeof(uint8_t), p_filesz, file_ptr) != p_filesz){
      free(segment);
      tuim_error = TUIM_ERROR_READING;
      return NULL;
   }

   if(p_memsz > p_filesz)
      memset((uint8_t*)segment + p_filesz, 0, p_memsz - p_filesz);

   return segment;
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
   {
      elf_list *node;
      node = list_add(&tuim_loaded, file_path, elf);
      if(node == NULL){
         free(elf);
         elf = NULL;
         tuim_error = TUIM_ERROR_MEMORY;
         goto error;
      }
   }

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
   }else if(ELF_E_TYPE(*ehdr) != ET_DYN){
      tuim_error = TUIM_ERROR_NOTDYN;
      goto error;
   }else if(ELF_E_MACHINE(*ehdr) != EM_){
      tuim_error = TUIM_ERROR_MACHINE;
      goto error;
   }else if(ELF_E_VERSION(*ehdr) != EV_CURRENT){
      tuim_error = TUIM_ERROR_INVALIDELF;
      goto error;
   }

   DBG("%s: loading the program header table.\n", file_path);

   /* Parse the Program Header table and load the segments. */
   {
      uint16_t e_phnum;
      e_phnum = ELF_E_PHNUM(*ehdr);

      if(e_phnum != UINT16_C(0)){
         size_t phdr_size;
         uint16_t e_phentsize;
         uintptr_t e_phoff;

         e_phentsize = ELF_E_PHENTSIZE(*ehdr);
         e_phoff = ELF_E_PHOFF(*ehdr);

         if((size_t)e_phentsize < sizeof(Elf_Phdr)){
            tuim_error = TUIM_ERROR_INVALIDELF;
            goto error;
         }

         /* Parse the program header table. */
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
         for(uint16_t i = UINT16_C(0); i < e_phnum; ++i){
            size_t p_memsz, p_filesz, p_align;
            uintptr_t p_offset;
            
            if(ELF_P_TYPE(phdr[i]) != PT_LOAD) continue;
            
            p_memsz = (size_t)ELF_P_MEMSZ(phdr[i]);
            if(p_memsz == (size_t)0) continue;

            p_align = (size_t)ELF_P_ALIGN(phdr[i]);
            p_filesz = (size_t)ELF_P_FILESZ(phdr[i]);
            p_offset = ELF_P_OFFSET(phdr[i]);

            elf->segments[i] = load_segment(
               p_offset, p_align, p_filesz, p_memsz, file_ptr
            );
            if(elf->segments[i] == NULL){
               tuim_error = TUIM_ERROR_READING;
               goto error;
            }
         }

         /* Check for embedded segments:
            Check if any other segments start and end within the boundaries
            of a PT_LOAD segment. If they do, simply point to the address
            where they are, without loading them again. If they don't, 
            load it. */
         for(uint16_t i = UINT16_C(0); i < e_phnum; ++i){
            size_t p_memsz;
            uint32_t p_type;
            uintptr_t p_vaddr;

            p_type = ELF_P_TYPE(phdr[i]);
            if(p_type == PT_LOAD) continue;

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
                  elf->segments[i] =
                  (uint8_t*)(elf->segments[j]) - pp_vaddr + p_vaddr;
                  break;
               }
            }
            if(elf->segments[i] == NULL){
               uintptr_t p_offset;
               size_t p_align, p_filesz;

               p_align = ELF_P_ALIGN(phdr[i]);
               p_filesz = ELF_P_FILESZ(phdr[i]);
               p_offset = ELF_P_OFFSET(phdr[i]);

               elf->segments[i] = load_segment(
                  p_offset, p_align, p_filesz, p_memsz, file_ptr
               );
               if(elf->segments[i] == NULL)
                  goto error;
            }
            if(p_type == PT_DYNAMIC){
               dyn = elf->segments[i];
               dyn_num = p_memsz / sizeof(Elf_Dyn);
            }
         }
      }
   }

   DBG("%s: loading the section header table.\n", file_path);

   /* Parse the Section Header table and map sections. */
   {
      uint16_t e_shnum;
      e_shnum = ELF_E_SHNUM(*ehdr);

      if(e_shnum != UINT16_C(0)){
         size_t shdr_size;
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
            uintptr_t sh_addr;

            sh_size = (size_t)ELF_SH_SIZE(shdr[i]);
            elf->sections[i] = NULL;

            if(sh_size == (size_t)0) continue;

            e_phnum = ELF_E_PHNUM(*ehdr);
            sh_addr = ELF_SH_ADDR(shdr[i]);
            sh_flags = ELF_SH_FLAGS(shdr[i]);

            /* Try to map the section data from segments */
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
                     elf->sections[i] =
                     (uint8_t*)(elf->segments[j]) - p_vaddr + sh_addr;
                     break;
                  }
               }
            }

            /* Otherwise copy it from the file. */
            if(elf->sections[i] != NULL) continue;

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
         }
      }
   }

   fclose(file_ptr);

   /* --------------------------------------------------------------------------
   Load dependencies
   -------------------------------------------------------------------------- */
   if(dyn){
      size_t d_tag;
      uint8_t *strtab;

      DBG("%s: loading dependencies.\n", file_path);

      // Find for the string table
      for(uint16_t i = UINT16_C(0); i < dyn_num; ++i)
         if((d_tag = ELF_D_TAG(dyn[i]), (d_tag == DT_STRTAB))){
            uint16_t j;
            uintptr_t d_ptr;
            d_ptr = ELF_D_UN(dyn[i]);
            j = phndx(elf, d_ptr);
            strtab = (uint8_t*)(elf->segments[j]) + d_ptr;
            break;
         }

      // Loop on dynamic section entries
      for(uint16_t i = UINT16_C(0); i < dyn_num; ++i)
      if((d_tag = ELF_D_TAG(dyn[i]), (d_tag == DT_NEEDED))){
         size_t d_val;
         uint8_t *soname;
         tuim_elf *dep;
         elf_list *dependencies, *node;

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
      for(uint16_t i = UINT16_C(0); i < dyn_num; ++i)
      if((d_tag = ELF_D_TAG(dyn[i]), (d_tag == DT_REL) || (d_tag == DT_RELA))){
         Elf_Shdr *rel_sec;
         void *reltab; // Handle both Elf_Rel[] and Elf_Rela[]
         uint8_t *rel_base_addrs, *strtab;
         uint16_t rel_shndx;
         Elf_Sym *symtab;
         uint32_t sh_info, sh_link;
         size_t relnum;
         uintptr_t d_ptr;

         // The relocation table
         d_ptr = ELF_D_UN(dyn[i]); // It virtual address
         rel_shndx = shndx(elf, d_ptr); // It section index
         reltab = elf->sections[rel_shndx];

         // The associated symbol table
         sh_link = ELF_SH_LINK(shdr[rel_shndx]);
         symtab = elf->sections[sh_link];

         // The associated string table
         strtab = elf->sections[ELF_SH_LINK(shdr[sh_link])];

         // The section to which relocation applies
         sh_info = ELF_SH_INFO(shdr[rel_shndx]);
         rel_base_addrs = ( (sh_info != UINT32_C(0)) ?
            section_base_addr(elf, sh_info) :
            section_base_addr(elf, rel_shndx)
         );

         relnum = (size_t)ELF_SH_SIZE(shdr[rel_shndx]) / ((d_tag == DT_REL) ?
            sizeof(Elf_Rel) : sizeof(Elf_Rela)
         );

         // Loop on relocation table entries
         for(uint32_t j = UINT32_C(0); j < relnum; ++j){
            void *rel;
            Elf_Sym *sym;
            uintN_t st_value, r_type, r_sym;
            intptr_t r_offset;

            if(d_tag == DT_REL)
               rel = (*(((Elf_Rel*)reltab) + j));
            else
               rel = (*(((Elf_Rela*)reltab) + j));

            r_offset = ELF_R_OFFSET(rel);
            r_sym = ELF_R_SYM(rel);
            r_type = ELF_R_TYPE(rel);

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
   // TODO:
#elif __ARM_ARCH <= 7
/* ------------------------------------
AARCH32 relocations
------------------------------------ */
   int32_t A;
   uint16_t st_shndx;
   uint32_t B, P, S;

   // Relocation types with no action
   if((r_type == R_ARM_TLS_DESC) || (r_type == R_ARM_COPY)) continue;
   // Support to outdated linkers
   if(r_type == R_ARM_ABS32) r_type = R_ARM_GLOB_DAT;

   // Address of the symbol reference (place)
   P = (uint32_t)(segment_base_addr(elf, phndx(elf, r_offset)) + r_offset);

   /* From _ELF for the Arm Architecture_:
      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits.
   */
   // The addend
   A = ((d_tag == DT_REL) ? *((int32_t*)P) : ELF_R_ADDEND(rel));

   // Base address of the symbol definition
   if(r_sym != UINT32_C(0)){
      sym = symtab + r_sym;
      st_shndx = ELF_ST_SHNDX(*sym);
      if(st_shndx != STN_UNDEF){
         B = (uint32_t)section_base_addr(elf, st_shndx);
      }else{
         uint32_t st_name;
         tuim_elf *dep = NULL;

         st_name = ELF_ST_NAME(*sym);
         findsymbol(&dep, &sym, strtab + st_name, elf->sections[0]);

         if(dep == NULL){
            goto error;
         }

         st_shndx = ELF_ST_SHNDX(*sym);
         B = (uint32_t)section_base_addr(dep, st_shndx);
      }
   }else{
      B = (uint32_t)rel_base_addrs; // ??
   }

   // Relocation types that do not need symbol value
   if(r_type == R_ARM_RELATIVE){
      if(r_sym == UINT32_C(0))
         B = (uint32_t)segment_base_addr(elf, phndx(elf, A));
      *((uint32_t*)P) = B + A;
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
      if((ELF_ST_TYPE(*sym) == STT_FUNC) && (S & UINT32_C(0x00000001)))
         T = UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);

      *((uint32_t*)P) = (S + A) | T;
   }else if(r_type == R_ARM_JUMP_SLOT){
      uint32_t T;
      if((ELF_ST_TYPE(*sym) == STT_FUNC) && (S & UINT32_C(0x00000001)))
         T = UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);

      *((uint32_t*)P) = (S + A) | T;
   }else{
      tuim_error = TUIM_ERROR_UNSUPPORTED_RT;
      goto error;
   }
#endif // __ARM_ARCH >= 8
#elif defined(__x86_64__)
/* ------------------------------------
X86-64 relocations
------------------------------------ */
   // TODO:
#elif defined(__i386__)
/* ------------------------------------
i386 relocations
------------------------------------ */
   // TODO:
#elif defined(__riscv)
#if __riscv_xlen == 64
/* ------------------------------------
RISC-V 64 relocations
------------------------------------ */
   // TODO:
#elif __riscv_xlen == 32
/* ------------------------------------
RISC-V 32 relocations
------------------------------------ */
   // TODO:
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

      for(uint16_t i = UINT16_C(0); i < e_phnum; ++i)
      if(ELF_P_TYPE(phdr[i]) == PT_LOAD){
         size_t p_memsz;
         uint32_t p_flags;

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
