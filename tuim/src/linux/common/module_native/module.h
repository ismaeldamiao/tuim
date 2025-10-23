/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 13, 2025.
 ******************************** MIT licensed ********************************/
#ifndef TUIM_MODULE_H
#define TUIM_MODULE_H
/*******************************************************************************
 * The reference implementation for the native backend for Linux.
 ******************************************************************************/

/**
 * For simplicity, all dependency header files are here
 */

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>

#include <tuim.h>

#include "elf.h"
#include "list.h"

/**
 * Architecture dependent macros and types to work with registers,
 * do system calls, etc, are defined in the file "asm.h". Also the image
 * used to create a new process is defined there.
 */
#include "asm.h"

#ifndef BUFFER_SIZE
   #define BUFFER_SIZE 1024
#endif

#define swap16(x) (x)
#define swap32(x) (x)
#define swap64(x) (x)

#define SIZE_C(x) ((size_t)x##U)

int tuim_va(int routine, va_list args);

#define TUIM_CTX_MAX_LEN 512

struct tuim_ctx {
   int (*tuim_va)(int routine, va_list args);

   char *tuim_home, *ld_library_path;
   char *machine; /* machine */

   struct list *elf_list; /* list of ELFs */
   /* error handling */
   int status;
   const char *strerror;
   bool not_expecting_exec;

   pid_t pid; uintptr_t entry; uintptr_t syscall;
   uintptr_t null;
};

typedef struct tuim_elf {
   /* information needed for file management */
   char *path;
   int fd;
   size_t sz;
   void *map;
   /**/
   union {
      const void *needed;
      const void *rel;
   } last;
   void *stack[BUFFER_SIZE / sizeof(void*)], *stack_ptr;
   /* things to remember */

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
static int  tuim_fopen(tuim_elf *file, const char *file_name);
static void tuim_fclose(tuim_elf *file);

/**
 * Here is the module representation of routines as C functions.
 */

static int tuim_newcontext(const char *tuim_home, const char *ld_library_path, tuim_ctx **ctx);

static int tuim_load00(tuim_ctx *ctx, const char *elf_path);
static int tuim_load01(tuim_ctx *ctx, tuim_elf *file);
static int tuim_load02(tuim_ctx *ctx, tuim_elf *elf);
static int tuim_load03(tuim_ctx *ctx, tuim_elf *elf);

static int tuim_symbol(tuim_ctx *ctx, const char *elf_path, const char *symbol, uintptr_t *address);
static int tuim_read(tuim_ctx *ctx, uintptr_t address, size_t size, void *buf);
static int tuim_write(tuim_ctx *ctx, uintptr_t address, size_t size, const void *buf);
static int tuim_alloc(tuim_ctx *ctx, size_t alignment, size_t size, uintptr_t *address);
static int tuim_mprotect(tuim_ctx *ctx, uintptr_t address, size_t size, int prot);
static int tuim_getregisters(tuim_ctx *ctx, tuim_registers *regs);
static int tuim_setregisters(tuim_ctx *ctx, tuim_registers *regs);

static int tuim_jump(tuim_ctx *ctx, uintptr_t address);
static int tuim_exec(tuim_ctx *ctx, const char *exec_path, char *const argv[], char *const envp[]);

static thread_local const char *is_that_tmp;
static bool is_that(void *elf){
   const char *s1 = ((tuim_elf*)(elf))->path;
   return (strcmp(s1, is_that_tmp) == 0);
}

#endif /* TUIM_MODULE_H */
