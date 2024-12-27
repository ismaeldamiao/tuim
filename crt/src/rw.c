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
/* *****************************************************************************
   Run-time ABI for the Arm Architecture.
   5.3.3   Unaligned memory access
   Last modified: December 12, 2024.
***************************************************************************** */

int __aeabi_uread4(void *address){
   int value;
   unsigned char *dest = (typeof(dest))(&value);
   const unsigned char *src = address;
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   return value;
}
int __aeabi_uwrite4(int value, void *address){
   unsigned char *dest = address;
   const unsigned char *src = (typeof(src))(&value);
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   return value;
}
long long __aeabi_uread8(void *address){
   int value;
   unsigned char *dest = (typeof(dest))(&value);
   const unsigned char *src = address;
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   dest[4] = src[4];
   dest[5] = src[5];
   dest[6] = src[6];
   dest[7] = src[7];
   return value;
}
long long __aeabi_uwrite8(long long value, void *address){
   unsigned char *dest = address;
   const unsigned char *src = (typeof(src))(&value);
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   dest[4] = src[4];
   dest[5] = src[5];
   dest[6] = src[6];
   dest[7] = src[7];
   return value;
}

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
