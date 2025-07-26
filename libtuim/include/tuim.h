/* *****************************************************************************
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
tuim_ctx *tuim_newcontext(
   const char *tuim_home, const char *ld_library_path, const char *machine);
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

#define TUIM_SIGSEGV         0x1001

int   tuim_errno(tuim_ctx *ctx);
char *tuim_strerror(tuim_ctx *ctx);

/* Get elf path based on their name */
void *tuim_getpathexec(tuim_ctx *ctx, const char *elf_name);
void *tuim_getpathdyn (tuim_ctx *ctx, const char *elf_name);

/* Load and link */
void tuim_loader(tuim_ctx *ctx, const char *elf_path);
void tuim_linker(tuim_ctx *ctx, const char *elf_path);

/* Execute */
int tuim_exec(
   tuim_ctx *ctx,         /* execution context */
   const char *exec_path, /* path of ELF executable */
   char *const argv[],    /* argument vector */
   char *const envp[]     /* environment pointer */
); /* execve */

#ifdef __cplusplus
}
#endif

#endif /* TUIM_H */
