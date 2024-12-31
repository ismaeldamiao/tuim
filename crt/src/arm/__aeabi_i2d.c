#include <stdint.h>

#define abs(i) ((i) > 0 ? (i) : -(i))

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

// Integer to double conversion
double __aeabi_i2d(int i) {
    double result;
    set_sign(&result, i < 0);
    set_exponent(&result, 0);
    set_mantissa(&result, (uint64_t)abs(i) << 32);
    return result;
}

// Unsigned to double conversion
double __aeabi_ui2d(unsigned u) {
    double result;
    set_sign(&result, 0);
    set_exponent(&result, 0);
    set_mantissa(&result, (uint64_t)u << 32);
    return result;
}

// Long long to double conversion
double __aeabi_l2d(long long l) {
    double result;
    set_sign(&result, l < 0);
    set_exponent(&result, 0);
    set_mantissa(&result, (uint64_t)abs(l) << 32);
    return result;
}

// Unsigned long long to double conversion
double __aeabi_ul2d(unsigned long long ul) {
    double result;
    set_sign(&result, 0);
    set_exponent(&result, 0);
    set_mantissa(&result, ul << 32);
    return result;
}

// Integer to float (single precision) conversion
float __aeabi_i2f(int i) {
    float result;
    set_signf(&result, i < 0);
    set_exponentf(&result, 0);
    set_mantissaf(&result, (uint32_t)abs(i) << 16);
    return result;
}

// Unsigned to float (single precision) conversion
float __aeabi_ui2f(unsigned u) {
    float result;
    set_signf(&result, 0);
    set_exponentf(&result, 0);
    set_mantissaf(&result, (uint32_t)u << 16);
    return result;
}

// Long long to float (single precision) conversion
float __aeabi_l2f(long long l) {
    float result;
    set_signf(&result, l < 0);
    set_exponentf(&result, 0);
    set_mantissaf(&result, (uint32_t)abs(l) << 16);
    return result;
}

// Unsigned long long to float (single precision) conversion
float __aeabi_ul2f(unsigned long long ul) {
    float result;
    set_signf(&result, 0);
    set_exponentf(&result, 0);
    set_mantissaf(&result, (uint32_t)ul << 16);
    return result;
}
