#include <stdint.h>

// Function to perform unsigned integer division and modulus
uint64_t __aeabi_uidivmod(uint32_t numerator, uint32_t denominator) {
   // Handle division by zero
   if (denominator == 0) {
      // Division by zero is undefined, here we return 0 for both quotient and remainder
      return 0;
   }

   // Initialize the quotient and remainder
   uint32_t quotient = 0;
   uint32_t remainder = numerator;

   // Iterate over each bit of the numerator
   for (int i = 31; i >= 0; i--) {
      // Check if the current bit of the numerator is greater than or equal to the denominator
      if ((remainder >> i) >= denominator) {
         // Subtract the shifted denominator from the shifted numerator
         remainder -= (denominator << i);

         // Set the corresponding bit in the quotient
         quotient |= (1U << i);
      }
   }

   // Pack quotient and remainder into a 64-bit value
   return ((uint64_t)remainder << 32) | quotient;
}
