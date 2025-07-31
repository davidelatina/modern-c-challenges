//CHALLENGE 3 (π). Compute the N first decimal places of π.

/*
Algorithm taken from 
http://www.stanleyrabinowitz.com/bibliography/spigot.pdf
*/
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t predigit = 0;
  size_t quotient = 0;
  //number of nines to hold back, as potential future zeroes
  size_t queue = 0;

  //Step 1
  size_t n = 0;
  do {
    printf("N. of decimal digits: ");
    scanf(" %zu", &n);
  } while (n < 1);
  n += 2; //first two places are "03" before the decimals
  size_t len = n * 10 / 3;
  size_t* arr = malloc(len * sizeof(size_t));
  if (!arr) {
    perror("Out of memory");
    return EXIT_FAILURE;
  }
  for (size_t i = 0; i < len; i++) {
    arr[i] = 2;
  }

  //Step 2
  for (size_t i = 0; i < n; i++) {
    //reset final leftmost quotient
    quotient = 0;

    //Multiply all elements by 10
    for (size_t j = 0; j < len; j++) {
      arr[j] *= 10;
    }

    //Normalize each element by modulo 2j+1
    for (size_t j = len-1; j > 0; j--) {
      while (arr[j] >= 2*j+1) {
        arr[j-1] += j;
        arr[j] -= 2*j+1;
      }
    }

    //Get the next predigit
    while (arr[0] >= 10) {
      quotient++;
      arr[0] -= 10;
    }

    //Adjust predigits
    if (quotient == 9) {
      queue++;
    } else
    if (quotient == 10) {
      printf("%zu", predigit+1);
      while (queue > 0) {
        printf("0");
        queue--;
      }
      predigit = 0;
    } else {
      printf("%zu", predigit);
      predigit = quotient;
      while (queue > 0) {
        printf("9");
        queue--;
      }
    }
  }
  free(arr);
  printf("\n");
  return EXIT_SUCCESS;
}
