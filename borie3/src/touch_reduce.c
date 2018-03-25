#include "../include/touch_reduce.h"

#include <stdlib.h>
#include <stdio.h>

/* WE HAVE A SINGLE THREAD, SINGLE PROCESSUS PROGRAM  */
/* WE CAN USE A SINGLE UNION-FIND STRUCTURE AT A TIME */
/* BEING A GLOBAL STATIC VARIABLE */
/* WE WILL NEED A SINGLE ALLOCATION FOR THE WHOLE EXECUTION */
static Touch_reduce TR;

static Edge_reduce ER;


void init_touch_reduce(int nb_nodes){
  TR.touch = (int*)malloc((nb_nodes+1)*sizeof(int));
  if (TR.touch == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  TR.nb_nodes = nb_nodes;
}


void reset_touch_reduce(void){
  int i;

  for (i=0 ; i<=TR.nb_nodes ; i++){
    TR.touch[i] = 0;
  }
}


void add_edge_touch_reduce(int src, int dst){
  TR.touch[src] += 1;
  TR.touch[dst] += 1;
}


void rm_edge_touch_reduce(int src, int dst){
  TR.touch[src] += -1;
  TR.touch[dst] += -1;
}


void init_edge_reduce(int nb_edges){
  ER.edge_indices = (int*)malloc((nb_edges)*sizeof(int));
  if (ER.edge_indices == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  ER.nb_edges = nb_edges;
}


void record_edge_tree(Tree* t){
  int i;

  for (i=0 ; i<ER.nb_edges ; i++){
    ER.edge_indices[i] = 0;
  }
  for (i=0 ; i<t->size ; i++){
    ER.edge_indices[t->edge_indices[i]] = 1;
  }
}


int remove_edge_tree(Graph_sparse* g, int edge){
  int src;
  int dst;
  
  if (ER.edge_indices[edge]){
    src = g->edges[edge].src;
    dst = g->edges[edge].dst;
    if ((TR.touch[src] == 1) && (!is_terminal(g, src))){
      ER.edge_indices[edge] = 0;
      rm_edge_touch_reduce(src, dst);
      return 1;
    }
    else if ((TR.touch[dst] == 1) && (!is_terminal(g, dst))){
      ER.edge_indices[edge] = 0;
      rm_edge_touch_reduce(src, dst);
      return 1;
    }
  }
  return 0;
}


void reduce_tree_edge(Graph_sparse* g, Tree* t){
  int remove = 1;
  int i;
  
  record_edge_tree(t);
  while (remove){
    remove = 0;
    for (i=0 ; i<t->size ; i++){
      remove += remove_edge_tree(g, t->edge_indices[i]);
    }
  }
}


void reduce_tree(Graph_sparse* g, Tree* t){
  int i;
  int k=0;
  
  reduce_tree_edge(g, t);
  for (i=0 ; i<t->size ; i++){
    if (ER.edge_indices[t->edge_indices[i]]){
      t->edge_indices[k] = t->edge_indices[i];
      k++;
    }
  }
  t->size = k;
}
