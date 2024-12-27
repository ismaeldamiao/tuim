#ifndef LIBM_H
#define LIBM_H 1
#include <math.h>
#if (defined(__clang__) && defined(__java__))
/* Allow the use of the sources in `java`.
   TODO: Implement a extension to LLVM that allows compilation to java. */
#define LIBM_JAVA
#endif /* defined(__java__) */

#if defined(__ARMCC_VERSION) || \
   (defined(__GNUC__) && defined(__arm__)) || \
   (defined(_MSC_VER) && defined(__ARM_ARCH))
/* Allow the use of the sources in `arm`. */
#define LIBM_ARM
#endif /* defined(__ARMCC_VERSION) */

#if defined(__STDC_IEC_559__) || \
   defined(_MSC_VER)
/* Allow the use of the sources in `ieee754`. */
#define LIBM_IEEE754
#endif /* defined(__STDC_IEC_559__) */



#endif /* LIBM_H */
