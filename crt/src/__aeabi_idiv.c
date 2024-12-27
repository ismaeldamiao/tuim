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

int __aeabi_idiv(int numerator, int denominator){
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

   return qr.q;
}