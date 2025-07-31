/*
CHALLENGE 2 (Numerical derivatives).
implement the numerical derivative double f(double x) of a function double F(double x).
Implement this with an example F for the function that you use for this exercise. A good
primary choice for F would be a function for which you know the derivative, such as sin,
cos, or sqrt. This allows to check your results for correctness.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

double sin_prime(double x);

double sqrt_prime(double x);

int main () {

  double x;
  char selection = 1;

  for (bool exit = false; exit == false;) {
    printf("Select an option: \n");
    printf("[0] Exit\n");
    printf("[1] F(x) = sin(x)\tf(x) = cos(x)\n");
    printf("[2] F(x) = sqrt(x)\tf(x) = 1/(2*sqrt(x))\n");
    scanf(" %c", &selection);
    switch (selection) {
      case '0': printf("Exiting...\n");
                exit = true;
                break;

      case '1': printf("Selecting F(x) = sin(x)\n");
                printf("Value of x: \t");
                scanf(" %lf", &x);
                printf("sin(x)= %lf\tcos(x) = %lf\n", sin(x), cos(x));
                printf("Symmetrized finite-difference formula: ");
                printf("\t%lf\n", sin_prime(x));
                break;

      case '2': printf("Selecting F(x) = sqrt(x)\n");
                do {
                  printf("Value of x (> 0): \t");
                  scanf(" %lf", &x);
                } while (x <= 0.0);
                printf("sqrt(x)= %lf\tsqrt'(x) = %lf\n", sqrt(x), 1/(2*sqrt(x)));
                printf("Symmetrized finite-difference formula: ");
                printf("\t%lf\n", sqrt_prime(x));
                break;

      default:  printf("Select a valid option.\n");
                break;
    }
  }

  return EXIT_SUCCESS;
}

double sin_prime(double x) {
  //double curvscale = fabs(sin(x)); //for lack of better options
  double h = 0.001;//DBL_EPSILON;//*curvscale;

  //prevent compiler optimization
  double volatile temp;
  temp = x + h;
  if (temp == x) temp = nextafter(x, DBL_MAX);
  h = temp - x;

  return (sin(x + h) - sin(x - h)) / (2*h);
}

double sqrt_prime(double x) {
  double curvscale = 2/cbrt(3)*x;
  double h = 0.001;//DBL_EPSILON;// * curvscale;

  //prevent compiler optimization
  double volatile temp;
  temp = x + h;
  if (temp == x) {
    temp = nextafter(x, DBL_MAX);
    puts("error");
  }
  h = temp - x;
  
  if (x-h == x) puts("error");

  return (sqrt(x + h) - sqrt(x - h)) / (2*h);

}
