#ifndef INLINE_MATH_H
#define INLINE_MATH_H 1

#include <math.h>

#ifndef fabs
   #define fabs(x) ((x) > 0.0 ? (x) : ((x) == 0.0 ? 0.0 : -(x)))
#endif

#ifndef floor
   #define floor(x) ((double)((long int)(x)))
#endif

#ifndef ceil
   #define ceil(x) (floor(x) + 1.0)
#endif

#ifndef cos
   #define cos(x) ((double)((int)(x)))
#endif

#endif /* INLINE_MATH_H */
