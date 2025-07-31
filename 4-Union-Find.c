#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

//initialization function that makes parent the singleton partition
void MakeSingles(
  size_t const dim,
  size_t parent[dim]);

//build tree structure pseudorandomly
void RandomAdoption(
  size_t const dim,
  size_t parent[dim]);

void PrintForest(
  size_t const dim,
  size_t const parent[dim]);

//for a given index, find the root of its tree
size_t Find(
  size_t const dim,
  size_t const parent[dim],
  size_t const elem);

//"implement a FindReplace function that changes all parent entries
//on a path to the root (including) to a specific value"
void FindReplace(
  size_t const dim,
  size_t parent[dim],
  size_t const elem,
  size_t const value);

//this one replaces the whole tree the element belongs to instead
void ReplaceTree(
  size_t const dim,
  size_t parent[dim],
  size_t const elem,
  size_t const value);

//"implement a FindCompress function that changes all parent entries
//to the root that has been found"
void FindCompress(
  size_t const dim,
  size_t parent[dim],
  size_t const root);

//"implement a Union function that, for two given elements,
//combines their trees into one.
//Use FindCompress for one side and FindReplace for the other."
void Union(
  size_t const dim,
  size_t parent[dim],
  size_t const root_c,
  size_t const root_r);

int main() {
  srand(time(NULL));
  //forest size
  size_t dim = 0;
  //tree to compress
  size_t comp  = SIZE_MAX;
  //tree to replace
  size_t repl = SIZE_MAX;

  do {
    printf("Select forest size: ");
    scanf(" %zu", &dim);
  } while (dim < 1);

  size_t* parent = calloc(dim, sizeof(size_t));
  if (!parent) {
    perror("Out of memory");
    return EXIT_FAILURE;
  }

  MakeSingles(dim, parent);
  RandomAdoption(dim, parent);
  PrintForest(dim, parent);

  printf("Select two trees by pointing to belonging elements.\n");
  printf("Both trees will have their elements\n");
  printf("point directly to the first's root.\n");

  do {
    printf("Enter first element: ");
    scanf(" %zu", &comp);
  } while (comp >= dim);
  do {
    printf("Enter second element: ");
    scanf(" %zu", &repl);
  } while (repl >= dim);

  size_t root_c = Find(dim, parent, comp);
  size_t root_r = Find(dim, parent, repl);

  Union(dim, parent, root_c, root_r);
  PrintForest(dim, parent);

  free(parent);
  return EXIT_SUCCESS;
}

void MakeSingles(
    size_t const dim,
    size_t parent[dim]) {
  for (int i = 0; i < dim; i++) {
    parent[i] = SIZE_MAX;
  }
}

void RandomAdoption(
    size_t const dim,
    size_t parent[dim]) {
  //let the user select n. of trees
  size_t pop = 0;
  do {
    printf("Select number of trees: ");
    scanf(" %zu", &pop);
    if (pop == 0) {
      printf("Must be more than zero\n");
    }
    if (pop > dim) {
      printf("Must be less than or equal to %zu\n", dim);
    }
  } while (pop < 1 || pop > dim);
  //assign children randomly
  for (size_t i = pop; i < dim; i++) {
    parent[i] = rand() % i;
  }
}

void PrintForest(
    size_t const dim,
    size_t const parent[dim]) {
  printf("[");
  for (size_t i = 0; i < dim; i++) {
    if (parent[i] == SIZE_MAX) {
      printf("R, ");
    } else {
      printf("p[%zu]=%zu, ", i, parent[i]);
    }
  }
  printf(" ]\n");
}

size_t Find(
    size_t const dim,
    size_t const parent[dim],
    size_t const elem) {
  if (elem >= dim) {
    perror("Index outside bounds");
    exit(EXIT_FAILURE);
  }

  if (parent[elem] == SIZE_MAX) {
    return elem;
  } else {
    return Find(dim, parent, parent[elem]);
  }
}

void FindReplace(
    size_t const dim,
    size_t parent[dim],
    size_t const elem,
    size_t const value) {
  if (elem >= dim) {
    perror("Index outside bounds");
    exit(EXIT_FAILURE);
  }
  if (parent[elem] != SIZE_MAX) {
    FindReplace(dim, parent, parent[elem], value);
  }
  parent[elem] = value;
}

void ReplaceTree(
    size_t const dim,
    size_t parent[dim],
    size_t const elem,
    size_t const value) {
  size_t root = Find(dim, parent, elem);

  //mark members of the tree
  bool* heir = calloc(dim, sizeof(bool));
  if (!heir) {
    perror("Out of memory");
    exit(EXIT_FAILURE);
  }

  heir[root] = true;

  //continue until last generation is reached
  for (bool lastgen = true; !lastgen;) {
    lastgen = true;
    //iterate through each element to check if it belongs to the
    //tree until there are none left
    for (int i = 0; i < dim; i++) {
      if (!heir[i] && heir[parent[i]]) {
        heir[i] = true;
        lastgen = false;
      }
    }
  }

  //overwrite every element of the tree with the new value
  for (int i = 0; i < dim; i++) {
    if (heir[i]) {
      parent[i] = value;
    }
  }
  free(heir);
}

void FindCompress(
    size_t const dim,
    size_t parent[dim],
    size_t const root) {
  if (root >= dim) {
    perror("Index outside bounds");
    exit(EXIT_FAILURE);
  }

  //mark members of the tree
  bool* heir = calloc(dim, sizeof(bool));
  if (!heir) {
    perror("Out of memory");
    exit(EXIT_FAILURE);
  }
  heir[root] = true;

  //continue until last generation is reached
  for (bool lastgen = true; !lastgen;) {
    lastgen = true;
    //iterate through each element to check if it belongs to the
    //tree until there are none left
    for (int i = 0; i < dim; i++) {
      if (!heir[i] && heir[parent[i]]) {
        heir[i] = true;
        lastgen = false;
      }
    }
  }

  //make all heirs point to root
  for (int i = 0; i < dim; i++) {
    if (i == root) continue;
    if (heir[i]) {
      parent[i] = root;
    }
  }

  free(heir);
}

void Union(
    size_t const dim,
    size_t parent[dim],
    size_t const root_c,
    size_t const root_r) {
  FindCompress(dim, parent, root_c);
  if (root_r != root_c) {
    ReplaceTree(dim, parent, root_r, root_c);
  }
}
