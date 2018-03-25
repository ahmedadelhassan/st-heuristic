#include "../include/graph.h"

#include <stdlib.h>


static int cmp_int(const void* a1, const void* a2){
  const int* i1 = a1;
  const int* i2 = a2;

  return *i1 - *i2;
}


void sort_terminals(Graph_sparse* g){
  qsort(g->terminals, g->nb_terminals, sizeof(int), cmp_int);
}


int is_terminal(Graph_sparse* g, int index){
  int start = 0;
  int end = g->nb_terminals - 1;
  int middle;

  if (g->terminals[0] == index){
    return 1;
  }
  else{
    while (end - start > 1){
      middle = (start + end) / 2;
      if (g->terminals[middle] < index)
	start = middle;
      else
	end = middle;
    }
  }
  return g->terminals[end] == index;
}
