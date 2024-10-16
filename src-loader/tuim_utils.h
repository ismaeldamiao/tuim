#ifndef TUIM_UTILS_H
#define TUIM_UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/tuim.h"
#include "elf.h"

/* ------------------------------------
   Determine processor
------------------------------------ */

/* To support MSVC */
#if _MSC_VER
   #if defined(_M_IX86)
      #define __i386__
   #elif defined(_M_AMD64) && !defined(__ARM_ARCH)
      #define __x86_64__
   #endif
#endif

#if defined(__ARM_ARCH)
   /* Macros defined by ARM Compiler are considered here
      the standard if compiling for ARM target. */
   #if __ARM_ARCH >= 8
      #define ELFCLASS      ELFCLASS64
      #if defined(__ARM_BIG_ENDIAN)
         #define ELFDATA    ELFDATA2MSB
      #else
         #define ELFDATA    ELFDATA2LSB
      #endif
      #define EM_           EM_AARCH64
   #elif __ARM_ARCH <= 7
      #define ELFCLASS      ELFCLASS32
      #if defined(__ARM_BIG_ENDIAN)
         #define ELFDATA    ELFDATA2MSB
      #else
         #define ELFDATA    ELFDATA2LSB
      #endif
      #define EM_           EM_ARM
   #endif
#elif defined(__x86_64__)
   /* FIXME: Macros defined by LLVM CLang are considered here
      the standard when compiling for AMD64 target. */
   #if defined(__LP64__)
      #define ELFCLASS      ELFCLASS64
      typedef uint64_t wordclass;
   #elif defined(__ILP32__)
      #define ELFCLASS      ELFCLASS32
      typedef uint32_t wordclass;
   #endif
   #define ELFDATA          ELFDATA2LSB
   #define EM_              EM_X86_64
#elif defined(__i386__)
   /* FIXME: Macros defined by LLVM CLang are considered here
      the standard when compiling for x86 target. */
   #define ELFCLASS      ELFCLASS32
   #define ELFDATA       ELFDATA2LSB
   #define EM_           EM_386
#elif defined(__riscv)
   /* Macros defined by RISC-V C API are considered
      here the standard when compiling for RISC-V target. */
   #if __riscv_xlen == 64
      #define ELFCLASS      ELFCLASS64
      #define EM_           EM_RISCV
   #elif __riscv_xlen == 32
      #define ELFCLASS      ELFCLASS32
      #define EM_           EM_RISCV
   #endif
#else
   #error "Unsupported processor."
#endif

#if ELFCLASS == ELFCLASS64
   typedef uint64_t uintN_t;
   typedef int64_t intN_t;
#elif ELFCLASS == ELFCLASS32
   typedef uint32_t uintN_t;
   typedef int32_t intN_t;
#endif // USE_ELF64

/* ------------------------------------
   Determine endianness
------------------------------------ */
#if !defined(ELFDATA)
   #if defined(__GNUC__)
      #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
         #define ELFDATA    ELFDATA2MSB
      #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
         #define ELFDATA    ELFDATA2LSB
      #endif // __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   #elif __STDC_VERSION__ >= 202311L
      #include <stdbit.h>
      #if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__
         #define ELFDATA    ELFDATA2MSB
      #elif __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
         #define ELFDATA    ELFDATA2LSB
      #else
         #error "Unsupported endianness."
      #endif // __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__
   #else
      #error "Can't determite endianness."
   #endif // defined(__GNUC__)
#endif // !defined(ELFDATA)

#define segment_base_addr(elf, phndx) (\
   (uint8_t*)((elf)->segments[phndx]) - \
   ELF_P_VADDR(((Elf_Phdr*)((elf)->phdr))[phndx])\
)
#define section_base_addr(elf, shndx) (\
   (uint8_t*)((elf)->sections[shndx]) - \
   ELF_SH_ADDR(((Elf_Shdr*)((elf)->shdr))[shndx])\
)

#define XOR(a, b) ((void*)((uintptr_t)(a) ^ (uintptr_t)(b)))

/* In system that do not use char as ASCII these macro need
   to be changed to a function. */
#define ascii(c) ((uint8_t*)(c))
#define str(i) ((char*)(i))

typedef struct elf_list_s {
   const char *file_path;
   tuim_elf *elf;
   void *link;
} elf_list;
extern elf_list *tuim_loaded;

static int strcmp_(uint8_t *l, uint8_t *r){
   while((*l == *r) && *l){
      ++l, ++r;
   }
	return (int)(*l - *r);
}

/* Get the index of the segment where the address vaddr is in. */
static uint16_t phndx(const tuim_elf *elf, uintptr_t vaddr){
   uint16_t e_phnum;

   Elf_Ehdr *ehdr = NULL;
   Elf_Phdr *phdr = NULL;

   ehdr = elf->ehdr;
   phdr = elf->phdr;
   e_phnum = ELF_E_PHNUM(*ehdr);

   for(uint16_t i = UINT16_C(0); i < e_phnum; ++i){
      size_t p_memsz;
      uintptr_t p_vaddr;

      p_vaddr = ELF_P_VADDR(phdr[i]);
      p_memsz = ELF_P_MEMSZ(phdr[i]);

      if((vaddr >= p_vaddr) && (vaddr < (p_vaddr + p_memsz)))
         return i;
   }

   return UINT16_C(0);
}

/* Get the index of the section where the address vaddr is in. */
static uint16_t shndx(const tuim_elf *elf, uintptr_t vaddr){
   uint16_t e_shnum;

   Elf_Ehdr *ehdr = NULL;
   Elf_Shdr *shdr = NULL;

   ehdr = elf->ehdr;
   shdr = elf->shdr;
   e_shnum = ELF_E_SHNUM(*ehdr);

   for(uint16_t i = UINT16_C(0); i < e_shnum; ++i){
      size_t sh_size;
      uintptr_t sh_addr;

      sh_addr = ELF_SH_ADDR(shdr[i]);
      sh_size = ELF_SH_SIZE(shdr[i]);

      if((vaddr >= sh_addr) && (vaddr < (sh_addr + sh_size)))
         return i;
   }

   return UINT16_C(0);
}

static
elf_list* list_add(elf_list **list, const char *file_path, tuim_elf *elf){
   elf_list *prev, *curr, *next, *new;

   new = malloc(sizeof(elf_list));

   if(new){

      curr = *list;
      prev = NULL;

      while(curr){ // Go to the end
         next = XOR(prev, curr->link);
         prev = curr;
         curr = next;
      }

      if(prev)
         prev->link = XOR(XOR(curr, prev->link), new);

      new->file_path = file_path;
      new->elf = elf;
      new->link = XOR(prev, NULL);

      if(*list == NULL) *list = new;
   }

   return new;
}

static tuim_elf* list_find(const elf_list *list, const char* file_path){
   elf_list *prev, *curr, *next;

   curr = (elf_list*)list;
   prev = NULL;

   while(curr){
      if(strcmp(curr->file_path, file_path) == 0)
         return (tuim_elf*)(curr->elf);

      next = XOR(prev, curr->link);
      prev = curr;
      curr = next;
   }

   return NULL;
}

static void list_remove(elf_list **list, const tuim_elf *elf){
   elf_list *prev, *curr, *next;

   curr = *list;
   prev = NULL;

   while(curr){
      next = XOR(prev, curr->link);

      if(curr->elf == elf){
         if(next)
            next->link = XOR(prev, XOR(curr, next->link));
         if(prev)
            prev->link = XOR(XOR(curr, prev->link), next);
         if(*list == curr) *list = next;
         free(curr);
         break;
      }

      prev = curr;
      curr = next;
   }
   return;
}

static void findsymbol(
   tuim_elf **elf, Elf_Sym **sym,
   const uint8_t *symbol, elf_list *list
){
   elf_list *prev, *curr, *next;

   curr = list;
   prev = NULL;

   while(curr){
      {
         Elf_Ehdr *ehdr;
         Elf_Shdr *shdr;
         uint16_t e_shnum;

         ehdr = curr->elf->ehdr;
         shdr = curr->elf->shdr;

         e_shnum = ELF_E_SHNUM(*ehdr);

         for(uint16_t i = UINT16_C(0); i < e_shnum; ++i){
            uint32_t sh_type;

            sh_type = ELF_SH_TYPE(shdr[i]);

            if(sh_type == SHT_SYMTAB){
               Elf_Sym *syms;
               uint8_t *str;
               uint32_t sym_num;

               syms = curr->elf->sections[i];
               str = curr->elf->sections[ELF_SH_LINK(shdr[i])];
               sym_num = ELF_SH_SIZE(shdr[i]) / sizeof(Elf_Sym);

               for(uint16_t j = UINT16_C(0); j < sym_num; ++j){

                  if(strcmp_(
                     (uint8_t*)symbol,
                     str + ELF_ST_NAME(syms[j])
                  ) == 0){
                     uint16_t st_shndx;
                     uint8_t st_bind;

                     st_shndx = ELF_ST_SHNDX(syms[j]);
                     st_bind = ELF_ST_BIND(syms[j]);

                     if(st_shndx == STN_UNDEF){
                        break;
                     }else if(st_bind != STB_GLOBAL){
                        break;
                     }
                     *elf = curr->elf;
                     *sym = syms + j;
                     break;
                  }
               }
            }
         }
      }
      if(*elf != NULL)
         break;

      next = XOR(prev, curr->link);
      prev = curr;
      curr = next;
   }

   return;
}


#endif // TUIM_UTILS_H
