#ifndef ST_HEURISTIC_INDIVIDUAL_H
#define ST_HEURISTIC_INDIVIDUAL_H

#include "graph.h"
#include "list.h"

typedef struct individual_t {
    size_t n_nodes;
    list_t *nodes;
    weight_t min_weight_spanning_tree;
} individual;

extern individual_t *individual_alloc(list_t *nodes);

extern void individual_release(individual_t *st);

#endif /* ST_HEURISTIC_INDIVIDUAL_H */
