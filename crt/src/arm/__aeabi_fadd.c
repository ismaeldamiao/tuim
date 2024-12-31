#include <stdint.h>

// Helper function to extract the sign bit from a float
static int get_sign(float f) {
   uint32_t* p = (uint32_t*)&f;
   return ((*p >> 31) & 1);
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

// Helper function to set the sign bit of a float
static void set_sign(float* f, int sign) {
   uint32_t* p = (uint32_t*)&f;
   *p = (*p & 0x7FFFFFFF) | ((uint32_t)sign << 31);
}

// Helper function to set the exponent of a float
static void set_exponent(float* f, int exp) {
   uint32_t* p = (uint32_t*)&f;
   *p = (*p & 0x807FFFFF) | ((uint32_t)(exp + 127) << 23);
}

// Helper function to set the mantissa of a float
static void set_mantissa(float* f, uint32_t mant) {
   uint32_t* p = (uint32_t*)&f;
   *p = (*p & 0xFF800000) | mant;
}

// Single-precision addition
float __aeabi_fadd(float a, float b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint32_t mant_a = get_mantissa(a);
   uint32_t mant_b = get_mantissa(b);

   if (sign_a != sign_b) {
      if (exp_a > exp_b) {
         // a is larger
         float result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a - (mant_b >> (exp_a - exp_b)));
         return result;
      } else if (exp_a < exp_b) {
         // b is larger
         float result;
         set_sign(&result, sign_b);
         set_exponent(&result, exp_b);
         set_mantissa(&result, mant_b - (mant_a >> (exp_b - exp_a)));
         return result;
      } else {
         // a and b have the same exponent
         float result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a - mant_b);
         return result;
      }
   } else {
      if (exp_a > exp_b) {
         // a is larger
         float result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a + (mant_b >> (exp_a - exp_b)));
         return result;
      } else if (exp_a < exp_b) {
         // b is larger
         float result;
         set_sign(&result, sign_b);
         set_exponent(&result, exp_b);
         set_mantissa(&result, mant_b + (mant_a >> (exp_b - exp_a)));
         return result;
      } else {
         // a and b have the same exponent
         float result;
         set_sign(&result, sign_a);
         set_exponent(&result, exp_a);
         set_mantissa(&result, mant_a + mant_b);
         return result;
      }
   }
}

// Single-precision division
float __aeabi_fdiv(float n, float d) {
   int sign_n = get_sign(n);
   int sign_d = get_sign(d);
   int exp_n = get_exponent(n);
   int exp_d = get_exponent(d);
   uint32_t mant_n = get_mantissa(n);
   uint32_t mant_d = get_mantissa(d);

   float result;
   set_sign(&result, sign_n ^ sign_d);
   set_exponent(&result, exp_n - exp_d);
   set_mantissa(&result, mant_n / mant_d);
   return result;
}

// Single-precision multiplication
float __aeabi_fmul(float a, float b) {
   int sign_a = get_sign(a);
   int sign_b = get_sign(b);
   int exp_a = get_exponent(a);
   int exp_b = get_exponent(b);
   uint32_t mant_a = get_mantissa(a);
   uint32_t mant_b = get_mantissa(b);

   float result;
   set_sign(&result, sign_a ^ sign_b);
   set_exponent(&result, exp_a + exp_b);
   set_mantissa(&result, (mant_a * mant_b) >> 23);
   return result;
}

// Single-precision subtraction
float __aeabi_fsub(float x, float y) {
   int sign_x = get_sign(x);
   int sign_y = get_sign(y);
   int exp_x = get_exponent(x);
   int exp_y = get_exponent(y);
   uint32_t mant_x = get_mantissa(x);
   uint32_t mant_y = get_mantissa(y);

   if (sign_x != sign_y) {
      if (exp_x > exp_y) {
         // x is larger
         float result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x + (mant_y >> (exp_x - exp_y)));
         return result;
      } else if (exp_x < exp_y) {
         // y is larger
         float result;
         set_sign(&result, sign_y);
         set_exponent(&result, exp_y);
         set_mantissa(&result, mant_y + (mant_x >> (exp_y - exp_x)));
         return result;
      } else {
         // x and y have the same exponent
         float result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x + mant_y);
         return result;
      }
   } else {
      if (exp_x > exp_y) {
         // x is larger
         float result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x - (mant_y >> (exp_x - exp_y)));
         return result;
      } else if (exp_x < exp_y) {
         // y is larger
         float result;
         set_sign(&result, sign_y);
         set_exponent(&result, exp_y);
         set_mantissa(&result, mant_y - (mant_x >> (exp_y - exp_x)));
         return result;
      } else {
         // x and y have the same exponent
         float result;
         set_sign(&result, sign_x);
         set_exponent(&result, exp_x);
         set_mantissa(&result, mant_x - mant_y);
         return result;
      }
   }
}

// Single-precision reverse subtraction
float __aeabi_frsub(float x, float y) {
   return __aeabi_fsub(y, x);
}
