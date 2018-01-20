#ifndef _GRAPH_COMMON__
#define _GRAPH_COMMON__

typedef struct graph_dense{
  int nb_nodes;
  int nb_edges;
  int** edges;
  int** dist;
  int** next;
  int* terminals;
}Graph_dense;

typedef struct edge{
  int src;
  int dst;
  int weigth;
}Edge;

typedef struct graph_sparse{
  int nb_nodes;
  int nb_edges;
  int nb_terminals;
  Edge* edges;
  int* terminals;  
}Graph_sparse;



#endif
