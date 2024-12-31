// Function to perform unsigned integer division
unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator) {
   // Initialize the quotient
   unsigned int quotient = 0;

   // Handle division by zero
   if (denominator == 0) {
      // Division by zero is undefined, here we return 0
      return 0xffffffff;
   }

   // Iterate over each bit of the numerator
   for (int i = 31; i >= 0; i--) {
      // Check if the current bit of the numerator is greater than or equal to the denominator
      if ((numerator >> i) >= denominator) {
         // Subtract the shifted denominator from the shifted numerator
         numerator -= (denominator << i);

         // Set the corresponding bit in the quotient
         quotient |= (1U << i);
      }
   }

   return quotient;
}
