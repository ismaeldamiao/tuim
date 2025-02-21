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
#ifndef TUIM_H
#define TUIM_H

#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------------
   Public header file for the Tuim project.
   * Last modified: January 23, 2025.
------------------------------------ */

/* Version */
#define TUIM_VERSION_MAJOR 0
#define TUIM_VERSION_MINOR 2

// FIXME: Among various tests erros codes 'estan perfecto lio'.
#define TUIM_SUCCESS         0x0000
#define TUIM_ENOENT          0x0002 /* No such file or directory */
#define TUIM_ENOELF          0x0003 /* Not a ELF file */
#define TUIM_ENOEXEC         0x0004 /* Exec format error */
#define TUIM_ENODYN          0x0005 /* Library format error */
#define TUIM_EFAULT          0x0001 /* Segmentation fault */
#define TUIM_ENOMEM          0x0006 /* Cannot allocate memory */
#define TUIM_EINVAL          0x0007 /* Invalid argument */

#define TUIM_HOME 0
#define LD_LIBRARY_PATH 1
int tuim_setenv(int env, char *value);

int tuim_run(const char *machine, char **argv);
int tuim_build(char **argv);
int tuim_install(char **argv);
int tuim_remove(char **argv);

#ifdef __cplusplus
}
#endif

#endif /* TUIM_H */
