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
#ifndef MAIN_H
#define MAIN_H
/* ------------------------------------
   This file is a private interface, it contains details about the design and
   implementaion of the ELF interpreter, among other things.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

#define DEBUG 1

#if DEBUG
#define DBG(str, ...) printf(str, __VA_ARGS__)
int printf(const char *, ...);
int puts(const char *);
#else
#define DBG(str, ...)
#endif

#include <stddef.h>
#include <stdint.h>
#include "elf.h"

#define string(s) ((char*)(s))
#define ascii(s)  ((uint8_t*)(s))
#define SIZE_C(x) ((size_t)x##U)
#define addressof(ptr) ((uintptr_t)(ptr)) // NOTE: Is this correct?

#define isELF(obj) (\
   ((obj)[EI_MAG0] == ELFMAG0) && \
   ((obj)[EI_MAG1] == ELFMAG1) && \
   ((obj)[EI_MAG2] == ELFMAG2) && \
   ((obj)[EI_MAG3] == ELFMAG3) \
)

#define Shdr_entry(i) ((Elf_Shdr*)(\
   obj + ((size_t)(ehdr->e_shoff) + (size_t)(i) * (size_t)(ehdr->e_shentsize)) \
))
#define Phdr_entry(i) ((Elf_Phdr*)(\
   obj + ((size_t)(ehdr->e_phoff) + (size_t)(i) * (size_t)(ehdr->e_phentsize)) \
))

#if __STDC_VERSION__ < 202311L
#define true 1
#define false 0
typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

extern uint8_t *tuim_env[];
extern size_t tuim_ld_library_path_len;

#if defined(_M_X64)
#define __x86_64__ 1
#elif defined(_M_IX86)
#define __i386__
#endif

#if   (defined(__riscv) && (__riscv_xlen == 64)) \
   || (defined(__ARM_ARCH) && (__ARM_ARCH >= 8)) \
   || defined(__x86_64__)
#define ELF_ST_TYPE(i) ELF64_ST_TYPE(i)

typedef Elf64_Addr Elf_Addr;

typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Shdr Elf_Shdr;
typedef Elf64_Sym  Elf_Sym;
typedef Elf64_Rel  Elf_Rel;
typedef Elf64_Rela Elf_Rela;
typedef Elf64_Dyn  Elf_Dyn;
#elif (defined(__riscv) && (__riscv_xlen == 32)) \
   || (defined(__ARM_ARCH) && (__ARM_ARCH <= 7)) \
   || defined(__i386__)
#define ELF_ST_TYPE(i) ELF32_ST_TYPE(i)

typedef Elf32_Addr Elf_Addr;

typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Phdr Elf_Phdr;
typedef Elf32_Shdr Elf_Shdr;
typedef Elf32_Sym  Elf_Sym;
typedef Elf32_Rel  Elf_Rel;
typedef Elf32_Rela Elf_Rela;
typedef Elf32_Dyn  Elf_Dyn;
#endif

typedef struct elf_s {
   struct elf_s *next;
   struct elf_s *previous; // This is a double linked list

   struct file_s {
      uint8_t *name;
      size_t sz;
      uint8_t *obj;
   } file;

   struct image_s {
      Elf_Addr start_vaddr; /* Lowest virtual address of a PT_LOAD segment. */
      size_t sz, needed_num;
      uint8_t *program; /* Program memory image (value is the load address) */
   } image;

   void (*entry)(void);

   // Usefull sections
   Elf_Dyn *dynamic;
   struct {
      Elf_Sym *sym; // sh_offset
      uint8_t *str; // sh_link
      size_t  first_non_local; // sh_info
      size_t  symnum; // sh_size / sizeof(Elf_Sym)
   } dynsym;

   struct elf_s *dep[];
} elf_s;

int tuim_mmap(const uint8_t *buf, void *structure);
int tuim_load(void *dst, void *src, size_t filesz, size_t memsz, uint32_t flags);
void tuim_munmap(struct file_s *file, struct image_s *image);

int  tuim_interpreter(uint8_t *file_path, elf_s **main_file);
int  tuim_exec(elf_s *executable);
void tuim_free(elf_s *elf);

void *tuim_FindSymbol(const uint8_t *symbol, const struct elf_s *elf);

/* Know external dependencies */
void *malloc(size_t);
void free(void*);


#define ASCII_SLASH UINT8_C(0x2f)
#define ASCII_LOWER_L UINT8_C(0x6c)
#define ASCII_LOWER_I UINT8_C(0x69)
#define ASCII_LOWER_B UINT8_C(0x62)
static bool haveslash(uint8_t *str){
   for(; *str != UINT8_C(0x00); ++str)
      if(*str == ASCII_SLASH) return true;
   return false;
}

static uint8_t *_strcpy(uint8_t * restrict s1, const uint8_t * restrict s2){
   uint8_t *_s1 = s1;
   loop: {
      *_s1 = *s2;
      if(*s2 != UINT8_C(0x00)){
         ++_s1;
         ++s2;
         goto loop;
      }
   }
   return s1;
}

static uint8_t *_strcat(uint8_t * restrict s1, const uint8_t * restrict s2){
   uint8_t *_s1 = s1;
   while(*_s1 != UINT8_C(0x00)){
      ++_s1;
   }
   _strcpy(_s1, s2);
   return s1;
}

static size_t _strlen(const uint8_t *s){
   const uint8_t * const s_cpy = s;
   for(;*s != UINT8_C(0x00); ++s){}
   return (size_t)(s - s_cpy);
}

static int _strcmp(const uint8_t *s1, const uint8_t *s2){
   for(; (*s1 == *s2) && (*s1 != UINT8_C(0x00)); ++s1, ++s2);
   return (int)(*s1 - *s2);
}

static void *_memcpy(void * restrict s1, const void * restrict s2, size_t n){
   uint8_t *_s1 = s1;
   const uint8_t *_s2 = s2;

   if(n == SIZE_C(0)) return s1;

   loop: {
      *_s1 = *_s2;
      --n;
      if(n > SIZE_C(0)){
         ++_s1, ++_s2;
         goto loop;
      }
   }
   return s1;
}

static void *_memset(void *s, int c, size_t n){
   uint8_t *_s = s;

   if(n == SIZE_C(0)) return s;

   loop: {
      *_s = (uint8_t)c;
      --n;
      if(n > SIZE_C(0)){
         ++_s;
         goto loop;
      }
   }
   return s;
}

#endif /* MAIN_H */
