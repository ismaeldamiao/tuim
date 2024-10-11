/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos

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
#include "machine.h"

#include "tuim_utils.h"
/* ------------------------------------
   Function to load in memory an PIE (shared object, ELF file).
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

#define STATIC static
#include "tuim_mprotect.c"
#include "tuim_pathsearch.c"

static bool haveslash(uint8_t *str){
   /* Value of '/' in the ASCII chart is 0x2f */
   for(; *str; ++str)
      if(*str == UINT8_C(0x2f)) return true;
   return false;
}

#ifdef __ANDROID_API__
#if __ANDROID_API__ < 28
static void* aligned_alloc(size_t alignment, size_t size){
   void *buf;
   posix_memalign(&buf, alignment, size);
   return buf;
};
#endif /* __ANDROID_API__ >= 28 */
#endif /* __ANDROID_API__ */

tuim_elf* tuim_loader(const char *file_path){
   unsigned int status;
   size_t dyn_num;
   FILE *file_ptr;

   Elf_Ehdr *ehdr = NULL;
   Elf_Phdr *phdr = NULL;
   Elf_Shdr *shdr = NULL;
   Elf_Dyn *dyn = NULL;

   tuim_elf *elf = NULL;

   /* --------------------------------------------------------------------------
   In memory loading
   -------------------------------------------------------------------------- */
   //printf("%s: Ateh aqui nos ajudou o Senhor.\n", file_path);
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

   /* Parse the Program Header table and load the segments. */
   {
      uint16_t *e_phnum;
      e_phnum = &ELF_E_PHNUM(*ehdr);

      if(*e_phnum != UINT16_C(0)){
         uint16_t e_phentsize;
         uintptr_t e_phoff;

         e_phentsize = ELF_E_PHENTSIZE(*ehdr);
         e_phoff = ELF_E_PHOFF(*ehdr);

         if((size_t)e_phentsize < sizeof(Elf_Phdr)){
            tuim_error = TUIM_ERROR_INVALIDELF;
            goto error;
         }

         phdr = malloc(sizeof(Elf_Phdr) * (size_t)(*e_phnum));
         if(phdr == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            goto error;
         }
         elf->phdr = phdr;
         elf->segments = malloc(sizeof(void*) * (size_t)(*e_phnum));
         if(elf->segments == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            *e_phnum = UINT16_C(0);
            goto error;
         }

         for(uint16_t i = UINT16_C(0); i < (*e_phnum); ++i){
            uintN_t p_memsz;

            /* Load the program header entry */
            if(fseek(
               file_ptr,
               (long)(e_phoff + (uintptr_t)(e_phentsize * i)),
               SEEK_SET
            ) != 0){
               tuim_error = TUIM_ERROR_READING;
               *e_phnum = i;
               goto error;
            }
            status = fread(
               phdr[i],
               sizeof(uint8_t),
               sizeof(Elf_Phdr),
               file_ptr
            );
            if(status != sizeof(Elf_Phdr)){
               tuim_error = TUIM_ERROR_READING;
               *e_phnum = i;
               goto error;
            }

            p_memsz = ELF_P_MEMSZ(phdr[i]);

            /* Load the segment */
            if(p_memsz != (uintN_t)0){
               uintN_t p_filesz, p_align;

               p_align = ELF_P_ALIGN(phdr[i]);
               p_filesz = ELF_P_FILESZ(phdr[i]);

               elf->segments[i] = aligned_alloc((size_t)p_align, (size_t)p_memsz);
               if(elf->segments[i] == NULL){
                  tuim_error = TUIM_ERROR_MEMORY;
                  *e_phnum = i;
                  goto error;
               }

               if(fseek(
                  file_ptr,
                  (long)ELF_P_OFFSET(phdr[i]),
                  SEEK_SET
               ) != 0){
                  tuim_error = TUIM_ERROR_READING;
                  *e_phnum = i;
                  goto error;
               }
               status = fread(
                  elf->segments[i],
                  sizeof(uint8_t),
                  (size_t)p_filesz,
                  file_ptr
               );
               if(status != p_filesz){
                  tuim_error = TUIM_ERROR_READING;
                  *e_phnum = i;
                  goto error;
               }

               if(p_memsz > p_filesz){
                  memset(
                     (uint8_t*)(elf->segments[i]) + p_filesz,
                     0, (size_t)(p_memsz - p_filesz)
                  );
               }

               if(ELF_P_TYPE(phdr[i]) == PT_DYNAMIC){
                  dyn = elf->segments[i];
                  dyn_num = p_filesz / sizeof(Elf_Dyn);
               }
            }else{
               elf->segments[i] = NULL;
            }
         }
      }
   }

   /* Parse the Section Header table and map sections. */
   {
      uint16_t *e_shnum;
      e_shnum = &ELF_E_SHNUM(*ehdr);

      if(*e_shnum != UINT16_C(0)){
         uint16_t e_shentsize;
         uintptr_t e_shoff;

         e_shentsize = ELF_E_SHENTSIZE(*ehdr);
         e_shoff = ELF_E_SHOFF(*ehdr);

         if(e_shentsize < sizeof(Elf_Shdr)){
            tuim_error = TUIM_ERROR_INVALIDELF;
            goto error;
         }

         shdr = malloc(sizeof(Elf_Shdr) * (size_t)(*e_shnum));
         if(shdr == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            goto error;
         }
         elf->shdr = shdr;
         elf->sections = malloc(sizeof(void*) * (size_t)(*e_shnum));
         if(elf->sections == NULL){
            tuim_error = TUIM_ERROR_MEMORY;
            *e_shnum = UINT16_C(0);
            goto error;
         }
         elf->sections[0] = NULL;

         for(uint16_t i = UINT16_C(1); i < (*e_shnum); ++i){
            uintN_t sh_size;

            /* Load the section header entry */
            if(fseek(
               file_ptr,
               (long)(e_shoff + (uintptr_t)(e_shentsize * i)),
               SEEK_SET
            ) != 0){
               tuim_error = TUIM_ERROR_READING;
               *e_shnum = i;
               goto error;
            }
            status = fread(
               shdr[i],
               sizeof(uint8_t),
               sizeof(Elf_Shdr),
               file_ptr
            );
            if(status != sizeof(Elf_Shdr)){
               tuim_error = TUIM_ERROR_READING;
               *e_shnum = i;
               goto error;
            }

            sh_size = ELF_SH_SIZE(shdr[i]);
            elf->sections[i] = NULL;
            if(sh_size != (uintN_t)0){
               uint16_t e_phnum;
               uintN_t sh_offset, p_offset, p_filesz, p_memsz;

               e_phnum = ELF_E_PHNUM(*ehdr);
               sh_offset = ELF_SH_OFFSET(shdr[i]);

               /* Try to map the section data from segments */
               for(uint16_t j = UINT16_C(0); j < e_phnum; ++j){
                  if(ELF_SH_TYPE(shdr[i]) != SHT_NOBITS){
                     p_offset = ELF_P_OFFSET(phdr[j]);
                     p_filesz = ELF_P_FILESZ(phdr[j]);

                     if(
                        (sh_offset >= p_offset) &&
                        (sh_offset < (p_offset + p_filesz))
                     ){
                        elf->sections[i] = (uint8_t*)(elf->segments[j]) +
                           (sh_offset - p_offset);
                        break;
                     }
                  }else{
                     p_offset = ELF_P_OFFSET(phdr[j]);
                     p_memsz = ELF_P_MEMSZ(phdr[j]);

                     if(
                        (sh_offset >= p_offset) &&
                        ((sh_offset + sh_size) < (p_offset + p_memsz))
                     ){
                        elf->sections[i] = (uint8_t*)(elf->segments[j]) +
                           (sh_offset - p_offset);
                        break;
                     }
                  }
               }
               /* Otherwise copy it from the file. */
               if(elf->sections[i] == NULL){

                  elf->sections[i] = malloc((size_t)sh_size);
                  if(elf->sections[i] == NULL){
                     tuim_error = TUIM_ERROR_MEMORY;
                     *e_shnum = i;
                     goto error;
                  }

                  if(ELF_SH_TYPE(shdr[i]) != SHT_NOBITS){
                     if(fseek(
                        file_ptr,
                        (long)sh_offset,
                        SEEK_SET
                     ) != 0){
                        tuim_error = TUIM_ERROR_READING;
                        *e_shnum = i;
                        goto error;
                     }
                     status = fread(
                        elf->sections[i],
                        sizeof(uint8_t),
                        (size_t)sh_size,
                        file_ptr
                     );
                     if(status != sh_size){
                        tuim_error = TUIM_ERROR_READING;
                        *e_shnum = i;
                        goto error;
                     }
                  }
               }
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
   {
      uint16_t e_phnum;

      e_phnum = ELF_E_PHNUM(*ehdr);

      for(uint16_t i = UINT16_C(0); i < e_phnum; ++i){
         size_t p_memsz;
         uint32_t p_flags;

         p_flags = ELF_P_FLAGS(phdr[i]);
         p_memsz = ELF_P_MEMSZ(phdr[i]);

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
