#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include "graph.h"
#include "node_list.h"
#include "union_find.h"

typedef struct individual_t {
    size_t n_non_terminal_nodes;
    node_list_t *non_terminal_nodes;
    weight_t weight;
} individual;

extern void individual_release(individual_t *st);

extern individual_t *individual_mk_rand(graph_t *g);

extern individual_t *individual_mk_rand_from_nodes(graph_t *g, individual_t *ind);

extern individual_t *individual_union(graph_t *g, individual_t *ind1, individual_t *ind2);

extern individual_t *individual_intersection(graph_t *g, individual_t *ind1, individual_t *ind2);

extern individual_t *individual_mutation(graph_t *g, individual_t *ind);


#endif /* ST_HEURISTIC_INDIVIDUAL_H */
