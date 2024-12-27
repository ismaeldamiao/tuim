#include "math.h"
#include "taylor.h"

double (cos)(double x){
   double cosine, phi;

   /* x is written as x = 2 n PI + phi, where n is integer and
      phi is in the range [0, 2 PI[. */
   {
      double x_abs, towPI = 2.0 * 3.14159265358979323846;
      x_abs = fabs(x);
      phi = x_abs - towPI * floor(x_abs / towPI);
   }

   /* Compute cos(phi) */
   phi *= phi;
   cosine = 1.0
      + phi * (-TAYLOR002
      + phi * ( TAYLOR004
      + phi * (-TAYLOR006
      + phi * ( TAYLOR008
      + phi * (-TAYLOR010
      + phi * ( TAYLOR012
      + phi * (-TAYLOR014
      + phi * ( TAYLOR016
      + phi * (-TAYLOR018
      + phi * ( TAYLOR020))))))))));

   return cosine;
}
