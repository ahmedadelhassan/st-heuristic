#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include <stdio.h>

#include "graph.h"
#include "pair.h"

#define INDIVIDUAL_MK_SIMPLE
#define INDIVIDUAL_MK_REDUCED

#undef INDIVIDUAL_MK_REDUCED

typedef struct  {
    size_t n_edges;
    edge_t *p_edges;
    weight_t total_weight;
} individual_t;

typedef struct {
    individual_t individual1;
    individual_t individual2;
} individuals2_t;

extern void individual_cleanup(individual_t individual);

extern individual_t individual_mk(graph_t *p_g, edge_list_t *p_init_el);

extern individual_t individual_union(graph_t *p_g, individual_t individual1, individual_t individual2);

extern individual_t individual_intersection(graph_t *p_g, individual_t individual1, individual_t individual2);

extern individuals2_t individual_crossing(graph_t *p_g, individual_t individual1, individual_t individual2, double p);

extern individual_t individual_drop_out(graph_t *p_g, individual_t individual, double p);

extern void individual_print(FILE *f, graph_t *p_g, individual_t individual);

extern int individual_compar(individual_t individual1, individual_t individual2);

#endif /* ST_HEURISTIC_INDIVIDUAL_H */
