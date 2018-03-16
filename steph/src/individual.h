#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include "graph.h"
#include "list.h"
#include "pair.h"

typedef struct individual_t {
    size_t n_edges;
    edge_t **p_edges;
    weight_t total_weight;
} individual_t;

extern void individual_release(individual_t *p_individual);

extern individual_t *individual_mk(graph_t *p_g);

extern individual_t *individual_mk_with_init_edges(graph_t *p_g, list_t *init_el);

extern individual_t *individual_union(graph_t *p_g, individual_t *p_individual1, individual_t *p_individual2);

extern individual_t *individual_intersection(graph_t *p_g, individual_t *p_individual1, individual_t *p_individual2);

extern pair_t individual_crossing(graph_t *p_g, individual_t *p_individual1, individual_t *p_individual2, double p);

extern individual_t *individual_drop_out(graph_t *p_g, individual_t *p_individual, double p);

extern void individual_print(graph_t *p_g, individual_t *p_individual);

extern int individual_compar(const void* p1, const void *p2);

extern int individual_compar_p(const void* p1, const void *p2);

#endif /* ST_HEURISTIC_INDIVIDUAL_H */
