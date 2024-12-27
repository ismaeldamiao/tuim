#include "ieee754.h"

double fabs(double x){
   ieee64_t ieee_x;
   ieee_x.floating = x;
   ieee_x.integer &= UINT64_C(0x7fffffffffffffff);
   return ieee_x.floating;
}
