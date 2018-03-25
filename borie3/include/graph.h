#ifndef __GRAPH_
#define __GRAPH_


typedef struct edge{
  int src;       /* index of the source */
  int dst;       /* index of the destination */
  int weight;    /* weight of this edge */
}Edge;


typedef struct graph_sparse{
  int nb_nodes;       /* number of nodes in the graph */
  int nb_edges;       /* number of edges in the graph */
  int nb_terminals;   /* number of terminals nodes in the graph */
  Edge* edges;        /* pointer to array of all edges in both ways */
  int* terminals;     /* pointer to the array of indices of terminals */
}Graph_sparse;


void sort_terminals(Graph_sparse* g);
int is_terminal(Graph_sparse* g, int index);

#endif
