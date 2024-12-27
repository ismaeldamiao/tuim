#include "aeabi.h"

static void division_qr(unsigned n, unsigned p, struct qr *qr)
{
   unsigned i = 1, q = 0;
   if (p == 0) {
      qr->r = 0xFFFFFFFF;	/* division by 0 */
      return;
   }

   while ((p >> 31) == 0) {
      i = i << 1;	/* count the max division steps */
      p = p << 1;     /* increase p until it has maximum size*/
   }

   while (i > 0) {
      q = q << 1;	/* write bit in q at index (size-1) */
      if (n >= p)
      {
         n -= p;
         q++;
      }
      p = p >> 1; 	/* decrease p */
      i = i >> 1; 	/* decrease remaining size in q */
   }
   qr->r = n;
   qr->q = q;
}

static void uint_div_qr(unsigned numerator, unsigned denominator, struct qr *qr)
{

   division_qr(numerator, denominator, qr);

   /* negate quotient and/or remainder according to requester */
   if (qr->q_n)
      qr->q = -qr->q;
   if (qr->r_n)
      qr->r = -qr->r;
}

static idiv_return ret_idivmod_values(signed quotient, signed remainder){
   asm("BX LR");
}
static uidiv_return  ret_uidivmod_values(unsigned quotient, unsigned remainder){
   asm("BX LR");
}

__value_in_regs idiv_return __aeabi_idivmod(int numerator, int denominator){
   // TODO: reimplement
   struct qr qr = { .q_n = 0, .r_n = 0 };

   if (((numerator < 0) && (denominator > 0)) ||
       ((numerator > 0) && (denominator < 0)))
      qr.q_n = 1;	/* quotient shall be negate */
   if (numerator < 0) {
      numerator = -numerator;
      qr.r_n = 1;	/* remainder shall be negate */
   }
   if (denominator < 0)
      denominator = -denominator;

   uint_div_qr(numerator, denominator, &qr);

   return ret_idivmod_values(qr.q, qr.r);
}
