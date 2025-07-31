#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/** \def DEBUG_MSG(...)
 *  Macro enabled by compiler flag, for added messages to stderr.
 *  With gcc, add "-D DEBUG_MSG"
 */
#ifdef DEBUG_MSG_ON
  #ifndef DEBUG_MSG
    #define DEBUG_MSG(...) fprintf(stderr, ##__VA_ARGS__)
  #endif
#else
  #define DEBUG_MSG(...) do {} while(0);
#endif

void mergesort (double a[], int left, int right);

void merge (double a[], int left, int center, int right);

int main () {

  srand(time(NULL));

  size_t dim = 0;
  int errflag = 0;

  do {
    printf("Select array size: ");
    scanf("%zu", &dim);
  } while (dim == 0);

  double* a = malloc(dim*sizeof(double));

  for (int i = 0; i < dim; i++) {
    a[i] = (double) rand() / RAND_MAX;
    a[i] *= 100.0;
  }


  for (int i = 0; i < dim; i++) {
    DEBUG_MSG("a[%d] = %.2f\n", i, a[i]);
  }

  mergesort(a, 0, dim-1);




  DEBUG_MSG("\nSorted:\n");
  for (int i = 0; i < dim; i++) {
    DEBUG_MSG("a[%d] = %.2f\n", i, a[i]);
  }

  DEBUG_MSG("\nChecking for errors:\n");
  for (int i = 1; i < dim; i++) {
    if (a[i] < a[i-1]) {
      DEBUG_MSG("Error: a[%d] = %.2f > %.2f = a[%d]\n", i-1, a[i-1], a[i], i);
      errflag = 1;
    }
  }
  if (errflag) {
    return EXIT_FAILURE;
  }
  DEBUG_MSG("\nNo errors found.\n");
  return EXIT_SUCCESS;
}

void mergesort (double a[], int left, int right) {
  if (left < right) {
    int center = (left + right) / 2;
    mergesort(a, left, center);
    mergesort(a, center+1, right);
    merge(a, left, center, right);
  }
}

void merge (double a[], int left, int center, int right) {

  int i = left;
  int j = center+1;
  int k = 0;
  double* b = malloc((right-left+1)*sizeof(double));


  while (i <= center && j <= right) {
    if (a[i] <= a[j]) {
      b[k] = a[i];
      i++;
    }
    else {
      b[k] = a[j];
      j++;
    }
    k++;
  }


  while (i <= center) {
    b[k] = a[i];
    i++;
    k++;
  }

  while (j <= right) {
    b[k] = a[j];
    j++;
    k++;
  }

  for (int c = left; c <= right; c++) {
    a[c] = b[c-left];
  }
}
