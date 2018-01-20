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




#endif
