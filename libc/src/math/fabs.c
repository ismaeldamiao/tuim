#include "libm.h"

#if defined(LIBM_ARM)
   #include "arm/fabs.c"
#elif defined(LIBM_IEEE754)
   #include "ieee754/fabs.c"
#else
   #include "stdc/fabs.c"
#endif
