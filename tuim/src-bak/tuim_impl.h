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

   int  (*new_vm)(void**,const char*);
   void (*delete_vm)(void*);

   int(*read) (void*,void *dest, tuim_addr src, tuim_size size);
   int(*write)(void*,tuim_addr dest, const void *src, tuim_size size);

   /**/
   int (*registers)(void*,void*,void*);

   /* allocate 'size' bytes on the target machine */
   uint64_t(*malloc)(uint64_t size);

   /* allocate size bytes on the target machine,
      returned address shall be 'alignment' aligned  */
   uint64_t(*aligned_alloc)(void *, uint64_t alignment, uint64_t size);

   /* protect memory blocks on the target machine */
   void(*mprotect)(void*,tuim_addr,tuim_size,int);

   /* free memory allocated with tuim_malloc or aligned_alloc */
   void(*free)(uint64_t addr);

   /* copy 'size' <unit>s from the object pointed to by 'src' to the object
      pointed to by 'dest', both on target machine memory */
   void(*memcpy)(uint64_t dest, uint64_t src, uint64_t size);

   /* nullptr value on the target machine */
   uint64_t null, sizeof_int, sizeof_ptr;

   int(*check_attributes)(uint8_t *obj);
   int(*load_segments)(const struct tuim_ctx_s *ctx, tuim_elf *elf);
   const uint8_t *(*get_dependency)(tuim_elf *elf);
   const void *(*get_relocation)(tuim_elf *elf);
   const uint8_t *(*relocate)(
      void *ptr, tuim_elf *elf, const void *_rel, const void *_dep);

   /* execute instruction at the address pointed to by 'address' */
   int (*jump)(void *ptr, tuim_addr address);
   int (*exec_init)(void *ptr, char *const argv[], char *const envp[]);
   int (*exec_fini)(void *ptr, int *exit_status);

   const void *(*get_sym)(const tuim_elf *elf, const uint8_t *symbol);
   uint64_t (*get_symbol)(const tuim_elf *elf, const uint8_t *symbol);
};

void tuim_writeerror(struct tuim_ctx_s *ctx, const char * const strings[], int status);

/* -------------------------------------------------------------------------- */

int  tuim_target_new_vm(void**, const char *);
void tuim_target_delete_vm(void*);

tuim_addr tuim_target_aligned_alloc(void *ptr, tuim_size alignment, tuim_size size);

int tuim_target_read(void *ptr, void *dest, tuim_addr src, tuim_size size);
int tuim_target_write(void *ptr, tuim_addr dest, const void *src, tuim_size size);
int tuim_target_memcpy(void *ptr, tuim_addr dest, tuim_addr src, tuim_size size);

int tuim_target_registers(void*,void*,void*);

int tuim_target_jump(void *ptr, tuim_addr address);
int tuim_target_exec_init(void *ptr, char *const argv[], char *const envp[]);
int tuim_target_exec_fini(void *ptr, int *exit_status);

uint64_t tuim_malloc(uint64_t size);
uint64_t tuim_aligned_alloc(uint64_t alignment, uint64_t size);
void tuim_target_mprotect(void *ptr, tuim_addr addr, tuim_size lenght, int prot);
void tuim_free(uint64_t addr);

extern const uint64_t tuim_target_nullptr;

int tuim_check_attributes(uint8_t *obj);
int tuim_load_segments(const tuim_ctx *ctx, tuim_elf *elf);

const uint8_t *tuim_get_dependency(tuim_elf *elf);
const void* tuim_get_relocation(tuim_elf *elf);
const uint8_t *tuim_relocate(
   void *ptr, tuim_elf *elf, const void *_rel, const void *_dep);

const void *tuim_get_sym(const tuim_elf *elf, const uint8_t *symbol);
uint64_t tuim_get_symbol(const tuim_elf *elf, const uint8_t *symbol);

/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


 /** Returns the exit status from a process for which `WIFEXITED` is true. */
#define WEXITSTATUS(__status) (((__status) & 0xff00) >> 8)

/** Returns true if a process dumped core. */
#define WCOREDUMP(__status) ((__status) & 0x80)

/** Returns the terminating signal from a process, or 0 if it exited normally. */
#define WTERMSIG(__status) ((__status) & 0x7f)

/** Returns the signal that stopped the process, if `WIFSTOPPED` is true. */
#define TUIM_STOPSIG(__status)  (((__status) & 0xff00) >> 8)

/** Returns true if the process exited normally. */
#define WIFEXITED(__status) (WTERMSIG(__status) == 0)

/** Returns true if the process was stopped by a signal. */
#define WIFSTOPPED(__status) (((__status) & 0xff) == 0x7f)

/** Returns true if the process was terminated by a signal. */
#define WIFSIGNALED(__status) (WTERMSIG((__status)+1) >= 2)

/** Returns true if the process was resumed . */
#define WIFCONTINUED(__status) ((__status) == 0xffff)

/** Constructs a status value from the given exit code and signal number. */
#define W_EXITCODE(__exit_code, __signal_number) ((__exit_code) << 8 | (__signal_number))

/** Constructs a status value for a process stopped by the given signal. */
#define TUIM_STOPCODE(__signal_number) ((__signal_number) << 8 | 0x7f)

#endif /* TUIM_IMPLEMENTATION_H */
