#ifndef __TOUCH_REDUCE_
#define __TOUCH_REDUCE_

#include "../include/graph.h"
#include "../include/tree.h"

typedef struct touch_reduce{
  int* touch;
  int nb_nodes;
}Touch_reduce;

typedef struct edge_reduce{
  int* edge_indices;
  int nb_edges;
}Edge_reduce;


void init_touch_reduce(int nb_nodes);
void reset_touch_reduce(void);
void add_edge_touch_reduce(int src, int dst);
void rm_edge_touch_reduce(int src, int dst);

void init_edge_reduce(int nb_edges);
void reduce_tree(Graph_sparse* g, Tree* t);

#endif
