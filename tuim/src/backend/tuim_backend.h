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
#ifndef TUIM_BACKEND_HOST_H
#define TUIM_BACKEND_HOST_H

#include <stddef.h>
#include <stdint.h>
#include "tuim.h"
/* ------------------------------------
   Header file for the backend of Tuim's interpreter.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */

#define SIZE_C(x) ((size_t)x##U)

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF64
   typedef uint64_t Elf_Addr;
#else
   typedef uint32_t Elf_Addr;
#endif

/* note that all pointer on the following structure shall point to objects
   on the host machine's memory */
struct tuim_backend {
   /* string holding the content of a ELF file */
   const uint8_t *obj;

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
   Elf_Addr program_image, start_vaddr;
   size_t program_size;
};

/* ---
   called only by backend
--- */
extern Elf_Addr tuim_nullptr;
Elf_Addr tuim_malloc(const tuim_ctx *ctx, size_t size);
Elf_Addr tuim_aligned_alloc(const tuim_ctx *ctx, size_t alignment, size_t size);
void tuim_mprotect(const tuim_ctx *ctx, Elf_Addr addr, size_t size, int prot);
void tuim_free(const tuim_ctx *ctx, Elf_Addr addr);

void tuim_memcpy (const tuim_ctx *ctx, Elf_Addr dest, Elf_Addr src, size_t n);
void tuim_memcpy2(const tuim_ctx *ctx, Elf_Addr dest, Elf_Addr src, size_t n);
void tuim_memcpy4(const tuim_ctx *ctx, Elf_Addr dest, Elf_Addr src, size_t n);
void tuim_memcpy8(const tuim_ctx *ctx, Elf_Addr dest, Elf_Addr src, size_t n);

void tuim_memset (const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n);
void tuim_memset2(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n);
void tuim_memset4(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n);
void tuim_memset8(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n);

void tuim_load (const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n);
void tuim_load2(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n);
void tuim_load4(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n);
void tuim_load8(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n);

void tuim_store (const tuim_ctx *ctx, Elf_Addr dest, const void *src, size_t n);
void tuim_store2(const tuim_ctx *ctx, Elf_Addr dest, const void *src, size_t n);
void tuim_store4(const tuim_ctx *ctx, Elf_Addr dest, const void *src, size_t n);
void tuim_store8(const tuim_ctx *ctx, Elf_Addr dest, const void *src, size_t n);

const void *tuim_get_sym(void *ptr, const uint8_t *symbol);

/* ---
   called by tuim_loader
--- */
bool tuim_attributes(uint8_t *obj);
int  tuim_load_segments(const tuim_ctx *ctx, const uint8_t *obj, void **ptr);

/* ---
   called by tuim_linker
--- */
const uint8_t *
   tuim_get_dependency(const tuim_ctx *ctx, void *ptr);
const void *
   tuim_get_relocation(const tuim_ctx *ctx, void *ptr);
const uint8_t *
   tuim_relocate(const tuim_ctx *ctx, void *ptr, const void *rel, const void *dep);

/* ---
   called by tuim_exec
--- */
uint64_t tuim_get_symbol(const tuim_ctx *ctx, void *ptr, const uint8_t *symbol);
void     tuim_jump(const tuim_ctx *ctx, uint64_t address);

#endif /* TUIM_BACKEND_H */
