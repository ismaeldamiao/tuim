#include "aeabi.h"

__value_in_regs uidiv_return __aeabi_uidivmod(unsigned numerator, unsigned denominator){
   struct qr qr = { .q_n = 0, .r_n = 0 };

   uint_div_qr(numerator, denominator, &qr);

   return ret_uidivmod_values(qr.q, qr.r);
}
