#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include "graph.h"
#include "list.h"
#include "union_find.h"

typedef struct individual_t {
    size_t n_nodes;
    list_t *nodes;
    weight_t min_weight_spanning_tree;
} individual;

extern void individual_release(individual_t *st);

extern individual_t *individual_mk_rand(graph_t *g);

extern individual_t *individual_mk_rand_from_nodes(graph_t *g, individual_t *ind);

#endif /* ST_HEURISTIC_INDIVIDUAL_H */
