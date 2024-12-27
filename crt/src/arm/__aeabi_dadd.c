#include "aeabi.h"
#include "__aeabi_utils.h"

#if __ARM_FP & 0b1000
[[gnu::naked]]
uint64_t __aeabi_dadd(uint64_t a, uint64_t b){
   asm("VMOV D0, R0, R1");
   asm("VMOV D1, R2, R3");
   asm("VADD.F64 D0, D0, D1");
   asm("VMOV R0, R1, D0");
   asm("BX LR");
}
#else
uint64_t __aeabi_dadd(uint64_t a, uint64_t b){
   unsigned int sing_a, sing_b, exponent_a, exponent_b;
   uint64_t mantissa_a, mantissa_b;

   sing_a = double_sign(a);
   sing_b = double_sign(b);
   exponent_a = double_exponent(a);
   exponent_b = double_exponent(b);
   mantissa_a = double_mantissa(a);
   mantissa_b = double_mantissa(b);

   /* ... */
}
#endif // __ARM_FP & 0b100
