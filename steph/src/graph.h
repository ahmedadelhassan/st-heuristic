#ifndef __GRAPH__
#define __GRAPH__

#include "list.h"

typedef struct edge
{
  unsigned int v_label;
  unsigned int w;
} edge;

typedef struct vertex
{
  unsigned int label;
  char         flag;
  list*        edges;
} vertex;

typedef struct graph
{
  size_t  n_vertices;
  size_t  n_edges;
  vertex* vertices;
} graph;

extern graph* graph_alloc(size_t n_vertices);
extern void   graph_release(graph* g);
extern void   graph_add_edge(graph* g, unsigned int u_label, unsigned int v_label, unsigned int w);


#endif /* __GRAPH__ */
