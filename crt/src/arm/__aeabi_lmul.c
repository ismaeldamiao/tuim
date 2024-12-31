
long long __aeabi_lmul(long long a, long long b) {
   unsigned int a_high = (unsigned int)(a >> 32);
   unsigned int a_low = (unsigned int)a;
   unsigned int b_high = (unsigned int)(b >> 32);
   unsigned int b_low = (unsigned int)b;

   unsigned long long p0 = (unsigned long long)a_low * b_low;
   unsigned long long p1 = (unsigned long long)a_low * b_high;
   unsigned long long p2 = (unsigned long long)a_high * b_low;
   unsigned long long p3 = (unsigned long long)a_high * b_high;

   unsigned long long result = p0 + (p1 << 32) + (p2 << 32);
   result += p3;

   return (long long)result;
}
