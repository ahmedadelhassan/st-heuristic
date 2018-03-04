#ifndef ST_HEURISTIC_HEAP_H
#define ST_HEURISTIC_HEAP_H

#include <stdlib.h>

typedef struct {
    void **array;
    size_t size;
    size_t capacity;

    int (*compar)(const void *, const void *);
} heap_t;

extern heap_t *heap_alloc(size_t capacity, int (*ompar)(const void *, const void *));

extern void heap_release(heap_t *h);

extern void heap_release_with_data_release(heap_t *h, void (*data_release)(void *));

extern void heap_insert(heap_t *h, void *data);

extern void *heap_extract_rand(heap_t *h);

extern void *heap_extract_max(heap_t *h);

#endif /* ST_HEURISTIC_HEAP_H */
