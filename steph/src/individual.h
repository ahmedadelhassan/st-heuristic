#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include "graph.h"
#include "list.h"
#include "pair.h"

typedef struct individual_t {
    size_t n_edges;
    edge_t *edges;
    weight_t total_weight;
} individual;

extern individual_t *individual_alloc(list_t *edges);

extern void individual_release(individual_t *individual);

extern individual_t *individual_mk(graph_t *p_g);

extern individual_t *individual_mk_with_init_edges(graph_t *p_g, list_t *init_el);

extern individual_t *individual_union(graph_t *p_g, individual_t *ind1, individual_t *ind2);

extern individual_t *individual_intersection(graph_t *p_g, individual_t *ind1, individual_t *ind2);

extern pair_t individual_crossing(graph_t *p_g, individual_t *individual1, individual_t *individual2, double p);

extern individual_t *p_individual_drop_out(graph_t *p_g, individual_t *p_individual, double p);

extern void individual_print(graph_t *p_g, individual_t *individual);

extern int individual_compar(const void* p1, const void *p2);

#endif /* ST_HEURISTIC_INDIVIDUAL_H */
