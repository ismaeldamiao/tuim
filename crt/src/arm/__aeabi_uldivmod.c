typedef struct {
   unsigned long long quot;
   unsigned long long rem;
} ulldiv_t;

ulldiv_t __aeabi_uldivmod(unsigned long long n, unsigned long long d) {
   ulldiv_t result;
   if (d == 0) {
      // Handle division by zero
      // You can either return an error or abort the program
      // For this example, we'll just return an error
      result.quot = 0;
      result.rem = n;
   } else {
      unsigned long long quot = 0;
      unsigned long long rem = n;

      while (rem >= d) {
         unsigned long long shift = 0;
         while ((d << shift) <= rem) {
               shift++;
         }
         shift--;
         rem -= d << shift;
         quot += 1ULL << shift;
      }

      result.quot = quot;
      result.rem = rem;
   }
   return result;
}
