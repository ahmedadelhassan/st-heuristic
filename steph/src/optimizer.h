#ifndef ST_HEURISTIC_OPTIMIZER_H
#define ST_HEURISTIC_OPTIMIZER_H

#include "graph.h"


typedef struct optimizer_t {
    optimizer_config_t config;
    individual_t **individuals;
    weight_t best_total_weight;
    weight_t worst_total_weight;
} optimizer_t;

extern optimizer_t optimizer_init(optimizer_config_t config);

extern void optimizer_release(optimizer_t opt);

extern void optimizer_run(optimizer_t opt);

extern list_t *optimizer_values(optimizer_t *opt);

extern individual_t *optimizer_best_individual(optimizer_t opt);

extern individual_t *optimizer_worst_individual(optimizer_t opt);

#endif /* ST_HEURISTIC_OPTIMIZER_H */
