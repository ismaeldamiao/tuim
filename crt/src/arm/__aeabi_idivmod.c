#include <stdint.h>

// Function to perform signed integer division and modulus
int64_t __aeabi_idivmod(int32_t numerator, int32_t denominator) {
   // Handle division by zero
   if (denominator == 0) {
      // Division by zero is undefined, here we return 0 for both quotient and remainder
      return 0;
   }

   // Determine the sign of the result
   int32_t sign = ((numerator < 0) ^ (denominator < 0)) ? -1 : 1;

   // Perform unsigned division on absolute values
   uint32_t abs_numerator = (numerator < 0) ? -numerator : numerator;
   uint32_t abs_denominator = (denominator < 0) ? -denominator : denominator;
   uint32_t quotient = 0;
   uint32_t remainder = abs_numerator;

   // Bitwise division algorithm
   while (remainder >= abs_denominator) {
      remainder -= abs_denominator;
      quotient++;
   }

   // Apply the sign to the quotient
   quotient = sign * quotient;

   // Apply the sign to the remainder if the numerator was negative
   remainder = (numerator < 0) ? -remainder : remainder;

   // Pack quotient and remainder into a 64-bit value
   return ((int64_t)remainder << 32) | (uint32_t)quotient;
}
