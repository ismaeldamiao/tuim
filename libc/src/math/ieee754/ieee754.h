#ifndef IEEE754_H
#define IEEE754_H 1

#ifndef __STDC__
   #error "Use a Standard C Compliant compiler."
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
   #include <stdint.h>
#endif /* __STDC_VERSION__ >= 199901L */

#ifndef UINT32_C
   #include <limits.h>
   #if (UINT_MAX == 0xffffffffU)
      #define UINT32_C(x) x ## U
      typedef unsigned int uint32_t;
   #elif (ULONG_MAX == 0xffffffffUL)
      #define UINT32_C(x) x ## UL
      typedef unsigned long int uint32_t;
   #else
      #error "Can't define the type uint32_t."
   #endif /* UINT_MAX == 0xfffffff */
#endif

typedef union ieee32_u {
   float    floating;
   uint32_t integer;
} ieee32_t;

#if defined(UINT64_C)
   typedef union ieee64_u {
      uint64_t integer;
      double   floating;
   } ieee64_t;
#else
   typedef union ieee64_u {
      double   floating;
      uint32_t integer[2];
   } ieee64_t;
#endif /* defined(UINT64_C) */

#endif /* IEEE754_H */
