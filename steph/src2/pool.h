#ifndef ST_HEURISTIC_POOL_H
#define ST_HEURISTIC_POOL_H

#include <stdlib.h>
#include "bvector.h"
#include "color.h"

typedef struct pool_item_t {
    struct pool_item_t *p_next;
    bvector_t *p_bvector;
} pool_item_t;

typedef struct {
    pool_item_t *p_pool_ready_items;
    pool_item_t *p_pool_empty_items;
    size_t n_bools;
} pool_t;

extern pool_t *pool_alloc();

extern void pool_release(pool_t *p_pool);

extern bvector_t *pool_get(pool_t *p_pool);

extern void pool_return(pool_t *p_pool, bvector_t *p_bvector);

#endif /* ST_HEURISTIC_POOL_H */
