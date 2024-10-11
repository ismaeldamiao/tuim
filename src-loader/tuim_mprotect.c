/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos

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
#include <stddef.h>

#ifndef STATIC
#define STATIC
#endif

/* ------------------------------------
   Function to correct memory protection.
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

#if defined(__unix__)
/* ------------------------------------
UNIX like
------------------------------------ */
#include <sys/mman.h>
STATIC int tuim_mprotect(void *addr, size_t len, int prot){
   return mprotect(addr, len, prot); /* Be happy */
}
#elif defined(_WIN32_API)
/* ------------------------------------
Microsoft Winsdows
------------------------------------ */
#include <memoryapi.h>

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

STATIC int tuim_mprotect(void *addr, size_t len, int prot){
   DWORD flNewProtect;
   PDWORD lpflOldProtect;

   // NOTE: There is a more intelligent way to do this?
   flNewProtect = PAGE_NOACCESS;
   if(prot == PROT_READ){
      flNewProtect = PAGE_READONLY;
   }else if(prot == PROT_WRITE){
      flNewProtect = PAGE_READWRITE;
   }else if(prot == PROT_EXEC){
      flNewProtect = PAGE_EXECUTE;
   }else if(prot == PROT_READ + PROT_EXEC){
      flNewProtect = PAGE_EXECUTE_READ;
   }else{
      flNewProtect = PAGE_EXECUTE_READWRITE;
   }

   // FIXME: The last argument can't be NULL
   return (int)VirtualProtect(addr, len, flNewProtect, NULL);
}
#else
/* ------------------------------------
None
------------------------------------ */
/* One can try to work with the code without protection, however this feature
   invoke undefined behavior and may have security issues. */
STATIC int tuim_mprotect(void *buf, size_t bytes, int flags){
   return -1;
}
#endif
