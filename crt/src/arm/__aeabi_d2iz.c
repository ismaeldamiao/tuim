#include <stdint.h>

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

// Helper function to extract the sign bit from a float
static int get_signf(float f) {
   uint32_t* p = (uint32_t*)&f;
   return ((*p >> 31) & 1);
}

// Helper function to extract the exponent from a float
static int get_exponentf(float f) {
   uint32_t* p = (uint32_t*)&f;
   return ((*p >> 23) & 0xFF) - 127;
}

// Helper function to extract the mantissa from a float
static uint32_t get_mantissaf(float f) {
   uint32_t* p = (uint32_t*)&f;
   return *p & 0x007FFFFF;
}

// Double to integer C-style conversion
int __aeabi_d2iz(double d) {
   int sign = get_sign(d);
   int exp = get_exponent(d);
   uint64_t mant = get_mantissa(d);

   if (exp < 0) {
      return 0;
   } else if (exp > 31) {
      if (sign) {
         return -2147483648;
      } else {
         return 2147483647;
      }
   } else {
      int result = (int)(mant << (exp - 52));
      if (sign) {
         result = -result;
      }
      return result;
   }
}

// Double to unsigned C-style conversion
unsigned __aeabi_d2uiz(double d) {
   int exp = get_exponent(d);
   uint64_t mant = get_mantissa(d);

   if (exp < 0) {
      return 0;
   } else if (exp > 31) {
      return 4294967295;
   } else {
      unsigned result = (unsigned)(mant << (exp - 52));
      return result;
   }
}

// Double to long long C-style conversion
long long __aeabi_d2lz(double d) {
   int sign = get_sign(d);
   int exp = get_exponent(d);
   uint64_t mant = get_mantissa(d);

   if (exp < 0) {
      return 0;
   } else if (exp > 63) {
      if (sign) {
         return -9223372036854775808U;
      } else {
         return 9223372036854775807;
      }
   } else {
      long long result = (long long)(mant << (exp - 52));
      if (sign) {
         result = -result;
      }
      return result;
   }
}

// Double to unsigned long long C-style conversion
unsigned long long __aeabi_d2ulz(double d) {
   int exp = get_exponent(d);
   uint64_t mant = get_mantissa(d);

   if (exp < 0) {
      return 0;
   } else if (exp > 63) {
      return 18446744073709551615U;
   } else {
      unsigned long long result = (unsigned long long)(mant << (exp - 52));
      return result;
   }
}

// Float to integer C-style conversion
int __aeabi_f2iz(float f) {
   int sign = get_signf(f);
   int exp = get_exponentf(f);
   uint32_t mant = get_mantissaf(f);

   if (exp < 0) {
      return 0;
   } else if (exp > 23) {
      if (sign) {
         return -2147483648;
      } else {
         return 2147483647;
      }
   } else {
      int result = (int)(mant << (exp - 23));
      if (sign) {
         result = -result;
      }
      return result;
   }
}

// Float to unsigned C-style conversion
unsigned __aeabi_f2uiz(float f) {
   int exp = get_exponentf(f);
   uint32_t mant = get_mantissaf(f);

   if (exp < 0) {
      return 0;
   } else if (exp > 23) {
      return 4294967295;
   } else {
      unsigned result = (unsigned)(mant << (exp - 23));
      return result;
   }
}

// Float to long long C-style conversion
long long __aeabi_f2lz(float f) {
   int sign = get_signf(f);
   int exp = get_exponentf(f);
   uint32_t mant = get_mantissaf(f);

   if (exp < 0) {
      return 0;
   } else if (exp > 31) {
      if (sign) {
         return -9223372036854775808;
      } else {
         return 9223372036854775807;
      }
   } else {
      long long result = (long long)(mant << (exp - 23));
      if (sign) {
         result = -result;
      }
      return result;
   }
}

// Float to unsigned long long C-style conversion
unsigned long long __aeabi_f2ulz(float f) {
   int exp = get_exponentf(f);
   uint32_t mant = get_mantissaf(f);

   if (exp < 0) {
      return 0;
   } else if (exp > 31) {
      return 18446744073709551615U;
   } else {
      unsigned long long result = (unsigned long long)(mant << (exp - 23));
      return result;
   }
}
