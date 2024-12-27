/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#include "aeabi.h"
#include "__aeabi_utils.h"
/* *****************************************************************************
   Run-time ABI for the Arm Architecture.
   5.3.4   Memory copying, clearing, and setting
   Last modified: December 12, 2024.
***************************************************************************** */

#define uintptr_t unsigned int

void __aeabi_memcpy8(void *dest, const void *src, size_t n){
   unsigned long long *c_dest = dest;
   const unsigned long long *c_src = src;
   const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}
void __aeabi_memcpy4(void *dest, const void *src, size_t n){
   unsigned *c_dest = dest;
   const unsigned *c_src = src;
   const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}
void __aeabi_memcpy(void *dest, const void *src, size_t n){
   unsigned char *c_dest = dest;
   const unsigned char *c_src = src;
   const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

// TODO: Implement __aeabi_memmove

void __aeabi_memset8(void *dest, size_t n, int c){
   unsigned long long *c_dest = dest;
   const unsigned int ch = (typeof(ch))c;
   const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}
void __aeabi_memset4(void *dest, size_t n, int c){
   unsigned int *c_dest = dest;
   const unsigned int ch = (typeof(ch))c;
   const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}
void __aeabi_memset(void *dest, size_t n, int c){
   unsigned char *c_dest = dest;
   const unsigned char ch = (typeof(ch))c;
   const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

[[gnu::naked]] void __aeabi_memclr8(void *dest, size_t n){
   asm("MOV R2, #0");
   asm("B __aeabi_memset8");
   //__aeabi_memset8(dest, n, 0);
}
[[gnu::naked]] void __aeabi_memclr4(void *dest, size_t n){
   asm("MOV R2, #0");
   asm("B __aeabi_memset4");
   //__aeabi_memset4(dest, n, 0);
}
[[gnu::naked]] void __aeabi_memclr(void *dest, size_t n){
   asm("MOV R2, #0");
   asm("B __aeabi_memset");
   //__aeabi_memset(dest, n, 0);
}

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
