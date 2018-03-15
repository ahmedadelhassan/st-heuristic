#ifndef ST_HEURISTIC_OPTIMIZER_H
#define ST_HEURISTIC_OPTIMIZER_H

#include "configuration.h
#include "graph.h"

typedef struct optimizer_t {
    configurations_t configuration;
    population_t *p_population;
    weight_t min_total_weight;
    weight_t max_total_weight;
} optimizer_t;

extern void optimizer_run(void optimizer_run(configuration_t configuration));


#endif /* ST_HEURISTIC_OPTIMIZER_H */
