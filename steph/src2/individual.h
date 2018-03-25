#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include <stdio.h>

#include "bvector.h"
#include "graph.h"
#include "pool.h"
#include "weight.h"

typedef struct  {
    bvector_t *p_bvector;
    weight_t total_weight;
} individual_t;

typedef struct {
    individual_t individual1;
    individual_t individual2;
} individuals_t;

extern void individual_cleanup(pool_t *p_pool, individual_t individual);

extern individual_t individual_mk(pool_t *p_pool, graph_t *p_graph);

extern individual_t individual_union(pool_t *p_pool, graph_t *p_graph, individual_t individual1, individual_t individual2);

extern individual_t individual_intersection(pool_t *p_pool, graph_t *p_graph, individual_t individual1, individual_t individual2);

extern individuals_t individual_crossing(pool_t *p_pool, graph_t *p_graph, individual_t individual1, individual_t individual2);

extern individual_t individual_drop_out(pool_t *p_pool, graph_t *p_graph, individual_t individual, double probability);

extern individual_t individual_insert(pool_t *p_pool, graph_t *p_graph, individual_t individual, double probability);

extern void individual_fprint(FILE *f, graph_t *p_graph, individual_t individual);

#endif /* ST_HEURISTIC_INDIVIDUAL_H */
