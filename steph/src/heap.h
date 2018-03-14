#ifndef ST_HEURISTIC_HEAP_H
#define ST_HEURISTIC_HEAP_H

#include <stdlib.h>

#include "individual.h"

typedef struct {
    size_t capacity;
    size_t size;
    void *array;
} heap_t;

extern heap_t *heap_alloc(size_t capacity, int (*data_compar)(const void *, const void *));

extern void individual_heap_release(individual_heap_t *h);

extern void individual_heap_insert(individual_heap_t *h, individual_t *ind);

extern individual_t *individual_heap_extract_rand(individual_heap_t *h);

extern individual_t *individual_heap_extract_max(individual_heap_t *h);

#endif /* ST_HEURISTIC_HEAP_H */
