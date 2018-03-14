#ifndef ST_HEURISTIC_POPULATION_H
#define ST_HEURISTIC_POPULATION_H

#include "heap.h"

typedef struct {
    unsigned int n_epochs;
    heap_t *heap;
} population_t;

extern population_t *population_alloc();

extern void population_release(population_t *population);

#endif /* ST_HEURISTIC_POPULATION_H */
