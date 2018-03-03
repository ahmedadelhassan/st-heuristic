#ifndef ST_HEURISTIC_UNION_FIND_H
#define ST_HEURISTIC_UNION_FIND_H

#include "graph.h"

typedef struct union_find_t {
    node_t *parent;      /**< Node's parents.                                   */
    size_t *rank;        /**< Node's ranks.                                     */
    size_t *n_terminals; /**< Number of terminals in the part rooted at a node. */
    size_t size;        /**< Number of nodes.                                  */
    size_t count;       /**< The number of parts.                              */
    graph_t *graph;       /**< The reference graph.                              */
} union_find_t;

extern union_find_t *union_find_alloc(graph_t *g);

extern void union_find_init(union_find_t *uf);

extern void union_find_release(union_find_t *uf);

extern node_t union_find_find_recursive_compression(union_find_t *uf, node_t i);

extern node_t union_find_find_iterative_splitting(union_find_t *uf, node_t i);

extern node_t union_find_find_iterative_halving(union_find_t *uf, node_t i);

extern node_t union_find_find_iterative_compression(union_find_t *uf, node_t i);

extern size_t union_find_union(union_find_t *uf, node_t i, node_t j);

#endif /* ST_HEURISTIC_UNION_FIND_H */
