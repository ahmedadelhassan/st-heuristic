#include "../include/tree.h"

#define SIZE_START 10
#define REALLOC_FACTOR 1.5

#include <stdlib.h>
#include <stdio.h>


Tree* allocate_tree(void){
  Tree* new;

  new = (Tree*)malloc(sizeof(Tree));
  if (new == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  new->edge_indices = (int*)malloc(SIZE_START*sizeof(int));
  if (new->edge_indices == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  new->size = 0;
  new->memory_size = SIZE_START;
  return new;
}


void free_tree(Tree* t){
  free(t->edge_indices);
  free(t);
}


int add_edge_tree(Tree* t, int edge){
  int* tmp;
  
  /* Reallocation case */
  if (t->size == t->memory_size){
    t->memory_size *= REALLOC_FACTOR;
    tmp = realloc(t->edge_indices, t->memory_size*sizeof(int));
    if (tmp == NULL){
      fprintf(stderr, "Reallocation memory error\n");
      return 0;
    }
    t->edge_indices = tmp;
  }
  t->edge_indices[t->size] = edge;
  t->size++;
  return 1;
}


int cmp_tree(const void* a1, const void* a2){
  const Tree* t1 = a1;
  const Tree* t2 = a2;
  long int diff, i;

  if ((diff = (t1->weight - t2->weight)))
    return diff;
  if ((diff = (t1->size - t2->size)))
    return diff;
  for (i=0 ; i<t1->size ; i++){
    if ((diff = (t1->edge_indices[i] - t2->edge_indices[i])))
      return diff;
  }
  return 0;
}


int cmp_int(const void* a1, const void* a2){
  const int* i1 = a1;
  const int* i2 = a2;

  return *i1 - *i2;
}


void sort_edge_in_tree(Tree* t){
  qsort(t->edge_indices, t->size ,sizeof(int), cmp_int);
}
