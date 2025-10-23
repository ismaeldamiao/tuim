/** ----------------------------------------------------------------------------
   @file tuim.h
   @brief
      The C API for Tuim.
   @author
      I.F.F. dos Santos <ismaellxd@gmail.com>
   @copyright

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

----------------------------------------------------------------------------- */
#ifndef TUIM_H
#define TUIM_H

#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------------
   Public header file for the Tuim project.
   * Last modified: July 11, 2025.
------------------------------------ */

/* Version */
#define TUIM_VERSION_MAJOR 0
#define TUIM_VERSION_MINOR 3

/* Context */
typedef struct tuim_ctx_s tuim_ctx;

/**
   @brief
   @param
   @return */
tuim_ctx *tuim_newcontext(
   const char *tuim_home, const char *ld_library_path, const char *machine);

/**
   @brief
   @param
   @return */
void tuim_deletecontext(tuim_ctx *ctx);

/* Error handing */
#define TUIM_SUCCESS         0x0000

#define TUIM_EINVAL          0x0001 /* Invalid argument */
#define TUIM_ENOMEM          0x0002 /* Cannot allocate memory */
#define TUIM_ENOENT          0x0003 /* No such file or directory */
#define TUIM_ENOELF          0x0004 /* Not a ELF file */
#define TUIM_ENODYN          0x0005 /* Not a shared object */
#define TUIM_EELFBAD         0x0006 /* Bad attributes */

#define TUIM_ENOSYM          0x0006 /* Library format error */

#define TUIM_ENOEXEC         0x0007 /* Not a executable */
#define TUIM_EFAULT          0x0008 /* Segmentation fault */

/* by convection, values from:
   https://www.man7.org/linux/man-pages/man7/signal.7.html */
#define SIGHUP          1
#define TUIM_SIGINT     2
#define TUIM_SIGQUIT    3
#define TUIM_SIGILL     4
#define TUIM_SIGTRAP    5
#define TUIM_SIGABRT    6
#define TUIM_SIGIOT     6
#define TUIM_SIGBUS     7
#define TUIM_SIGEMT     -
#define TUIM_SIGFPE     8
#define TUIM_SIGKILL    9
#define TUIM_SIGUSR1    10
#define TUIM_SIGSEGV    11
#define TUIM_SIGUSR2    12
#define TUIM_SIGPIPE    13
#define TUIM_SIGALRM    14
#define TUIM_SIGTERM    15
#define TUIM_SIGSTKFLT     16
#define TUIM_SIGCHLD    17
#define TUIM_SIGCLD     -
#define TUIM_SIGCONT    18
#define TUIM_SIGSTOP    19
#define TUIM_SIGTSTP    20
#define TUIM_SIGTTIN    21
#define TUIM_SIGTTOU    22
#define TUIM_SIGURG     23
#define TUIM_SIGXCPU    24
#define TUIM_SIGXFSZ    25
#define TUIM_SIGVTALRM     26
#define TUIM_SIGPROF    27
#define TUIM_SIGWINCH      28
#define TUIM_SIGIO      29
#define TUIM_SIGPWR     30
#define TUIM_SIGINFO    -
#define TUIM_SIGLOST    -
#define TUIM_SIGSYS     31
#define TUIM_SIGUNUSED     31

/** Constructs a status value for a process stopped by the given signal. */
#define TUIM_STOPCODE(__signal_number) ((__signal_number) << 8 | 0x7f)

/**
   @brief
   @param
   @return */
int   tuim_errno(tuim_ctx *ctx);

/**
   @brief
   @param
   @return */
char *tuim_strerror(tuim_ctx *ctx);

/**
   @brief
   @param
   @return */
void *tuim_getpathexec(tuim_ctx *ctx, const char *elf_name);

/**
   @brief
   @param
   @return */
void *tuim_getpathdyn (tuim_ctx *ctx, const char *elf_name);

/**
   @brief
   @param
   @return */
void tuim_loader(tuim_ctx *ctx, const char *elf_path);

/**
   @brief
   @param
   @return */
void tuim_linker(tuim_ctx *ctx, const char *elf_path);

/**
   @brief
   @param
   @return */
int tuim_exec(
   tuim_ctx *ctx,         /* execution context */
   const char *exec_path, /* path of ELF executable */
   char *const argv[],    /* argument vector */
   char *const envp[]     /* environment pointer */
); /* execve */


#include <stdint.h>
typedef uint64_t tuim_addr;
typedef uint64_t tuim_size;

/**
   @brief
   @param
   @return */
tuim_addr tuim_symbol(
   tuim_ctx *ctx,
   const char *elf_path,
   const char *symbol
);

/**
   @brief
   @param
   @return */
void tuim_jump(
   tuim_ctx *ctx,
   tuim_addr address
);

/**
   @brief
   @param
   @return */
void tuim_read(
   tuim_ctx  *ctx,
   void      *dest,
   tuim_addr  src,
   tuim_size  size
);

/**
   @brief
   @param
   @return */
void tuim_write(
   tuim_ctx   *ctx,
   tuim_addr   dest,
   const void *src,
   tuim_size   size
);

typedef uint32_t tuim_registers_arm[18];

/**
   @brief
   @param
   @return */
void tuim_registers(tuim_ctx *ctx, void *new, void *old);

#ifdef __cplusplus
}
#endif

#endif /* TUIM_H */
