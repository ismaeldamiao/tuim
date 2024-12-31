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

// Helper function to set the sign bit of a double
static void set_sign(double* d, int sign) {
   uint64_t* p = (uint64_t*)&d;
   *p = (*p & 0x7FFFFFFFFFFFFFFF) | ((uint64_t)sign << 63);
}

// Helper function to set the exponent of a double
static void set_exponent(double* d, int exp) {
   uint64_t* p = (uint64_t*)&d;
   *p = (*p & 0x800FFFFFFFFFFFFF) | ((uint64_t)(exp + 1023) << 52);
}

// Helper function to set the mantissa of a double
static void set_mantissa(double* d, uint64_t mant) {
   uint64_t* p = (uint64_t*)&d;
   *p = (*p & 0xFF00000000000000) | mant;
}

// Double-precision addition
double __aeabi_dadd(double a, double b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint64_t mant_a = get_mantissa(a);
   uint64_t mant_b = get_mantissa(b);

   if (sign_a != sign_b) {
      if (exp_a > exp_b) {
         // a is larger
         double result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a - (mant_b >> (exp_a - exp_b)));
         return result;
      } else if (exp_a < exp_b) {
         // b is larger
         double result;
         set_sign(&result, sign_b);
         set_exponent(&result, exp_b);
         set_mantissa(&result, mant_b - (mant_a >> (exp_b - exp_a)));
         return result;
      } else {
         // a and b have the same exponent
         double result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a - mant_b);
         return result;
      }
   } else {
      if (exp_a > exp_b) {
         // a is larger
         double result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a + (mant_b >> (exp_a - exp_b)));
         return result;
      } else if (exp_a < exp_b) {
         // b is larger
         double result;
         set_sign(&result, sign_b);
         set_exponent(&result, exp_b);
         set_mantissa(&result, mant_b + (mant_a >> (exp_b - exp_a)));
         return result;
      } else {
         // a and b have the same exponent
         double result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a + mant_b);
         return result;
      }
   }
}

// Double-precision division
double __aeabi_ddiv(double n, double d) {
   int sign_n = get_sign(n);
   int sign_d = get_sign(d);
   int exp_n = get_exponent(n);
   int exp_d = get_exponent(d);
   uint64_t mant_n = get_mantissa(n);
   uint64_t mant_d = get_mantissa(d);

   double result;
   set_sign(&result, sign_n ^ sign_d);
   set_exponent(&result, exp_n - exp_d);
   set_mantissa(&result, mant_n / mant_d);
   return result;
}

// Double-precision multiplication
double __aeabi_dmul(double a, double b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint64_t mant_a = get_mantissa(a);
   uint64_t mant_b = get_mantissa(b);

   double result;
   set_sign(&result, sign_a ^ sign_b);
   set_exponent(&result, exp_a + exp_b);
   set_mantissa(&result, (mant_a * mant_b) >> 52);
   return result;
}

// Double-precision subtraction
double __aeabi_dsub(double x, double y) {
   int sign_x = get_sign(x);
   int sign_y = get_sign(y);
   int exp_x = get_exponent(x);
   int exp_y = get_exponent(y);
   uint64_t mant_x = get_mantissa(x);
   uint64_t mant_y = get_mantissa(y);

   if (sign_x != sign_y) {
      if (exp_x > exp_y) {
         // x is larger
         double result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x + (mant_y >> (exp_x - exp_y)));
         return result;
      } else if (exp_x < exp_y) {
         // y is larger
         double result;
         set_sign(&result, sign_y);
         set_exponent(&result, exp_y);
         set_mantissa(&result, mant_y + (mant_x >> (exp_y - exp_x)));
         return result;
      } else {
         // x and y have the same exponent
         double result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x + mant_y);
         return result;
      }
   } else {
      if (exp_x > exp_y) {
         // x is larger
         double result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x - (mant_y >> (exp_x - exp_y)));
         return result;
      } else if (exp_x < exp_y) {
         // y is larger
         double result;
         set_sign(&result, sign_y);
         set_exponent(&result, exp_y);
         set_mantissa(&result, mant_y - (mant_x >> (exp_y - exp_x)));
         return result;
      } else {
         // x and y have the same exponent
         double result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x - mant_y);
         return result;
      }
   }
}

// Double-precision reverse subtraction
double __aeabi_drsub(double x, double y) {
   return __aeabi_dsub(y, x);
}
