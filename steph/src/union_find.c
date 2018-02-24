#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "union_find.h"


union_find_t* union_find_alloc(size_t n) {
  union_find_t* uf = (union_find_t*) malloc(sizeof(union_find_t));

  if (uf == NULL) {
    fprintf(stderr, "%s\n", "memory allocation error");
    exit(EXIT_FAILURE);
  }

  uf->size  = n;
  uf->count = n;

  uf->parents = (int*) calloc(n, sizeof(int));
  if (uf->parents == NULL) {
    fprintf(stderr, "%s\n", "memory allocation error");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < uf->size; i++) {
    uf->parents[i] = i;
  }

  uf->ranks = (int*) calloc(n, sizeof(int));
  if (uf->ranks == NULL) {
    fprintf(stderr, "%s\n", "memory allocation error");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < uf->size; i++) {
    uf->ranks[i] = 0;
  }

  return(uf);
}


void union_find_release(union_find_t* uf) {
  if (uf) {
    if (uf->parents) {
      memset(uf->parents, 0x0, uf->size * sizeof(int));
      free(uf->parents);
    }

    if (uf->ranks) {
      memset(uf->ranks, 0x0, uf->size * sizeof(int));
      free(uf->ranks);
    }

    memset(uf, 0x0, sizeof(union_find_t));
    free(uf);
  }
}

int union_find_find_recursive_compression(union_find_t* uf, int i) {
  assert(uf != NULL);
  assert((uf->size == 0) || (uf->ranks != NULL));
  assert((uf->parents == 0) || (uf->parents != NULL));
  assert((i >= 0) && (i < uf->size));

  if (uf->parents[i] != uf->parents[uf->parents[i]]) {
    uf->parents[i] = union_find_find_recursive_compression(uf, uf->parents[i]);
  }

  return(uf->parents[i]);
}

int union_find_find_iterative_splitting(union_find_t* uf, int i) {
  assert(uf != NULL);
  assert((uf->size == 0) || (uf->ranks != NULL));
  assert((uf->parents == 0) || (uf->parents != NULL));
  assert((i >= 0) && (i < uf->size));

  int j = uf->parents[i];
  while (j != uf->parents[j]) {
    uf->parents[i] = uf->parents[j];
    i = j;
    j = uf->parents[j];
  }

  return(j);
}

int union_find_find_iterative_halving(union_find_t* uf, int i) {
  assert(uf != NULL);
  assert((uf->size == 0) || (uf->ranks != NULL));
  assert((uf->parents == 0) || (uf->parents != NULL));
  assert((i >= 0) && (i < uf->size));

  while (uf->parents[i] != uf->parents[uf->parents[i]]) {
    uf->parents[i] = uf->parents[uf->parents[i]];
    i = uf->parents[i];
  }

  return(uf->parents[i]);
}

int union_find_find_iterative_compression(union_find_t* uf, int i) {
  assert(uf != NULL);
  assert((uf->size == 0) || (uf->ranks != NULL));
  assert((uf->parents == 0) || (uf->parents != NULL));
  assert((i >= 0) && (i < uf->size));

  int p = uf->parents[i];
  int j = i;

  /* pass 1 */
  while (p != uf->parents[p]) {
    p = uf->parents[p];
  }

  /* pass 2 */
  while (j != p) {
    uf->parents[i] = p;
    i = j;
    j = uf->parents[i];
  }

  return(p);
}

void union_find_union(union_find_t* uf, int i, int j) {
  #ifdef DEBUG
  printf("union %d %d\n", i, j);
  #endif

  if (i == j) {
    return;
  }

  int i_root = union_find_find_iterative_compression(uf, i);
  int j_root = union_find_find_iterative_compression(uf, j);

  if (i_root == j_root) {
    return;
  }

  uf->count--;

  if (uf->ranks[i_root] == uf->ranks[j_root]) {
    uf->parents[j_root] = i_root;
    uf->ranks[i_root]++;
    return;
  }

  if (uf->ranks[i_root] < uf->ranks[j_root]) {
    uf->parents[i_root] = j_root;
  }
  else {
    uf->parents[j_root] = i_root;
  }
}

int main(int argc, char *argv[]) {
  int n = 10000000;
  int threshold = 10;

  srand(time(NULL));

  for (int i = 0; i < 10; i++) {
    union_find_t* uf = union_find_alloc(n);
    while (uf->count > threshold) {
      int i = rand() % n;
      int j = rand() % n;
      union_find_union(uf, i, j);
    }
    union_find_release(uf);
  }

  return(EXIT_SUCCESS);
}
