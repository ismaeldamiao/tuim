#include "aeabi.h"

unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator){
   struct qr qr = { .q_n = 0, .r_n = 0 };

   uint_div_qr(numerator, denominator, &qr);

   return qr.q;
}
