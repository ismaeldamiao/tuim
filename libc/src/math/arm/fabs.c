/* *****************************************************************************
   This file contains implementations of the following assembly using the
   inline assembler.

   ```
   fabs:
      VMOV d0, r0, r1
      VABS.F64 d0, d0
      VMOV r0, r1, d0
      BX lr
   ```

   COMPILE (any of):
      armcc -c libm.c
      gcc -c libm.c
      clang -c libm.c
   *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos SANTOS

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

#if defined(__ARMCC_VERSION) /* ARM Compiler */
#error "TODO: Test this feature on Visual Studio Compiler."
double fabs(double x){
   __asm {
      VABS.F64 x, x
   }
   return x;
}
#elif defined(__GNUC__) /* GNU C Compiler */
/* Is Know that it also works on:
   - LLVM CLang
   - Intel C++ Compiler */
[[gnu::naked]]
double fabs(double x){
   //__asm__("VMOV.F64 d0, r0, r1");
   //__asm__("VABS.F64 d0, d0");
   //__asm__("VMOV.F64 r0, r1, d0");
   __asm__("BX LR");
}
#elif defined(_MSC_VER) /* Visual Studio Compiler */
#error "TODO: Test this feature on Visual Studio Compiler."
double fabs(double x){
   __asm {
      VMOV d0, r0, r1
      VABS.F64 d0, d0
      VMOV r0, r1, d0
      BX lr
   };
}
#endif /* __ARMCC_VERSION */
