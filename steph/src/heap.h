#ifndef ST_HEURISTIC_HEAP_H
#define ST_HEURISTIC_HEAP_H

typedef struct heap_item_t {
    node_t node;
    size_t count;
} heap_item_t;

typedef enum {
    MIN_HEAP,
    MAX_HEAP
} heap_type_t;

typedef struct heap_t {
    void **array;
    size_t size;
    size_t alloc_size;
    heap_type_t type;
    int (compar)(const void*, const void*);
} heap_t;

extern heap_t *heap_alloc(heap_type_t type, size_t alloc_size, int (compar)(const void*, const void*));
extern void heap_release(heap_t *h);
extern void *heap_search(heap_t *h, const void *x);
extern int heap_insert(heap_t *h, const void *x);
extern void *heap_peek(heap_t *h);

#endif /* ST_HEURISTIC_HEAP_H */
