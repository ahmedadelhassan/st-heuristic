#ifndef ST_HEURISTIC_GRAPH_H
#define ST_HEURISTIC_GRAPH_H

#include <stdlib.h>

#include "color.h"
#include "edge.h"
#include "node.h"
#include "list.h"
#include "weight.h"

typedef struct graph_t {
    size_t n_nodes;     /**< Number of nodes.     */
    color_t *node_colors;      /**< Nodes' array         */
    int *node_counters;
    size_t n_terminals; /**< Number of terminals. */
    size_t n_non_terminals; /**< Number of terminals. */
    node_t min_terminal_node;
    int *node_terminals;
    size_t n_edges;     /**< Number of edges.     */
    edge_t *edges;      /**< Edges' array.        */
} graph_t;


extern graph_t *graph_read(FILE *f);

extern void graph_write(FILE *f, graph_t *g);

extern void graph_release(graph_t *g);

extern int graph_node_is_terminal(graph_t *g, node_t i);

extern int graph_node_is_non_terminal(graph_t *g, node_t i);

extern void graph_node_color_set_all(graph_t *g, color_t c);

extern void graph_node_color_set(graph_t *g, node_t i, color_t c);

extern color_t graph_node_color_get(graph_t *g, node_t i);

extern void graph_node_counter_set_all(graph_t *g, int counter);

extern void graph_node_counter_set(graph_t *g, node_t i, int counter);

extern void graph_node_counter_reset_all(graph_t *g);

extern void graph_node_counter_reset(graph_t *g, node_t i);

extern void graph_node_counter_increment_all(graph_t *g);

extern void graph_node_counter_increment(graph_t *g, node_t i);

extern color_t graph_node_counter_get(graph_t *g, node_t i);

extern void graph_random_shuffle_edges(graph_t *g);

extern list_t *graph_kruskal_min_spanning_tree(graph_t *g);

#endif /* ST_HEURISTIC_GRAPH_H */
