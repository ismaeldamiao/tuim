#include <stdint.h>
#define asm __asm__

// Helper function to extract the sign bit from a double
static int get_sign(double d) {
   uint64_t* p = (uint64_t*)&d;
   return ((*p >> 63) & 1);
}

// Helper function to extract the exponent from a double
static int get_exponent(double d) {
   uint64_t* p = (uint64_t*)&d;
   return ((*p >> 52) & 0x7FF) - 1023;
}

// Helper function to extract the mantissa from a double
static uint64_t get_mantissa(double d) {
   uint64_t* p = (uint64_t*)&d;
   return *p & 0xFFFFFFFFFFFFF;
}

// Non-excepting equality comparison
void __aeabi_cdcmpeq(double a, double b) {
   uint64_t* pa = (uint64_t*)&a;
   uint64_t* pb = (uint64_t*)&b;
   if (*pa == *pb) {
      // Set Z flag (zero) in PSR
      asm volatile("mov r0, #0");
   } else {
      // Clear Z flag (zero) in PSR
      asm volatile("mov r0, #1");
   }
}

// 3-way compare
void __aeabi_cdcmple(double a, double b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint64_t mant_a = get_mantissa(a);
   uint64_t mant_b = get_mantissa(b);

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
void __aeabi_cdrcmple(double a, double b) {
   __aeabi_cdcmple(b, a);
}

// Result (1, 0) denotes (=, ?<>) [2], use for C == and !=
int __aeabi_dcmpeq(double a, double b) {
   uint64_t* pa = (uint64_t*)&a;
   uint64_t* pb = (uint64_t*)&b;
   if (*pa == *pb) {
      return 1;
   } else {
      return 0;
   }
}

// Result (1, 0) denotes (<, ?>=) [2], use for C <
int __aeabi_dcmplt(double a, double b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint64_t mant_a = get_mantissa(a);
   uint64_t mant_b = get_mantissa(b);

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
int __aeabi_dcmple(double a, double b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint64_t mant_a = get_mantissa(a);
   uint64_t mant_b = get_mantissa(b);

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
int __aeabi_dcmpge(double a, double b) {
   return !__aeabi_dcmplt(a, b);
}

// Result (1, 0) denotes (>, ?<=) [2], use for C >
int __aeabi_dcmpgt(double a, double b) {
   return !__aeabi_dcmple(a, b);
}

// Result (1, 0) denotes (?, <=>) [2], use for C99 isunordered()
int __aeabi_dcmpun(double a, double b) {
   uint64_t* pa = (uint64_t*)&a;
   uint64_t* pb = (uint64_t*)&b;
   if ((*pa & 0x7FF0000000000000) == 0x7FF0000000000000 || (*pb & 0x7FF0000000000000) == 0x7FF0000000000000) {
      return 1;
   } else {
      return 0;
   }
}
