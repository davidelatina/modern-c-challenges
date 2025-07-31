#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

// two command line arguments:
// - number of elements in array to be sorted
// - log2 of the number of threads to employ

/* From Modern C chapter 18
 * Implement a parallel algorithm for sorting using two threads
 * that builds on your merge sort implementation.
 * That is, a merge sort that cuts the input array in half
 * and sorts each half in its own thread, and then
 * merges the two halves sequentially as before.
 * Generalize this parallel sorting to P threads,
 * with P = 2^k for k = 1, 2, 3, 4...
 * where k is given on the command line
 */

// - regular sequential algorithm for mergesort, in two functions
// splits data in half until each section holds only 1 or 2 values
void mergesort(_Atomic(double*) a, size_t size);

// merges two sorted consecutive halves into one larger sorted section
void merge(_Atomic(double*) a, size_t center, size_t size);

// - parallelization
//spawns additional threads each time it splits data in half.
//thread_num_log2 serves as a downward counter for this purpose,
//after it reaches zero, mergesort is called to proceed sequentially
void mergesort_threaded(_Atomic(double*) a,
                        size_t size,
                        size_t thread_num_log2);

//wrapper and struct to call the function within pthread_create
void* mergesort_threaded_wrp(void* arg);
typedef struct mergesort_threaded_data mergesort_threaded_data;
struct mergesort_threaded_data {
  _Atomic(double*) a;
  size_t size;
  size_t thread_num_log2;
};

int main (int argc, char* argv[]) {
  //set random seed
  srand(time(NULL));


  size_t size;  //first argument: array size
  size_t thread_num_log2; //second argument: log2 of number of threads

  if (argc <= 2) {
    puts("two arguments required: n. of elements to sort, \
          log2 of n. of threads to use");
    return EXIT_SUCCESS;
  }

  //first argument: array size
  errno = 0;
  size = strtoumax(argv[1], NULL, 10);
  if (errno) {
    perror("array size invalid");
    return EXIT_FAILURE;
  }

  //second argument: log2 of number of threads to be used
  errno = 0;
  thread_num_log2 = strtoumax(argv[2], NULL, 10);
  if (errno) {
    perror("number of threads invalid");
    return EXIT_FAILURE;
  }
  if (pow(2.0, (double) thread_num_log2) > size) {
    fprintf(stderr, "number of threads invalid");
    return EXIT_FAILURE;
  }

  //the pointer is atomic, not the data
  _Atomic(double*) a = malloc(size*sizeof(double));
  if (!a) {
    perror("Memory allocation failure");
    return EXIT_FAILURE;
  }
  //create random data
  for (int i = 0; i < size; i++) {
    a[i] = (double) rand() / RAND_MAX;
    a[i] *= 100.0;
  }

  //sort data
  mergesort_threaded(a, size, thread_num_log2);

  //check for errors
  int errflag = 0;
  for (int i = 1; i < size; i++) {
    if (a[i] < a[i-1]) {
      fprintf(stderr, "Error: a[%d] = %.2f > %.2f = a[%d]\n",
              i-1, a[i-1], a[i], i);
      errflag = 1;
    }
  }

  free(a);

  if (errflag)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

void* mergesort_threaded_wrp(void* arg) {
  mergesort_threaded_data* S = (mergesort_threaded_data*) arg;
  mergesort_threaded(S->a, S->size, S->thread_num_log2);
  pthread_exit(NULL);
}

void mergesort_threaded(_Atomic(double*) a,
                        size_t size,
                        size_t thread_num_log2) {
  //check number of threads left to create
  if (thread_num_log2 == 0) {
    // no more threads to create, proceed sequentially
    mergesort(a, size);
    return;
  }

  // split data in half for each thread
  size_t center = size / 2;
  pthread_t tid[2];

  pthread_create(&tid[0], NULL, mergesort_threaded_wrp,
    &((mergesort_threaded_data)
      {a, center, thread_num_log2-1}));

  pthread_create(&tid[1], NULL, mergesort_threaded_wrp,
    &((mergesort_threaded_data)
      {a+center, size-center, thread_num_log2-1}));

  //join threads
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  //merge the halves
  merge(a, center, size);
}

void mergesort(_Atomic(double*) a, size_t size) {
  if (size <= 1) {
    return;
  }
  // simpler switch if pair of values
  if (size == 2) {
    if (a[0] > a[1]) {
      double temp = a[0];
      a[0] = a[1];
      a[1] = temp;
    }
    return;
  }

  // split in half, sort each half then merge
  size_t center = size / 2;

  mergesort(a, center);
  mergesort(a+center, size-center);

  merge(a, center, size);
}

void merge(
    _Atomic(double*) a,      //pointer to start of array section
    size_t center,  //size of first half of array
    size_t size     //total size
  ) {
  //merge and sort 2 contiguous parts of an array after each is sorted
  size_t i = 0;         //iterates over left half
  size_t j = center;    //iterates over right half
  size_t k = 0;         //iterates over result
  double* b = malloc(size*sizeof(double)); //temporary store

  //fill result with smallest value from either sorted half
  while (i < center && j < size) {
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

  //add remaining values after either half is exhausted
  while (i < center) {
    b[k] = a[i];
    i++;
    k++;
  }
  while (j < size) {
    b[k] = a[j];
    j++;
    k++;
  }

  //copy sorted values to original address
  memcpy(a, b, size*sizeof(double));

  //destroy temporary store
  free(b);
}
