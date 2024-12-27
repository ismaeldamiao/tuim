#include "aeabi.h"

int64_t __aeabi_llsr(int64_t a, int shift){
   //  TODO: Reimplement
   union dword dword = { .dw = a };
   unsigned long hi = dword.w[1];
   unsigned long lo = dword.w[0];

   if (shift >= 32) {
      lo = hi >> (shift - 32);
      hi = 0;
   } else if (shift > 0) {
      lo = (lo >> shift) | (hi << (32 - shift));
      hi = hi >> shift;
   }

   dword.w[1] = hi;
   dword.w[0] = lo;
   return dword.dw;
}
