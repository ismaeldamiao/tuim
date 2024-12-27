#include "aeabi.h"

int64_t __aeabi_llsl(int64_t a, int shift){
   //  TODO: Reimplement
   union dword dword = { .dw = a };
   unsigned long hi = dword.w[1];
   unsigned long lo = dword.w[0];

   if (shift >= 32) {
      hi = lo << (shift - 32);
      lo = 0;
   } else if (shift > 0) {
      hi = (hi << shift) | (lo >> (32 - shift));
      lo = lo << shift;
   }

   dword.w[1] = hi;
   dword.w[0] = lo;
   return dword.dw;
}
