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
#ifndef TUIM_IMPLEMENTATION_H
#define TUIM_IMPLEMENTATION_H

#include <stddef.h>
#include <stdint.h>
#include "tuim.h"

/* ------------------------------------
   This file is a private interface, it contains details about the design and
   implementaion of the ELF interpreter, among other things.
   * Part of Tuim Project.
   * Last modified: June 20, 2025.
------------------------------------ */

struct string {
   uint8_t *character_array;
   size_t len;
};

/* Structure to store a object file, their name and others informations
   the system may need to allocate/deallocate memory for the file. */
struct file_s {
   uint8_t *obj; /* file content */
   size_t sz; /* file size */
};

/* Structure for executables and shared objects */
typedef struct tuim_elf_s {
   struct string identifier;
   struct file_s file;
   void *backend_ptr;
   uint64_t entry, start_vaddr;
   void *phdr, *shdr, *dyn, *dynstr, *sym;
} tuim_elf;

//int tuim_mmap(const uint8_t *buf, void *structure);
//void tuim_munmap(struct file_s *file, struct image_s *image);

void tuim_writeerror(tuim_ctx *ctx, const char * const strings[], int status);

#if 0

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
#include "elf/elf.h"

#define LIST_TYPE struct tuim_elf_s *
#include "list.h"

#if __STDC_VERSION__ < 202311L
#define true 1
#define false 0
typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

#define SIZE_C(x) ((size_t)x##U)
#define addressof(ptr) ((uintptr_t)(void*)(ptr)) // NOTE: Is this correct?

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

extern uint8_t *tuim_env[];
extern size_t tuim_ld_library_path_len;

typedef struct tuim_elf_s tuim_elf;

static bool tuim_HaveCorrectAttributes(void *_ehdr, bool is_executable);
static void tuim_FillElfStructure(struct tuim_elf_s *elf);

void *malloc(size_t);

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

typedef struct tuim_elf_s {
   struct tuim_elf_s *next;
   struct tuim_elf_s *previous; // This is a double linked list

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

   struct tuim_elf_s *dep[];
} tuim_elf_s;

int tuim_mmap(const uint8_t *buf, void *structure);
int tuim_load(void *dst, void *src, size_t filesz, size_t memsz, uint32_t flags);
void tuim_munmap(struct file_s *file, struct image_s *image);

int  tuim_interpreter(uint8_t *file_path, tuim_elf_s **main_file);
//int  tuim_exec(tuim_elf_s *executable);
void tuim_free(tuim_elf_s *elf);

void *tuim_FindSymbol(const uint8_t *symbol, const struct tuim_elf_s *elf);

/* Know external dependencies */
void *malloc(size_t);
void free(void*);

static void tuim_FillElfStructure(struct tuim_elf_s *elf){
   uint8_t
      *obj = elf->file.obj,
      *program = elf->image.program;
   Elf_Addr start_vaddr = elf->image.start_vaddr;
   Elf_Ehdr *ehdr = (Elf_Ehdr*)obj;
   Elf_Dyn *dynamic = NULL;
   Elf_Shdr *dynsym;

   /* Find the entry point */
   if(ehdr->e_type == ET_EXEC)
      elf->entry = (void(*)(void))(program + (ehdr->e_entry - start_vaddr));

   /* Find for the Dynamic Section */
   {
      for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_phnum); ++i){
         Elf_Phdr *phdr;

         phdr = Phdr_entry(i);
         if(phdr->p_type != PT_DYNAMIC) continue;

         dynamic = (Elf_Dyn*)(obj + phdr->p_offset);
         break;
      }

      if(dynamic == NULL) return;
      elf->dynamic = dynamic;
   }

   // Find for the dynamic symbol table
   {
      for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_shnum); ++i){
         Elf_Shdr *shdr;

         shdr = Shdr_entry(i);
         if(shdr->sh_type != SHT_DYNSYM) continue;

         elf->dynsym.sym = (Elf_Sym*)(obj + shdr->sh_offset);
         elf->dynsym.str = obj + Shdr_entry(shdr->sh_link)->sh_offset;
         elf->dynsym.first_non_local = (size_t)(shdr->sh_info);
         elf->dynsym.symnum =
            (size_t)(shdr->sh_size) / sizeof(Elf_Sym);
         break;
      }
   }
}

#endif

#endif /* TUIM_IMPLEMENTATION_H */
