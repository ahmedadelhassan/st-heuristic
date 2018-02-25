#ifndef ST_HEURISTIC_UNION_GRAPH_H
#define ST_HEURISTIC_UNION_GRAPH_H

#include <stdio.h>

typedef unsigned int node_t;
typedef unsigned int weight_t;

#define WHITE 0
#define GREY  1
#define BLACK 2

typedef struct edge_t
{
    vertex_t src;
    vertex_t dest;
    weight_t weight
} edge_t;

typedef struct graph_t
{
    size_t    n_nodes;
    size_t    n_edges;
    size_t    n_terminals;
    edge_t*   edges;
    vertex_t* terminals;
} graph_t;

extern graph_t* graph_read(FILE* f);
extern void     graph_write(FILE* f, graph_t g);
extern graph_t* graph_alloc();
extern void     graph_release(graph_t* g);

#endif /* ST_HEURISTIC_UNION_GRAPH_H */
