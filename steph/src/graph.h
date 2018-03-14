#ifndef ST_HEURISTIC_GRAPH_H
#define ST_HEURISTIC_GRAPH_H

#include <stdlib.h>

#include "color.h"
#include "edge.h"
#include "node.h"
#include "list.h"
#include "weight.h"

typedef struct {
    node_t *parents;     /**< Node's parents.                                   */
    size_t *ranks;       /**< Node's ranks.                                     */
    size_t *n_terminals; /**< Number of terminals in the part rooted at a node. */
    size_t count;        /**< The number of parts.                              */
} union_find_t;


typedef struct graph_t {
    size_t n_nodes;          /**< Number of nodes.             */
    size_t n_terminals;      /**< Number of terminal nodes     */
    node_t *terminals;       /**< Terminal nodes               */
    size_t n_non_terminals;  /**< Number of non-terminal nodes */
    node_t *non_terminals;   /**< Non-terminal nodes           */
    color_t *node_colors;    /**< Nodes' colors                */
    int *node_counters;      /**< nodes' counters              */
    size_t n_edges;          /**< Number of edges.             */
    edge_t *edges_sorted_by_endpoints;           /**< Edges' array. (edges are sorted on n1 and next n2). */
    edge_t *edges_sorted_by_weight;    /**< Edges' array. (edges are sorted on weight).         */
    edge_t *edges_no_order_guaranteed;      /**< Edges' array. (no order is guaranteed)             */
    union_find_t union_find; /**< Union find facility.     **/
} graph_t;


extern graph_t *p_graph_read(FILE *f);

extern void graph_write(FILE *f, graph_t *p_g);

extern graph_t *p_graph_alloc();

extern void graph_release(graph_t *p_g);

extern int graph_node_is_terminal(graph_t *p_g, node_t i);

extern int graph_node_is_non_terminal(graph_t *p_g, node_t i);

extern void graph_node_color_assert_all(graph_t *p_g, color_t c);

extern void graph_node_color_assert_all_white(graph_t *p_g);

extern void graph_node_color_assert_all_black(graph_t *p_g);

extern void graph_node_color_set_all(graph_t *p_g, color_t c);

extern void graph_node_color_set(graph_t *p_g, node_t i, color_t c);

extern color_t graph_node_color_get(graph_t *p_g, node_t i);

extern void graph_node_counter_assert_all(graph_t *p_g, int counter_val);

extern void graph_node_counter_assert_all_zero(graph_t *p_g);

extern void graph_node_counter_set_all(graph_t *p_g, int counter_val);

extern void graph_node_counter_set(graph_t *p_g, node_t i, int counter_val);

extern void graph_node_counter_reset_all(graph_t *p_g);

extern void graph_node_counter_reset(graph_t *p_g, node_t i);

extern void graph_node_counter_increment_all(graph_t *p_g);

extern void graph_node_counter_increment(graph_t *p_g, node_t i);

extern color_t graph_node_counter_get(graph_t *p_g, node_t i);

extern void graph_edge_random_shuffle(graph_t *p_g);

extern edge_list_t *graph_kruskal_min_spanning_tree_on_black_nodes(graph_t *p_g);

extern void graph_union_find_init(graph_t *p_g);

extern node_t graph_union_find_find(graph_t *p_g, node_t u);

extern int graph_union_find_union(graph_t *p_g, node_t u, node_t v);

extern edge_t *graph_search_edge_by_endpoints(graph_t *p_g, edge_t e);

#endif /* ST_HEURISTIC_GRAPH_H */
