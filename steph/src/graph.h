#ifndef __GRAPH__
#define __GRAPH__

typedef unsigned int vertex_idx;
typedef unsigned int weight;

#define WHITE 0
#define GREY  1
#define BLACK 2

typedef struct edge
{
  vertex_idx v;
  weight     w;
} edge;

typedef struct vertex
{
  unsigned int label;
  char         color;
  char         terminal,
  size_t       degree;
  size_t       n_alloc;
  edge*        edges;
} vertex;

typedef struct graph
{
  size_t  n_vertices;
  size_t  n_edges;
  size_t  n_terminals;
  vertex* vertices;
} graph;

extern graph* graph_alloc(size_t n_vertices);
extern void   graph_release(graph* g);
extern void   graph_adjust(graph* g);
extern void   graph_add_edge(graph* g, vertex_idx u_label, vertex_idx v_label, weight w);
extern void   graph_set_terminal(graph* g, vertex_idx u);

#endif /* __GRAPH__ */
