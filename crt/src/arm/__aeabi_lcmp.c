int __aeabi_lcmp(long long a, long long b) {
   unsigned int a_high = (unsigned int)(a >> 32);
   unsigned int a_low = (unsigned int)a;
   unsigned int b_high = (unsigned int)(b >> 32);
   unsigned int b_low = (unsigned int)b;

   if (a_high < b_high) {
      return -1;
   } else if (a_high > b_high) {
      return 1;
   } else {
      if (a_low < b_low) {
         return -1;
      } else if (a_low > b_low) {
         return 1;
      } else {
         return 0;
      }
   }
}
