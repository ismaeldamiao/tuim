#ifndef __AEABI_UTILS_H
#define __AEABI_UTILS_H

#if defined(__GNUC__)
#define asm __asm__
#endif

#define double_sign(x) ((x) >> 61)
#define double_exponent(x) (((x) >> 52) & 0x7FF)
#define double_mantissa(x) ((x) & UINT64_C(0xFFFFFFFFFFFFF))

#define float_sign(x) ((x) >> 31)
#define float_exponent(x) (((x) >> 23) & 0xFF)
#define float_mantissa(x) ((x) & UINT32_C(0x007FFFFF))

#endif // __AEABI_UTILS_H
