#ifndef INLINE_MATH_H
#define INLINE_MATH_H 1

#include <math.h>

#if defined(__ARMCC_VERSION) /* ARM Compiler */
#error "TODO: Test this feature on Visual Studio Compiler."
double fabs(double x){
   __asm {
      VABS.F64 x, x
   }
   return x;
}
#elif defined(__GNUC__) /* GNU C Compiler */
   #define fabs(x)
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


#ifndef fabs
   #define fabs(x) ((x) > 0.0 ? (x) : ((x) == 0.0 ? 0.0 : -(x)))
#endif

#endif /* INLINE_MATH_H */
