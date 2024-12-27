#include "aeabi.h"
#include "__aeabi_utils.h"

#if __ARM_FP & 0b100
[[gnu::naked]]
uint32_t __aeabi_fadd(uint32_t a, uint32_t b){
   asm("VMOV S0, R0");
   asm("VMOV S1, R1");
   asm("VADD.F32 S0, S0, S1");
   asm("VMOV R0, S0");
   asm("BX LR");
}
#else

uint32_t __aeabi_fadd(uint32_t a, uint32_t b){
   unsigned int sing_a, sing_b, exponent_a, exponent_b;
   uint32_t mantissa_a, mantissa_b;

   sing_a = float_sign(a);
   sing_b = float_sign(b);
   exponent_a = float_exponent(a);
   exponent_b = float_exponent(b);
   mantissa_a = float_mantissa(a);
   mantissa_b = float_mantissa(b);

   /* ... */
}
#endif // __ARM_FP & 0b100
