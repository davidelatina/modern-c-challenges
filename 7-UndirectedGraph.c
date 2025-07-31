/**
 * @file 7-UndirectedGraph.c
 * @brief CHALLENGE 7 (Adjacency matrix).
 * The adjacency matrix of a graph G is a matrix A that
 * holds a value true or false in element A[i][j]
 * if there is an arc from node i to node j.
 * At this point, can you use an adjacency matrix
 * to conduct a breadth-first search in a graph G?
 * Can you find connected components? Can you find a spanning tree?
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

typedef struct queue queue;
/**
 * @brief fixed-size queue
 */
struct queue {
  size_t size;
  size_t start;
  size_t end;
  size_t item[];
};
queue* queue_init(size_t size);
void queue_del(queue* Q);
void queue_add(queue* Q, size_t n);
size_t queue_take(queue* Q);

/**
 * @brief Initialize spanning tree.
 *
 * Takes form of an array where each element points to its parent,
 * as seen in 4-UnionFind.c
 *
 * @return size_t*
 */
size_t* span_tree(size_t dim, bool adjacent[dim][dim], size_t root);

int main(void) {

  // Seed for pseudorandom numbers
  srand(time(NULL));

  // number of nodes in graph
  size_t n = 0;

  for (;;) {
    puts("Select number of nodes in graph (> 1):");
    scanf(" %zu", &n);
    if (n >= 2) break;
  }

  // initialize adjacency matrix
  bool (*adjacent)[n] = calloc(1, sizeof(double[n][n]));
  if (!adjacent) {
    perror("out of memory");
    exit(EXIT_FAILURE);
  }

  // Create random graph
  // graph not necessarily connected
  // graph does not allow nodes connected to themselves
  for (size_t i = 1; i < n; i++) {
    for (size_t j = 0; j < i; j++) {
      adjacent[i][j] = (bool) (rand() % 2);
      adjacent[j][i] = adjacent[i][j];
    }
  }

  // Print graph adjacency matrix
  puts("Adjacency matrix:");
  puts("");
  for (size_t i = 0; i < n; i++) {
    for(size_t j = 0; j < n; j++) {
      fputs(adjacent[i][j] ? "1 " : "0 ", stdout);
    }
    puts("");
  }

  // User selects root element
  size_t root = SIZE_MAX;
  do {
    printf("Select root element to extract a spanning tree " \
    "(< %zu):\n", n);
    scanf(" %zu", &root);
  } while (root >= n);

  // Generate spanning tree
  size_t* parent = span_tree(n, adjacent, root);

  // Print result
  puts("Spanning tree:");
  for (size_t i = 0; i < n; i++) {
    if (i == root) {
      fputs("R ", stdout);
    }
    else if (parent[i] == SIZE_MAX) {
      fputs("- ", stdout);
    } else {
      printf("%zu ", parent[i]);
    }
  }
  puts("");

  // De-allocate memory
  free(parent);
  free(adjacent);

  return EXIT_SUCCESS;
}

size_t* span_tree(size_t dim, bool adjacent[dim][dim], size_t root) {
  // Invalid dimension
  if (!dim) {
    perror("invalid spanning tree dimension");
    exit(EXIT_FAILURE);
  }
  // Root outside graph
  if (root >= dim) {
    perror("out of bounds");
    exit(EXIT_FAILURE);
  }

  size_t* parent = malloc(dim * sizeof(size_t));
  if (!parent) {
    perror("out of memory");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < dim; i++)
    parent[i] = SIZE_MAX; // any element i pointing to SIZE_MAX
                          // is not part of the spanning tree
  // root will point to itself
  parent[root] = root;

  // Array of bool flags for which nodes have already been explored.
  bool* explored = calloc(dim, sizeof(bool));
  if (!explored) {
    perror("out of memory");
    exit(EXIT_FAILURE);
  }
  explored[root] = true; // starts already known

  // initialize a queue with known size
  queue* Q = queue_init(dim);
  queue_add(Q, root);

  size_t temp; // temp variable for currently active queue element
  // while there are still elements to process in queue
  while (Q->end > Q->start) {
    // take first element in line in queue
    temp = queue_take(Q);
    // find, enqueue and mark as explored all its connected elements
    printf("elements connected to %zu: ", temp);
    for (size_t i = 0; i < dim; i++) {
      if (i == temp) continue;
      if (adjacent[i][temp] && !explored[i]) {
        queue_add(Q, i);
        explored[i] = true;
        parent[i] = temp;
        printf("%zu ", i);
      }
    }
    puts("");
  }

  queue_del(Q);
  free(explored);

  return parent;
}

queue* queue_init(size_t size) {
  queue* Q = malloc(sizeof(queue) + size * sizeof(size_t));
  if (!Q) {
    perror("out of memory");
    exit(EXIT_FAILURE);
  }

  Q->size = size;
  Q->start = 0;
  Q->end = 0;
  return Q;
}

void queue_del(queue* Q) {
  free(Q);
}

void queue_add(queue* Q, size_t n) {
  if (n > Q->size - 1) {
    perror("queue: out of bounds");
    exit(EXIT_FAILURE);
  }
  if (Q->end == Q->size) {
    perror("queue overflow");
    exit(EXIT_FAILURE);
  }

  Q->item[Q->end] = n;
  Q->end++;
}

size_t queue_take(queue* Q) {

  if (Q->start > Q->end) {
    perror("accessing empty queue");
    exit(EXIT_FAILURE);
  }

  Q->start++;
  return Q->item[Q->start-1];
}


