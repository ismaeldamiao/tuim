typedef struct {
   long long quot;
   long long rem;
} lldiv_t;

lldiv_t __aeabi_ldivmod(long long n, long long d) {
   lldiv_t result;
   if (d == 0) {
      // Handle division by zero
      // You can either return an error or abort the program
      // For this example, we'll just return an error
      result.quot = 0;
      result.rem = n;
   } else {
      unsigned long long abs_n = (n < 0) ? -n : n;
      unsigned long long abs_d = (d < 0) ? -d : d;
      unsigned long long quot = 0;
      unsigned long long rem = abs_n;

      while (rem >= abs_d) {
         unsigned long long shift = 0;
         while ((abs_d << shift) <= rem) {
               shift++;
         }
         shift--;
         rem -= abs_d << shift;
         quot += 1LL << shift;
      }

      if ((n < 0) ^ (d < 0)) {
         quot = -quot;
      }
      result.quot = quot;
      result.rem = (n < 0) ? -rem : rem;
   }
   return result;
}
