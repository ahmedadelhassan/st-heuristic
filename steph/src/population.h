#ifndef ST_HEURISTIC_POPULATION_H
#define ST_HEURISTIC_POPULATION_H

#include "individual.h"

typedef struct {
    size_t capacity;
    size_t n_individuals;
    individual_t **p_individuals;
    weight_t min_total_weight;
    weight_t max_total_weight;
} population_t;

extern population_t *p_population_alloc(size_t capacity);

extern void population_release(population_t *p_population);

extern void population_insert(population_t *p_population, individual_t *ind);

extern individual_t *population_extract_rand_individual(population_t *p_population);

extern individual_t *population_extract_max_individual(population_t *p_population);

extern individual_t *population_extract_min_individual(population_t *p_population);

extern weight_t population_get_max_weight(const population_t *p_population);

extern weight_t population_get_min_weight(const population_t *p_population);

extern void population_statistics_print(const population_t *p_population);

#endif /* ST_HEURISTIC_POPULATION_H */
