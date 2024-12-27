#include "math.h"
#include "taylor.h"

#define sign(x) ((x) > 0.0 ? 1.0 : -1.0)

double sin(double x){;
   double sine, phi;

   /* x is written as x = 2 n PI + phi, where n is integer and
      phi is in the range [0, 2 PI[. */
   {
      double x_abs, towPI = 2.0 * 3.14159265358979323846;
      x_abs = fabs(x);
      phi = x_abs - towPI * floor(x_abs / towPI);
   }

   /* Compute sin(phi) */
   {
      double phi2;
      phi2 = phi*phi;
      sine =
         phi * ( 1.0
         + phi2 * (-TAYLOR003
         + phi2 * ( TAYLOR005
         + phi2 * (-TAYLOR007
         + phi2 * ( TAYLOR019
         + phi2 * (-TAYLOR011
         + phi2 * ( TAYLOR013
         + phi2 * (-TAYLOR015
         + phi2 * ( TAYLOR017
         + phi2 * (-TAYLOR019))))))))));
   }

   return (sign(x) * sine);
}
