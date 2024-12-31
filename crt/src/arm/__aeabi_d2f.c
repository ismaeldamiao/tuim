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

// Helper function to set the sign bit of a float
static void set_signf(float* f, int sign) {
   uint32_t* p = (uint32_t*)&f;
   *p = (*p & 0x7FFFFFFF) | ((uint32_t)sign << 31);
}

// Helper function to set the exponent of a float
static void set_exponentf(float* f, int exp) {
   uint32_t* p = (uint32_t*)&f;
   *p = (*p & 0x807FFFFF) | ((uint32_t)(exp + 127) << 23);
}

// Helper function to set the mantissa of a float
static void set_mantissaf(float* f, uint32_t mant) {
   uint32_t* p = (uint32_t*)&f;
   *p = (*p & 0xFF800000) | mant;
}

// Double to float (single precision) conversion
float __aeabi_d2f(double d) {
   int sign = get_sign(d);
   int exp = get_exponent(d);
   uint64_t mant = get_mantissa(d);

   if (exp < -1022) {
      // underflow
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, -126);
      set_mantissaf(&result, 0);
      return result;
   } else if (exp > 1023) {
      // overflow
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, 127);
      set_mantissaf(&result, 0x007FFFFF);
      return result;
   } else {
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, exp - 896);
      set_mantissaf(&result, mant >> 29);
      return result;
   }
}

// Float (single precision) to double conversion
double __aeabi_f2d(float f) {
   int sign = get_signf(f);
   int exp = get_exponentf(f);
   uint32_t mant = get_mantissaf(f);

   if (exp < -126) {
      // underflow
      double result;
      set_sign(&result, sign);
      set_exponent(&result, -1022);
      set_mantissa(&result, 0);
      return result;
   } else if (exp > 127) {
      // overflow
      double result;
      set_sign(&result, sign);
      set_exponent(&result, 1023);
      set_mantissa(&result, 0xFFFFFFFFFFFFF);
      return result;
   } else {
      double result;
      set_sign(&result, sign);
      set_exponent(&result, exp + 896);
      set_mantissa(&result, mant << 29);
      return result;
   }
}

// IEEE 754 binary16 storage format (VFP half precision) to binary32 (float) conversion
float __aeabi_h2f(short hf) {
   int sign = (hf >> 15) & 1;
   int exp = (hf >> 10) & 0x1F;
   uint16_t mant = hf & 0x3FF;

   if (exp == 0) {
      // subnormal
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, -14);
      set_mantissaf(&result, mant << 13);
      return result;
   } else if (exp == 31) {
      // infinity or NaN
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, 127);
      set_mantissaf(&result, mant << 13);
      return result;
   } else {
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, exp - 15);
      set_mantissaf(&result, mant << 13);
      return result;
   }
}

// IEEE 754 binary16 storage format (VFP alternative format) to binary32 (float) conversion
float __aeabi_h2f_alt(short hf) {
   int sign = (hf >> 15) & 1;
   int exp = (hf >> 10) & 0x1F;
   uint16_t mant = hf & 0x3FF;

   if (exp == 0) {
      // subnormal
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, -14);
      set_mantissaf(&result, mant << 13);
      return result;
   } else if (exp == 31) {
      // infinity or NaN
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, 127);
      set_mantissaf(&result, mant << 13);
      return result;
   } else {
      float result;
      set_signf(&result, sign);
      set_exponentf(&result, exp - 15);
      set_mantissaf(&result, mant << 13);
      return result;
   }
}
