#include <stdint.h>
#define asm __asm__

// Helper function to extract the sign bit from a float
static int get_sign(float f) {
   uint32_t* p = (uint32_t*)&f;
   return (*p >> 31) & 1;
}

// Helper function to extract the exponent from a float
static int get_exponent(float f) {
   uint32_t* p = (uint32_t*)&f;
   return ((*p >> 23) & 0xFF) - 127;
}

// Helper function to extract the mantissa from a float
static uint32_t get_mantissa(float f) {
   uint32_t* p = (uint32_t*)&f;
   return *p & 0x007FFFFF;
}

// Non-excepting equality comparison
void __aeabi_cfcmpeq(float a, float b) {
   uint32_t* pa = (uint32_t*)&a;
   uint32_t* pb = (uint32_t*)&b;
   if (*pa == *pb) {
      // Set Z flag (zero) in PSR
      asm volatile("mov r0, #0");
   } else {
      // Clear Z flag (zero) in PSR
      asm volatile("mov r0, #1");
   }
}

// 3-way compare
void __aeabi_cfcmple(float a, float b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint32_t mant_a = get_mantissa(a);
   uint32_t mant_b = get_mantissa(b);

   if (sign_a != sign_b) {
      if (sign_a < sign_b) {
         // a is negative, b is positive
         // Set Z flag (zero) and C flag (carry) in PSR
         asm volatile("mov r0, #0");
         asm volatile("mov r1, #1");
      } else {
         // a is positive, b is negative
         // Clear Z flag (zero) and C flag (carry) in PSR
         asm volatile("mov r0, #1");
         asm volatile("mov r1, #0");
      }
   } else if (exp_a != exp_b) {
      if (exp_a < exp_b) {
         // a is less than b
         // Set Z flag (zero) and C flag (carry) in PSR
         asm volatile("mov r0, #0");
         asm volatile("mov r1, #1");
      } else {
         // a is greater than b
         // Clear Z flag (zero) and C flag (carry) in PSR
         asm volatile("mov r0, #1");
         asm volatile("mov r1, #0");
      }
   } else {
      if (mant_a < mant_b) {
         // a is less than b
         // Set Z flag (zero) and C flag (carry) in PSR
         asm volatile("mov r0, #0");
         asm volatile("mov r1, #1");
      } else if (mant_a > mant_b) {
         // a is greater than b
         // Clear Z flag (zero) and C flag (carry) in PSR
         asm volatile("mov r0, #1");
         asm volatile("mov r1, #0");
      } else {
         // a is equal to b
         // Set Z flag (zero) and clear C flag (carry) in PSR
         asm volatile("mov r0, #0");
         asm volatile("mov r1, #0");
      }
   }
}

// Reversed 3-way compare
void __aeabi_cfrcmple(float a, float b) {
   __aeabi_cfcmple(b, a);
}

// Result (1, 0) denotes (=, ?<>) [2], use for C == and !=
int __aeabi_fcmpeq(float a, float b) {
   uint32_t* pa = (uint32_t*)&a;
   uint32_t* pb = (uint32_t*)&b;
   if (*pa == *pb) {
      return 1;
   } else {
      return 0;
   }
}

// Result (1, 0) denotes (<, ?>=) [2], use for C <
int __aeabi_fcmplt(float a, float b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint32_t mant_a = get_mantissa(a);
   uint32_t mant_b = get_mantissa(b);

   if (sign_a != sign_b) {
      if (sign_a < sign_b) {
         return 1;
      } else {
         return 0;
      }
   } else if (exp_a != exp_b) {
      if (exp_a < exp_b) {
         return 1;
      } else {
         return 0;
      }
   } else {
      if (mant_a < mant_b) {
         return 1;
      } else {
         return 0;
      }
   }
}

// Result (1, 0) denotes (<=, ?>) [2], use for C <=
int __aeabi_fcmple(float a, float b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint32_t mant_a = get_mantissa(a);
   uint32_t mant_b = get_mantissa(b);

   if (sign_a != sign_b) {
      if (sign_a < sign_b) {
         return 1;
      } else {
         return 0;
      }
   } else if (exp_a != exp_b) {
      if (exp_a < exp_b) {
         return 1;
      } else {
         return 0;
      }
   } else {
      if (mant_a <= mant_b) {
         return 1;
      } else {
         return 0;
      }
   }
}

// Result (1, 0) denotes (>=, ?<) [2], use for C >=
int __aeabi_fcmpge(float a, float b) {
   return !__aeabi_fcmplt(a, b);
}

// Result (1, 0) denotes (>, ?<=) [2], use for C >
int __aeabi_fcmpgt(float a, float b) {
   return !__aeabi_fcmple(a, b);
}

// Result (1, 0) denotes (?, <=>) [2], use for C99 isunordered()
int __aeabi_fcmpun(float a, float b) {
   uint32_t* pa = (uint32_t*)&a;
   uint32_t* pb = (uint32_t*)&b;
   if ((*pa & 0x7F800000) == 0x7F800000 || (*pb & 0x7F800000) == 0x7F800000) {
      return 1;
   } else {
      return 0;
   }
}
