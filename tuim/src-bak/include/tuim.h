/** ----------------------------------------------------------------------------
   @file
      tuim.h
   @brief
      The C API for Tuim.

      The `<tuim.h>` header file exposes the Tuim API for C applications.
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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------ Version ------------ */

/** The Tuim's version.

   0 means that it's designed only for test purposes. */
#define TUIM_VERSION_MAJOR    0

/** The Tuim's minor version */
#define TUIM_VERSION_MINOR    3

/* ------------ Error handing ------------ */

/* --- no error identified --- */

/** Success... */
#define TUIM_SUCCESS          0x0000

/** Unknown error */
#define TUIM_EUNKNOWN         0x7fff

/* --- load time errors --- */

/** No such file or directory */
#define TUIM_ENOENT           0x0001

/** Not a ELF file */
#define TUIM_ENOELF           0x0002

/** Not a executable */
#define TUIM_ENOEXEC          0x0003

/** Not a shared object */
#define TUIM_ENODYN           0x0004

/** Bad ELF attributes */
#define TUIM_EELFBAD          0x0005

/** Invalid argument */
#define TUIM_EINVAL           0x0006

/** Cannot allocate memory */
#define TUIM_ENOMEM           0x0007

/** Library format error */
#define TUIM_ENOSYM           0x0008

/* --- run time errors --- */

/** Segmentation fault */
#define TUIM_EFAULT           0x1000

/** Illegal instruction */
#define TUIM_EILL             0x1001

/* ------------ typedefs ------------ */

/** Object to store informations related to a execution context */
typedef struct tuim_ctx_s     tuim_ctx;

/** Unsigned integer capable to hold any address on a execution context */
typedef uint64_t              tuim_addr;

/** Unsigned integer capable to index any array on a execution context */
typedef uint64_t              tuim_size;

typedef uint32_t              tuim_registers_arm[18];

/* ------------ basic functions ------------ */

/**
   @brief
      Create a new execution context.
   @param tuim_home
      TODO: Complete
   @param ld_library_path
      TODO: Complete
   @param machine
      TODO: Complete
   @return
      On success return a valid pointer to a tuim_ctx object.
      
      On error return a null pointer if the execution context can't be created,
      otherwise return a valid pointer to a tuim_ctx object and set
      context's errno. */
tuim_ctx *tuim_newcontext(
   const char *tuim_home, const char *ld_library_path, const char *machine);

/**
   @brief
      Release a execution context.
   @param ctx
      TODO: Complete */
void tuim_deletecontext(tuim_ctx *ctx);


/**
   @brief
      Get the status code of the execution context
   @param
      TODO: Complete
   @return
      TODO: Complete */
int tuim_errno(tuim_ctx *ctx);

/**
   @brief
      Get a string that describes the latest error
   @param ctx
      TODO: complete
   @return
      TODO: complete */
char *tuim_strerror(tuim_ctx *ctx);

/**
   @brief
      Get the path of a executable from their name
   @param ctx
      TODO: complete
   @param elf_name
      TODO: complete
   @return
      TODO: complete */
void *tuim_getpathexec(tuim_ctx *ctx, const char *elf_name);

/**
   @brief
      Get the path of a library from their name
   @param ctx
      TODO: complete
   @param elf_name
      TODO: complete
   @return
      TODO: complete */
void *tuim_getpathdyn (tuim_ctx *ctx, const char *elf_name);

/**
   @brief
      Load a ELF on the execution context
   @param ctx
      TODO: complete
   @param elf_apth
      TODO: complete
   @return
      TODO: complete */
void tuim_loader(tuim_ctx *ctx, const char *elf_path);

/**
   @brief
      Link a ELF on the execution context
   @param ctx
      TODO: complete
   @param elf_path
      TODO: complete
   @return
      TODO: complete */
void tuim_linker(tuim_ctx *ctx, const char *elf_path);

/**
   @brief
      Execute a ELF on the execution context
   @param ctx
      TODO: complete
   @param argv
      TODO: complete
   @param envp
      TODO: complete
   @return
      TODO: complete */
int tuim_exec(
   tuim_ctx *ctx,         /* execution context */
   const char *exec_path, /* path of ELF executable */
   char *const argv[],    /* argument vector */
   char *const envp[]     /* environment pointer */
); /* execve */


/* ------------ advanced functions ------------ */

/**
   @brief
      Get the address of a symbol in a library
   @param ctx
      TODO: complete
   @param elf_path
      TODO: complete
   @param symbol
      TODO: complete
   @return
      TODO: complete */
tuim_addr tuim_symbol(
   tuim_ctx *ctx,
   const char *elf_path,
   const char *symbol
);

/**
   @brief
      TODO: complete
   @param ctx
      TODO: complete
   @param address
      TODO: complete
   @return
      TODO: complete */
void tuim_jump(
   tuim_ctx *ctx,
   tuim_addr address
);

/**
   @brief
      TODO: complete
   @param ctx
      TODO: complete
   @param dest
      TODO: complete
   @param src
      TODO: complete
   @param size
      TODO: complete
   @return
      TODO: complete */
void tuim_read(
   tuim_ctx  *ctx,
   void      *dest,
   tuim_addr  src,
   tuim_size  size
);

/**
   @brief
      TODO: complete
   @param ctx
      TODO: complete
   @param dest
      TODO: complete
   @param src
      TODO: complete
   @param size
      TODO: complete
   @return
      TODO: complete */
void tuim_write(
   tuim_ctx   *ctx,
   tuim_addr   dest,
   const void *src,
   tuim_size   size
);



/**
   @brief
      TODO: complete
   @param ctx
      TODO: complete
   @param new
      TODO: complete
   @param old
      TODO: complete
   @return
      TODO: complete */
void tuim_registers(tuim_ctx *ctx, void *new, void *old);

#ifdef __cplusplus
}
#endif

#endif /* TUIM_H */
