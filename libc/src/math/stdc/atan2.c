#include "math.h"

#define M_PI_2          1.57079632679489661923  /* pi/2 */

double (atan2)(double x, double y){
   if(x > 0.0)
      return atan(y / x);
   else if(x == 0.0){
      if(y >= 0.0)
         return M_PI_2;
      else if(y < 0.0)
         return -M_PI_2;
      else
         return NAN;
   }else{
      if(y >= 0.0)
         return atan(y / x) + M_PI;
      else
         return atan(y / x) - M_PI;
   }
}
