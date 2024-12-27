#ifndef INLINE_COMPLEX_H
#define INLINE_COMPLEX_H 1

#if defined(__GNUC__)
   #define __CIMAG(z, type) (__imag__ (z))
#else
   #define __CIMAG(z, type) (((union {\
      type _Complex m_complex; type ma_real[2];\
   }){ .m_complex = (type _Complex)(z) }).ma_real[1])
#endif /* defined(__GNUC__) */

#define cimag(z)  __CIMAG(z, double)
#define cimagf(z) __CIMAG(z, float)
#define cimagl(z) __CIMAG(z, long double)

#define creal(z)  ((double)(z))
#define crealf(z) ((float)(z))
#define creall(z) ((long double)(z))

#endif /* INLINE_COMPLEX_H */
