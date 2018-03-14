#ifndef ST_HEURISTIC_CONFIGURATION_H
#define ST_HEURISTIC_CONFIGURATION_H

#include "graph.h"
#include "random.h"

typedef struct {
    probability_t event_probability;
} configuration_union_t;

typedef struct {
    probability_t event_probability;
} configuration_intersection_t;

typedef struct {
    probability_t event_probability;
    probability_t crossing_probability;
} configuration_crossing_t;

typedef struct {
    probability_t event_probability;
    probability_t drop_out_probability;
} configuration_drop_out_t;

typedef struct {
    probability_t event_probability;
    double percentage;
} configuration_renew_t;

typedef struct {
    graph_t *graph;
    size_t n_individuals;
    size_t n_epochs;
    configuration_union_t configuration_union;
    configuration_intersection_t configuration_intersection;
    configuration_crossing_t configuration_crossing;
    configuration_drop_out_t configuration_drop_out;
    configuration_renew_t configuration_renew;
} configuration_t;

#endif /* ST_HEURISTIC_CONFIGURATION_H */
