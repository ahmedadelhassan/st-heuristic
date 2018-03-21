#ifndef ST_HEURISTIC_GRAPH_H
#define ST_HEURISTIC_GRAPH_H

#include <stdlib.h>
#include <stdio.h>

#include "bvector.h"
#include "edge.h"
#include "node.h"
#include "edge_list.h"
#include "weight.h"

typedef struct {
    node_t *p_parents;     /**< Node's parents.                                   */
    size_t *p_ranks;       /**< Node's ranks.                                     */
    size_t *p_n_terminal_nodes; /**< Number of terminals in the part rooted at a node. */
    size_t count;          /**< The number of parts.                              */
    size_t max_connected_terminal_nodes;
} union_find_t;

typedef struct {
    int start_idx;
    size_t len;
} plateau_t;

typedef struct {
    plateau_t *p_plateaux;
    size_t n_plateaux;
} plateaux_t;

typedef struct graph_t {
    size_t n_nodes;                      /**< Number of nodes.                                    */
    size_t n_terminal_nodes;             /**< Number of terminal nodes                            */
    size_t n_non_terminal_nodes;         /**< Number of non-terminal nodes                        */
    int *p_node_is_terminal;             /**< p_node_is_terminal[n] != 0 if node n is terminal    */
    node_t fst_terminal_node;
    bvector_t *p_bvector;
    int *p_node_counters;                /**< A counter for each node                             */
    size_t n_edges;                      /**< Number of edges.                                    */
    edge_t *p_edges_sorted_by_endpoints; /**< Edges' array. (edges are sorted on n1 and next n2). */
    edge_t *p_edges_sorted_by_weight;    /**< Edges' array. (edges are sorted on weight).         */
    edge_t *p_edges_no_order_guaranteed; /**< Edges' array. (no order is guaranteed)              */
    plateaux_t plateaux;
    union_find_t union_find;             /**< Union find facility.                                */
} graph_t;


extern graph_t *graph_read(FILE *f);

extern void graph_write(FILE *f, graph_t *p_g);

extern graph_t *graph_alloc();

extern void graph_release(graph_t *p_g);

extern int graph_node_is_terminal(graph_t *p_g, node_t i);

extern int graph_node_is_non_terminal(graph_t *p_g, node_t i);

extern void graph_node_counter_assert_all(graph_t *p_g, int counter_val);

extern void graph_node_counter_assert_all_zero(graph_t *p_g);

extern void graph_node_counter_set_all(graph_t *p_g, int counter_val);

extern void graph_node_counter_set(graph_t *p_g, node_t i, int counter_val);

extern void graph_node_counter_reset_all(graph_t *p_g);

extern void graph_node_counter_reset(graph_t *p_g, node_t i);

extern void graph_node_counter_increment_all(graph_t *p_g);

extern void graph_node_counter_increment(graph_t *p_g, node_t i);

extern int graph_node_counter_get(graph_t *p_g, node_t i);

extern void graph_edges_random_shuffle(graph_t *p_g);

extern void graph_edges_random_shuffle_weight_respectful(graph_t *p_g);

extern void graph_union_find_init(graph_t *p_g);

extern node_t graph_union_find_find(graph_t *p_g, node_t u);

extern size_t graph_union_find_get_max_connected_terminal_nodes(graph_t *p_g);

extern int graph_union_find_union(graph_t *p_g, node_t u, node_t v);

extern edge_t *graph_search_edge_by_endpoints(graph_t *p_g, edge_t e);

extern edge_list_t *graph_mst(graph_t *p_g);

#endif /* ST_HEURISTIC_GRAPH_H */
