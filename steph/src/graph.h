#ifndef ST_HEURISTIC_UNION_GRAPH_H
#define ST_HEURISTIC_UNION_GRAPH_H

#include <stdio.h>

typedef unsigned int node_t;
typedef unsigned int weight_t;

/* WHITE, GREY, BLACK colors */
#define WHITE 0
#define GREY  1
#define BLACK 2

typedef struct edge_t {
    node_t src;
    node_t dest;
    weight_t weight
} edge_t;

typedef struct graph_t {
    size_t n_nodes;      /**< Number of nodes.      */
    node_t *nodes;
    size_t n_edges;      /**< Number of edges.      */
    edge_t *edges;        /**< Edges array.          */
    size_t n_terminals;  /**< Number of terminals.  */
    node_t *terminals;    /**< Terminals array.      */
} graph_t;


extern graph_t *graph_read(FILE *f);

extern void graph_write(FILE *f, graph_t g);

extern graph_t *graph_alloc();

extern void graph_release(graph_t *g);

extern void graph_init_colors(graph_t *g, color_t c);

extern list_t *graph_kruskal_min_spanning_tree(graph_t *g);

#endif /* ST_HEURISTIC_UNION_GRAPH_H */
