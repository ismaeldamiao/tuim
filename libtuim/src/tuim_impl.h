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
#include <stdio.h>
#include <tuim.h>

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#if defined(tuim_environment)
   #include <tuim/arch.h>
   #include <tuim/kernel.h>
#else
   #include "arch.h"
   #include "kernel.h"
#endif

/* ------------------------------------
   This file is a private interface, it contains details about the design and
   implementaion of the ELF interpreter, among other things.
   * Part of Tuim Project.
   * Last modified: June 20, 2025.
------------------------------------ */

/* -----------------------------------------------------------------------------
   BUILD SETUP
----------------------------------------------------------------------------- */

/* language extensions */

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
#endif
#if __STDC_VERSION__ < 199901L
   #define bool int
#elif (__STDC_VERSION__ >= 199901L) && (__STDC_VERSION__ < 202311L)
   typedef _Bool bool;
#endif

#if __STDC_VERSION__ < 201112L
   #define thread_local
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define thread_local _Thread_local
#endif

#if __STDC_VERSION__ < 201112L
   #define alignof(x)
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define alignas(x) _Alignas(x)
#endif

#if __STDC_VERSION__ < 199901L
   #define restrict
#endif

/* Swap macros */

#define swap16(x) (x)
#define swap32(x) (x)
#define swap64(x) (x)

/* TUIM_BUILD_FLAGS */

#define TUIM_BF_ELF32 (1 << 0)
#define TUIM_BF_ELF64 (1 << 1)

/* Instruction Set Architecture support */

#if   tuim_arch == tuim_riscv32
   #define TUIM_BUILD_RISCV32
   #define TUIM_BUILD_FLAGS TUIM_BF_ELF32
   #error Support to RISC-V architecture is not implemented yet
#elif tuim_arch == tuim_riscv64
   #define TUIM_BUILD_RISCV64
   #define TUIM_BUILD_FLAGS (TUIM_BF_ELF32 | TUIM_BF_ELF64)
   #error Support to RISC-V architecture is not implemented yet
#elif tuim_arch == tuim_arm
   #define TUIM_BUILD_AARCH32
   #define TUIM_BUILD_FLAGS TUIM_BF_ELF32
#elif tuim_arch == tuim_aarch64
   #define TUIM_BUILD_AARCH64
   #define TUIM_BUILD_FLAGS TUIM_BF_ELF64
#elif tuim_arch == tuim_i386
   #define TUIM_BUILD_I386
   #define TUIM_BUILD_FLAGS TUIM_BF_ELF32
   #error Support to Intel 386 architecture is not implemented yet
#elif tuim_arch == tuim_amd64
   #define TUIM_BUILD_AMD64
   #define TUIM_BUILD_FLAGS (TUIM_BF_ELF32 | TUIM_BF_ELF64)
#else
   #error Target Instruction Set Architecture is not suported
#endif

/* -----------------------------------------------------------------------------
   API UTILS
----------------------------------------------------------------------------- */

struct string {
   char *character_array;
   size_t len;
};

/* Structure to store a object file, their name and others informations
   the system may need to allocate/deallocate memory for the file. */
struct file_s {
   const char *path;
   #if   tuim_kernel == tuim_linux ||\
         tuim_kernel == tuim_xnu
      int fd;
   #elif tuim_kerel == tuim_windows_nt
   #endif
   FILE *stream;
   uint8_t *obj; /* file content */
   size_t sz; /* file size */ // TODO: delete this
};

/* Structure for executables and shared objects */
typedef struct tuim_elf_s {
   struct file_s file;

   /* dynamic section */
   const void *dyn;
   const uint8_t *dynstr;


  /* dynamic symbol table */
   const void *dynsym;
   size_t sh_info, sh_entsize, sh_size;

   /* dynamic rellocation tables */
   const void *jmprel, *rel, *rela;
   uint64_t pltrel;
   size_t relent, relaent;
   size_t pltrelsz, relsz, relasz;
   size_t relnum, relanum;
   bool is_rel, is_rela;

   const void *auxiliary;

   /* others informations */
   uint64_t program_image, start_vaddr;
   size_t program_size;
} tuim_elf;

#define TUIM_CTX_MAX_LEN 512

struct tuim_ctx_s {
   /* environment variables */
   char tuim_home[TUIM_CTX_MAX_LEN], ld_library_path[TUIM_CTX_MAX_LEN];
   char *machine; /* machine */

   struct list *elf_list; /* list of ELFs */
   /* error handling */
   int status;
   char *strerror;
   bool can_load_exec;

   /* ------ machine abstraction ------ */
   void *backend_pointer;

   void * (*new_vm)(void);
   void (*delete_vm)(void*);

   /* allocate 'size' bytes on the target machine */
   uint64_t(*malloc)(uint64_t size);

   /* allocate size bytes on the target machine,
      returned address shall be 'alignment' aligned  */
   uint64_t(*aligned_alloc)(uint64_t alignment, uint64_t size);

   /* protect memory blocks on the target machine */
   void(*mprotect)(uint64_t addr, uint64_t size, int prot);

   /* free memory allocated with tuim_malloc or aligned_alloc */
   void(*free)(uint64_t addr);

   /* copy 'size' <unit>s from the object pointed to by 'src' to the object
      pointed to by 'dest', both on target machine memory */
   void(*memcpy)(uint64_t dest, uint64_t src, uint64_t size);
   void(*memcpy2)(uint64_t dest, uint64_t src, uint64_t size);
   void(*memcpy4)(uint64_t dest, uint64_t src, uint64_t size);
   void(*memcpy8)(uint64_t dest, uint64_t src, uint64_t size);

   /* copy 'size' <unit>s from the object pointed to by 'src' on target machine
      to the object pointed to by 'dest' on host machine */
   void(*load)(void *dest, uint64_t src, uint64_t size);
   void(*load2)(void *dest, uint64_t src, uint64_t size);
   void(*load4)(void *dest, uint64_t src, uint64_t size);
   void(*load8)(void *dest, uint64_t src, uint64_t size);

   /* copy 'size' <unit>s from the object pointed to by 'src' on host machine
      to the object pointed to by 'dest' on target machine */
   void(*store)(uint64_t dest, const void *src, uint64_t n);
   void(*store2)(uint64_t dest, const void *src, uint64_t n);
   void(*store4)(uint64_t dest, const void *src, uint64_t n);
   void(*store8)(uint64_t dest, const void *src, uint64_t n);

   /* set 'size' <unit>s to the value of 'c' casted to fill on the
      object pointed to by 'dest' */
   void(*memset)(uint64_t dest, int c, uint64_t size);
   void(*memset2)(uint64_t dest, int c, uint64_t size);
   void(*memset4)(uint64_t dest, int c, uint64_t size);
   void(*memset8)(uint64_t dest, int c, uint64_t size);

   /* nullptr value on the target machine */
   uint64_t null, sizeof_int, sizeof_ptr;

   int(*check_attributes)(uint8_t *obj);
   int(*load_segments)(const struct tuim_ctx_s *ctx, tuim_elf *elf);
   const uint8_t *(*get_dependency)(tuim_elf *elf);
   const void *(*get_relocation)(tuim_elf *elf);
   const uint8_t *(*relocate)(tuim_elf *elf, const void *_rel, const void *_dep);

   /* execute instruction at the address pointed to by 'address' */
   void(*jump)(uint64_t address);
   const void *(*get_sym)(const tuim_elf *elf, const uint8_t *symbol);
   uint64_t (*get_symbol)(const tuim_elf *elf, const uint8_t *symbol);
};

void tuim_writeerror(struct tuim_ctx_s *ctx, const char * const strings[], int status);

/* -------------------------------------------------------------------------- */

void *tuim_host_new_vm(void);
void  tuim_host_delete_vm(void*);

uint64_t tuim_malloc(uint64_t size);
uint64_t tuim_aligned_alloc(uint64_t alignment, uint64_t size);
void tuim_mprotect(uint64_t addr, uint64_t size, int prot);
void tuim_free(uint64_t addr);

void tuim_memcpy (uint64_t dest, uint64_t src, uint64_t n); // unit: bytes
void tuim_memcpy2(uint64_t dest, uint64_t src, uint64_t n); // unit: half words
void tuim_memcpy4(uint64_t dest, uint64_t src, uint64_t n); // unit: word
void tuim_memcpy8(uint64_t dest, uint64_t src, uint64_t n); // unit: xwords

void tuim_memset (uint64_t dest, int c, uint64_t n); // unit: bytes
void tuim_memset2(uint64_t dest, int c, uint64_t n); // unit: half words
void tuim_memset4(uint64_t dest, int c, uint64_t n); // unit: word
void tuim_memset8(uint64_t dest, int c, uint64_t n); // unit: xwords

void tuim_load (void *dest, uint64_t src, uint64_t n); // unit: bytes
void tuim_load2(void *dest, uint64_t src, uint64_t n); // unit: half words
void tuim_load4(void *dest, uint64_t src, uint64_t n); // unit: word
void tuim_load8(void *dest, uint64_t src, uint64_t n); // unit: xwords

void tuim_store (uint64_t dest, const void *src, uint64_t n); // unit: bytes
void tuim_store2(uint64_t dest, const void *src, uint64_t n); // unit: half words
void tuim_store4(uint64_t dest, const void *src, uint64_t n); // unit: word
void tuim_store8(uint64_t dest, const void *src, uint64_t n); // unit: xwords

extern const uint64_t tuim_nullptr;

int tuim_check_attributes(uint8_t *obj);
int tuim_load_segments(const tuim_ctx *ctx, tuim_elf *elf);

const uint8_t *tuim_get_dependency(tuim_elf *elf);
const void* tuim_get_relocation(tuim_elf *elf);
const uint8_t *tuim_relocate(tuim_elf *elf, const void *_rel, const void *_dep);

void tuim_jump(uint64_t address);
const void *tuim_get_sym(const tuim_elf *elf, const uint8_t *symbol);
uint64_t tuim_get_symbol(const tuim_elf *elf, const uint8_t *symbol);

#endif /* TUIM_IMPLEMENTATION_H */
