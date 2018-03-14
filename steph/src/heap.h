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

extern void heap_release(heap_t *h);

extern void heap_insert(heap_t *h, individual_t *ind);

extern void *heap_extract_rand(heap_t *h);

extern void *heap_extract_max(heap_t *h);

#endif /* ST_HEURISTIC_HEAP_H */
