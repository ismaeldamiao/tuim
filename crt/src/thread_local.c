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
   5.3.5   Thread-local storage (new in v2.01)
   Last modified: December 12, 2024.
***************************************************************************** */
[[gnu::naked]]
void *__aeabi_read_tp(void){
   /* Store in R0 the thread pointer by copying it from the correct register. */
   #if defined(__ARM_ARCH_7A__)
   asm("MRC P15, 0, R0, C13, C0, 2");
   #elif defined(__linux__)
   asm("MOV R0, R9");
   #elif defined(_WIN32_API)
   asm("MRS R0, FS");
   #endif

   /* Return to the caller. */
   asm("BX LR");
}
/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
