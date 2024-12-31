#include <stdint.h>

// Function to perform signed integer division
int32_t __aeabi_idiv(int32_t numerator, int32_t denominator) {
   // Handle division by zero
   if (denominator == 0) {
      // Division by zero is undefined, here we return 0
      return 0;
   }

   // Determine the sign of the result
   int32_t sign = ((numerator < 0) ^ (denominator < 0)) ? -1 : 1;

   // Perform unsigned division on absolute values
   uint32_t abs_numerator = (numerator < 0) ? -numerator : numerator;
   uint32_t abs_denominator = (denominator < 0) ? -denominator : denominator;
   uint32_t quotient = 0;

   // Bitwise division algorithm
   while (abs_numerator >= abs_denominator) {
      abs_numerator -= abs_denominator;
      quotient++;
   }

   // Apply the sign to the quotient
   return sign * quotient;
}
