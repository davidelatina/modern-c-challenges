/*
 * Challenge 5 (complex numbers).
 * Can you extend the derivative (Challenge 2) to the complex domain:
 * that is, functions that receive and return double complex values?
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <tgmath.h>

#define EPS 0.00001

double complex sinc_prime(double complex z);

double complex sqrtc_prime(double complex z);

int main () {

  double complex z;
  double complex res;
  int selection = 1;
  double temp = 0;

  for (bool exit = false; exit == false;) {
    printf("Select an option: \n");
    printf("[0] Exit\n");
    printf("[1] F(z) = sin(z)\tf(z) = cos(z)\n");
    printf("[2] F(z) = sqrt(z)\tf(z) = 1/(2*sqrt(z))\n");
    scanf(" %d", &selection);
    switch (selection) {
      case 0:
        printf("Exiting...\n");
        exit = true;
        break;

      case 1:
        printf("Selecting F(z) = sin(z)\n");
        printf("Choose Re(z), Im(z):\n");
        scanf(" %lf", &temp);
        z = temp;
        scanf(" %lf", &temp);
        z += temp * I;
        printf(
          "sin(z) = %lf%+lfi\tcos(z) = %lf%+lfi\n",
          creal(sin(z)),
          cimag(sin(z)),
          creal(cos(z)),
          cimag(cos(z))
        );
        res = sinc_prime(z);
        printf("Symmetrized finite-difference formula: ");
        printf(
          "%lf%+lfi\n",
          creal(res),
          cimag(res)
        );
        break;

      case 2:
        printf("Selecting F(z) = sqrt(z)\n");
        printf("Choose Re(z), Im(z):\n");
        scanf(" %lf", &temp);
        z = temp;
        for (;;) {
          //printf("Im(z) = ");
          scanf(" %lf", &temp);
          if (creal(z) > 0) break;
          if (temp > EPS || temp < -EPS) break;
          printf("the complex square root is not differentiable ");
          printf("along the negative real semiaxis.\n");
          printf("Choose a different value.\n");
        }
        z += temp * I;
        res = 1/(2*sqrt(z));
        printf(
          "\nsqrt(z)= %lf%+lfi\tsqrt'(z) = %lf%+lfi\n",
          creal(sqrt(z)),
          cimag(sqrt(z)),
          creal(res),
          cimag(res)
        );
        printf("Symmetrized finite-difference formula: ");
        res = sqrtc_prime(z);
        printf(" %lf%+lfi\n", creal(res), cimag(res));
        break;

      default:
        printf("Select a valid option.\n");
        break;
    }
  }
  return EXIT_SUCCESS;
}

double complex sinc_prime(double complex z) {
  double complex h = cbrt(DBL_EPSILON) * sin(z);
  //"volatile": prevent compiler optimization
  //make sure difference between z+h and z is representable
  double complex volatile temp;
  temp = z + h;
  h = temp - z;
  return (sin(z + h) - sin(z - h)) / (2*h);
}

double complex sqrtc_prime(double complex z) {
  double complex h = cbrt(DBL_EPSILON) * z;
  //"volatile": prevent compiler optimization
  //make sure difference between z+h and z is representable
  double complex volatile temp;
  temp = z + h;
  h = temp - z;
  return (sqrt(z + h) - sqrt(z - h)) / (2*h);
}
